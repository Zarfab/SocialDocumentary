#include "UserManager.h"
// OSC includes
#include "OSCSender.h"


using namespace Fubi;

UserManager::UserManager(bool display, const int & width, const int & height, const std::vector<std::string>& paths) :
m_display(display), m_width(width), m_height(height), lastTime(0), m_nbUsers(0), m_nbFaceTrackedUsers(0), m_nbFaceTrackedUsersPrev(0)
{
	// face recognizer
	//m_recognizer = new KITV::Recognizer(paths.front());

	// screens
	m_screenManager = new KITV::ScreenManager(paths.front());
	m_screenManager->printScreens();

	// window manager
	if (m_display)
		m_windowManager = WindowManager();

	// confidences for each distance
	// pair<distance(mm), confidence[0..1]>
	m_confidenceByDistance.push_back(std::pair<float, float>(800.0, 0.0));
	m_confidenceByDistance.push_back(std::pair<float, float>(1000.0, 1.0));
	m_confidenceByDistance.push_back(std::pair<float, float>(1500.0, 1.0));
	m_confidenceByDistance.push_back(std::pair<float, float>(2000.0, 0.80));
	m_confidenceByDistance.push_back(std::pair<float, float>(2500.0, 0.60));
	m_confidenceByDistance.push_back(std::pair<float, float>(3000.0, 2.0));
	m_confidenceByDistance.push_back(std::pair<float, float>(4000.0, 0.0));
	std::sort(m_confidenceByDistance.begin(), m_confidenceByDistance.end());

	// init filters for face tracking
	m_faceTrackedUsersFilterSize = 3;
	m_faceTrackedUsersFilter[1] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
	m_faceTrackedUsersFilter[2] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
	m_faceTrackedUsersFilter[3] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
	m_faceTrackedUsersFilter[4] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
	m_faceTrackedUsersFilter[5] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
	m_faceTrackedUsersFilter[6] = std::deque<bool>(m_faceTrackedUsersFilterSize, false);
}


UserManager::~UserManager()
{
}

void UserManager::update(const cv::Mat & rgb, bool resetTimers)
{
	m_attentionChanged.clear();
	m_intersectionCoordinates.clear();
	// get the currents users in scene
	std::deque<unsigned int> ids = getClosestUserIDs();
	m_users = Fubi::getClosestUsers();
	m_nbUsersPrev = m_nbUsers;
	m_nbUsers = ids.size();
	
	m_allUsersLookingSameScreenPrev = m_allUsersLookingSameScreen;

	// update users
	for (unsigned int i = 0; i < m_nbUsers; i++)
	{
		FubiUser* tempUser = Fubi::getUser(ids[i]);

		if (resetTimers)
		{
			tempUser->m_interestBegTime = getCurrentTime();
			tempUser->m_interestTime = 0.0;
		}

		if (tempUser->m_isFaceTracked)
		{
			m_faceTrackedUsersFilter[ids[i]].pop_front();
			m_faceTrackedUsersFilter[ids[i]].push_back(true);


			// update screen watched
			std::pair<int, cv::Point3f> sw = userWatchingScreen(tempUser->m_id);
			updateUserScreenWatched(tempUser->m_id, sw.first);
			if (tempUser->m_interestChanged)
				m_attentionChanged.push_back(tempUser->m_id);
			if (sw.first > 0)
				m_intersectionCoordinates[tempUser->m_id] = sw.second;

			
			//display face and other information
			if (m_display)
			{
				//get the face from the image
				cv::Mat face = rgb(getFaceRect(tempUser));

				cv::resize(face, face, cv::Size(240, 320));
				addInfoToFace(tempUser, face);
				m_windowManager.show(face, FACE, tempUser->m_id);
			}
		}
	}

	//update face tracked users
	m_faceTrackedUsers.clear();
	m_nbFaceTrackedUsersPrev = m_nbFaceTrackedUsers;
	m_nbFaceTrackedUsers = 0;
	for (std::map<unsigned short, std::deque<bool>>::iterator it = m_faceTrackedUsersFilter.begin(); it != m_faceTrackedUsersFilter.end(); ++it)
	{
		std::deque<bool> tracked = it->second;
		int trackedTimes = 0;
		for (int j = 0; j < tracked.size(); j++)
		{
			if (tracked[j])
				trackedTimes++;
		}
		if (trackedTimes >= 1)
		{
			m_nbFaceTrackedUsers++;
			m_faceTrackedUsers.push_back(it->first);
		}
	}

	// joint attention
	if (m_users.size() > 1)
	{
		m_allUsersLookingSameScreen = true;
		int screenWatched = m_users.front()->m_screenWatched;
		std::deque<FubiUser*>::iterator user = m_users.begin();
		user++;
		for (user; user != m_users.end(); ++user)
		{
			m_allUsersLookingSameScreen &= (*user)->m_screenWatched == screenWatched;
		}
	}

	else
		m_allUsersLookingSameScreen = false;
	
	if (m_display)
		m_windowManager.clearUnused(ids);
}

cv::Rect UserManager::getFaceRect(FubiUser* user)
{
	cv::Rect faceRect = user->m_faceRectCv;
	if (faceRect.x < 0) faceRect.x = 0;
	if (faceRect.y < 0) faceRect.y = 0;
	if ((faceRect.x + faceRect.width) > m_width)  faceRect.width = m_width - faceRect.x;
	if ((faceRect.y + faceRect.height) > m_height) faceRect.height = m_height - faceRect.y;
	return faceRect;
}

