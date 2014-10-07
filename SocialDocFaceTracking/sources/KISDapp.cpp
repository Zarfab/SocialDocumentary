#include "KISDapp.h"
#include "OSCUtils.h"
#include <Fubi\Fubi.h>
#include <iostream>


KISDapp::KISDapp(bool display) :
showRgb(display), options(Fubi::RenderOptions::None), sendIntersection(false), jointAttentionState(false)
{
}


KISDapp::~KISDapp()
{
	// Now release Fubi
	Fubi::release();
	// And all allocated buffers
	delete[] g_rgbData;

	// close OSC connections
	m_sender.close();
	m_receiver.close();
}

void KISDapp::init(const std::vector<std::string>& paths,
	Fubi::SensorType::Type sensorType, 
	bool seatedSkel, 
	DisplayOptions dispOpt,
	std::vector<std::pair<std::string, int>> clientsIP,
	bool sendGazeCoord,
	std::vector<int> ports,
	const Fubi::FilterOptions & filter)
{
	setDisplayOptions(dispOpt);
	sendIntersection = sendGazeCoord;

	Fubi::SkeletonTrackingProfile::Profile skelTrackPro;
	if (seatedSkel)
		skelTrackPro = Fubi::SkeletonTrackingProfile::UPPER_BODY;
	else
		skelTrackPro = Fubi::SkeletonTrackingProfile::ALL;

	if (Fubi::init(Fubi::SensorOptions(Fubi::StreamOptions(), Fubi::StreamOptions(), Fubi::StreamOptions(-1, -1, -1), sensorType, skelTrackPro, true, false), filter))
	{
		Fubi::getRgbResolution(rgbWidth, rgbHeight);
			if (rgbWidth > 0 && rgbHeight > 0)
				g_rgbData = new unsigned char[rgbWidth*rgbHeight * 3];
			manager = new UserManager(showRgb, rgbWidth, rgbHeight, paths);
			m_sender.init(clientsIP);
			m_receiver.init(ports);
			
			// test the connection
			OSCMessage mes1;
			mes1.text = "/connectiontest";
			m_sender.send(mes1);
		
		return;
	}	
	else
		throw std::exception("Initilisation failed");
}


void KISDapp::run()
{
	int key = 0;
	
	float fps = 0;
	double time = Fubi::getCurrentTime();
	int frames = 0;

	while (key != 'q')
	{
		//calculate fps every 2 seconds
		frames++;
		double newTime = Fubi::getCurrentTime();
		if (newTime >= time + 2.0)
		{
			fps = (float)(frames / (newTime - time));
			time = newTime;
			frames = 0;
		}


		// Update the sensor	
		Fubi::updateSensor();

		unsigned char* buffer = g_rgbData;
		
		// get native image for the manager
		Fubi::ImageType::Type type = Fubi::ImageType::Color;
		Fubi::ImageNumChannels::Channel numChannels = Fubi::ImageNumChannels::C3;
		getImage(buffer, type, numChannels, Fubi::ImageDepth::D8, Fubi::RenderOptions::SwapRAndB);
		cv::Mat rgb = cv::Mat(rgbHeight, rgbWidth, CV_8UC3, buffer);
		

		// get OSC messages
		bool reset = false;
		if (m_receiver.messageReceived())
		{
			OSCMessage messageReceived = m_receiver.getMessage();
			if (messageReceived.text == "/player/next")
			{
				std::cout << "reinitialise time count" << std::endl;
				reset = true;
			}
		}
			
		// update the manager
		manager->update(rgb, reset);
		
		// send OSC messages
		if (manager->nbUsersHasChanged())
		{
			//d::cout << "number of users changed : " << manager->getNbUsers() << std::endl;
			OSCMessage message;
			message.text = "/context/nbusers";
			message.values.push_back((float)(manager->getNbUsers()));
			m_sender.send(message);
		}
		if (manager->nbFaceTrackedUsersChanged())
		{
			OSCMessage message;
			message.text = "/context/facetrackedusers";
			std::vector<unsigned short> tempusers = manager->getFaceTrackedUsers();
			for (unsigned int i = 0; i < tempusers.size(); i++)
				message.values.push_back((float)(tempusers[i]));
			m_sender.send(message);
		}
		std::vector<unsigned short> attChange = manager->attentionChanged();
		for (unsigned int i = 0; i < attChange.size(); i++)
		{				
			FubiUser* user = Fubi::getUser(attChange[i]);
			std::cout << "user attention engaged" << std::endl;
			OSCMessage message;
			message.text = "/context/user/attention";
			message.values.push_back((float)(user->m_id));
			message.values.push_back((float)user->m_screenWatched);
			message.values.push_back((float)(user->m_interest));
			m_sender.send(message);
		}
		if (manager->jointAttentionStart())
		{
			//std::cout << "joint attention engaged" << std::endl;
			OSCMessage message;
			message.text = "/context/jointattention";
			message.values.push_back(1);
			m_sender.send(message);
			jointAttentionState = true;
		}
		if (manager->jointAttentionEnd())
		{
			//std::cout << "joint attention stopped" << std::endl;
			OSCMessage message;
			message.text = "/context/jointattention";
			message.values.push_back(0);
			m_sender.send(message);
			jointAttentionState = false;
		}
		if (sendIntersection)
		{
			std::map<int, cv::Point3f> intCoord = manager->intersectionCoordinates();
			for (std::map<int, cv::Point3f>::iterator it = intCoord.begin(); it != intCoord.end(); ++it)
			{
				//std::cout << "send coordinates :" << std::endl;
				OSCMessage message;
				message.text = "/context/user/coordinates";
				message.values.push_back(it->first);
				message.values.push_back(it->second.z);
				message.values.push_back(it->second.x);
				message.values.push_back(it->second.y);
				m_sender.send(message);

			}
		}
		if (showRgb)
		{
			// get modified image to display
			getImage(buffer, type, numChannels, Fubi::ImageDepth::D8, options, Fubi::RenderOptions::ALL_JOINTS);
			rgbMat = cv::Mat(rgbHeight, rgbWidth, CV_8UC3, buffer);
			std::ostringstream oss;
			oss.precision(3);
			oss << "fps : " << fps;
			cv::putText(rgbMat, oss.str(), cv::Point(9, 13), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 0));
			cv::putText(rgbMat, oss.str(), cv::Point(8, 12), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 255));
			// display a green circle if joint attention
			if (jointAttentionState)
				cv::circle(rgbMat, cv::Point(rgbMat.size().width - 20, 20), 15, cv::Scalar(0, 255, 0), -1);
			else
				cv::circle(rgbMat, cv::Point(rgbMat.size().width - 20, 20), 15, cv::Scalar(0, 50, 0), -1);
			cv::imshow("rgb", rgbMat);
		}

		key = cv::waitKey(10);
	}

	// close OSC connections

}


void KISDapp::startNextSensor()
{
	Fubi::SensorType::Type type = Fubi::getCurrentSensorType();
	bool success = false;
	while (!success)
	{
		if (type == Fubi::SensorType::NONE)
			type = Fubi::SensorType::OPENNI2;
		else if (type == Fubi::SensorType::OPENNI2)
			type = Fubi::SensorType::OPENNI1;
		else if (type == Fubi::SensorType::OPENNI1)
			type = Fubi::SensorType::KINECTSDK;
		else if (type == Fubi::SensorType::KINECTSDK)
			type = Fubi::SensorType::NONE;

		if (Fubi::isInitialized())
			success = Fubi::switchSensor(Fubi::SensorOptions(Fubi::StreamOptions(), Fubi::StreamOptions(), Fubi::StreamOptions(-1, -1, -1),
									type, Fubi::SkeletonTrackingProfile::UPPER_BODY, true, false));
		else
			success = Fubi::init(Fubi::SensorOptions(Fubi::StreamOptions(), Fubi::StreamOptions(), Fubi::StreamOptions(-1, -1, -1), 
									Fubi::SensorType::KINECTSDK, Fubi::SkeletonTrackingProfile::UPPER_BODY, true, false));

		if (success)
		{

			Fubi::getRgbResolution(rgbWidth, rgbHeight);

				delete[] g_rgbData;
				if (rgbWidth > 0 && rgbHeight > 0)
					g_rgbData = new unsigned char[rgbWidth*rgbHeight * 3];
		}

		if (type == Fubi::SensorType::NONE)
			break;	// None should always be successful so we ensure termination of this loop
	}
}

void KISDapp::setDisplayOptions(DisplayOptions dispOpt)
{
	options = Fubi::RenderOptions::None;
	if (dispOpt.shapes)
		options |= Fubi::RenderOptions::Shapes;
	if (dispOpt.skeletons)
		options |= Fubi::RenderOptions::Skeletons;
	if (dispOpt.globalPos)
		options |= Fubi::RenderOptions::GlobalPosCaptions;
	if (dispOpt.fingers)
		options |= Fubi::RenderOptions::FingerShapes;
	if (dispOpt.faceDetails)
		options |= Fubi::RenderOptions::DetailedFaceShapes;
	if (dispOpt.bodyMeasurements)
		options |= Fubi::RenderOptions::BodyMeasurements;
	if (dispOpt.useFilter)
		options |= Fubi::RenderOptions::UseFilteredValues;

	options |= Fubi::RenderOptions::SwapRAndB;
}