float UserManager::getHeadRotationConfidence(FubiUser* user)
{
	std::vector<float> faceCenter = getUserHeadCenter(user->m_id);
	Fubi::Vec3f fcenter(faceCenter[0], faceCenter[1], faceCenter[2]);
	Fubi::Vec3f origin(0, 0, 0);
	float dist = (fcenter - origin).length();
	
	// out of bounds -> confidence = 0
	if (dist < m_confidenceByDistance.front().first || dist >= m_confidenceByDistance.back().first)
		return 0.0;
	
	// else calculate confidence (linear interpolation on distance)
	for (unsigned int i = 1; i < m_confidenceByDistance.size(); i++)
	{
		if (dist >= m_confidenceByDistance[i - 1].first && dist < m_confidenceByDistance[i].first)
		{
			float a = (m_confidenceByDistance[i].second - m_confidenceByDistance[i - 1].second) /
				(m_confidenceByDistance[i].first - m_confidenceByDistance[i - 1].first);

			return m_confidenceByDistance[i - 1].second + a * (dist - m_confidenceByDistance[i - 1].first);
		}
	}
	return 0.0;
}

void  UserManager::addInfoToFace(FubiUser* user, cv::Mat& image)
{
	int org(image.size().height);
	std::vector<std::string> lines;

	std::vector<float> faceCenter = getUserHeadCenter(user->m_id);
	// convert from mm to meters
	for (unsigned int i = 0; i < faceCenter.size(); i++)
		faceCenter[i] /= 1000.0;
	std::vector<float> faceRotation = getUserHeadRotation(user->m_id);

	std::ostringstream oss;
	oss << getUserName(user->m_id) << " watches screen " << user->m_screenWatched;
	lines.push_back(oss.str());

	oss.clear();
	oss.str("");
	oss.precision(2);
	oss.setf(std::ios::fixed);
	oss << "x : " << faceCenter[0] << "  pitch : " << faceRotation[0];
	lines.push_back(oss.str());

	oss.clear();
	oss.str("");
	oss << "y : " << faceCenter[1] << "  yaw : " << faceRotation[1];
	lines.push_back(oss.str());
	
	oss.clear();
	oss.str("");
	oss << "z : " << faceCenter[2] << "  roll : " << faceRotation[2];
	lines.push_back(oss.str());

	oss.clear();
	oss.str("");
	oss << "confidence : " << getHeadRotationConfidence(user);
	lines.push_back(oss.str());

	// dist(left shoulder - right shoulder)
	Fubi::BodyMeasurementDistance shoulderWidth = user->m_bodyMeasurements[Fubi::BodyMeasurement::SHOULDER_WIDTH];
		
	if (shoulderWidth.m_confidence>0.9)
	{
		oss.clear();
		oss.str("");
		//age based on dist(left shoulder - right shoulder) en croissance age=0.08*taillemm-14.8  1.5 d'offset car distance epaule? 
		int age = 0.08*1.5*shoulderWidth.m_dist - 14.8;

		int AgeSeuil = 13;
		oss << "age estimation = " << age;
		lines.push_back(oss.str());
	}

	cv::Mat blackMat(lines.size()*12+4, image.size().width, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::vconcat(image, blackMat, image);

	for (unsigned int i = 0; i < lines.size(); i++)
		cv::putText(image, lines[i], cv::Point(8, org + (i+1)*12), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 255));

	// print a rectangle fo interest
	double time = user->m_interestTime;
	cv::Rect interestRect;
	cv::Scalar interestColor;

	interestRect.x = 20;
	interestRect.y = 10;
	interestRect.height = 20;
	if (time <= 1.5)
	{
		interestRect.width = 0;
		interestColor = cv::Scalar(255, 255, 255);
	}
	else if (time < 8)
	{
		interestRect.width = 200 * (time / 16.0);
		interestColor = cv::Scalar(0, 255, 255 * time / 8.0);
	}
	else if (time < 16)
	{
		interestRect.width = 200 * (time/16.0);
		interestColor = cv::Scalar(0, 255 - 255*(time-8.0)/8.0, 255);
	}
	else
	{
		interestRect.width = 200;
		interestColor = cv::Scalar(0, 0, 255);
	}
	
	cv::rectangle(image, interestRect, interestColor, -1);

}


std::pair<int, cv::Point3f> UserManager::userWatchingScreen(int userID)
{
	std::vector<float> faceCenter = getUserHeadCenter(userID);
	std::vector<float> faceRotation = getUserHeadRotation(userID);

	// convert from mm to meters
	for (unsigned int i = 0; i < faceCenter.size(); i++)
		faceCenter[i] /= 1000.0;

	cv::Mat mat_translation = cv::Mat(faceCenter);
	cv::Mat mat_rotation;
	cv::Rodrigues(cv::Mat(faceRotation) * CV_PI / 180, mat_rotation);

	cv::Point3f gaze1;
	gaze1.x = faceCenter[0];
	gaze1.y = faceCenter[1];
	gaze1.z = faceCenter[2];

	cv::Point3f gaze2;
	cv::Mat m = (cv::Mat_<float>(3, 1) << 0, 0, -1);
	cv::Mat mat_gaze2 = mat_rotation * m + mat_translation;
	gaze2.x = mat_gaze2.at<float>(0);
	gaze2.y = mat_gaze2.at<float>(1);
	gaze2.z = mat_gaze2.at<float>(2);

	return m_screenManager->whichScreenBeingWatched(gaze1, gaze2);
}
