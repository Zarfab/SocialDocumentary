#pragma once

// OSC includes
#include "OSCSender.h"
#include "OSCReceiver.h"

#include <Fubi\Fubi.h>
#include <Fubi\FubiUtils.h>

#include <iostream>
#include <string>
#include <sstream>
#include <queue>

#include <opencv2\opencv.hpp>


#include "UserManager.h"

struct DisplayOptions
{
	bool shapes;
	bool skeletons;
	bool globalPos;
	bool fingers;
	bool faceDetails;
	bool bodyMeasurements;
	bool useFilter;
	DisplayOptions():
		shapes(false), 
		skeletons(false), 
		globalPos(false), 
		fingers(false), 
		faceDetails(false), 
		bodyMeasurements(false), 
		useFilter(false)
		{};
};

class KISDapp //: public QWidget
{
public:
	KISDapp(bool display = true);
	~KISDapp();
	void init(const std::vector<std::string>& paths,
		Fubi::SensorType::Type sensorType,
		bool seatedSkel,
		DisplayOptions dispOpt = DisplayOptions(),
		std::vector<std::pair<std::string, int>> clientsIP = std::vector<std::pair<std::string, int>>(),
		bool sendGazeCoord = false,
		std::vector<int> ports = std::vector<int>(),
		const Fubi::FilterOptions & filter = Fubi::FilterOptions());
	void run();
	void startNextSensor();

	UserManager* manager;


private:
	void setDisplayOptions(DisplayOptions dispOpt);

	OSCSender m_sender;
	OSCReceiver m_receiver;


	int rgbWidth = 0, rgbHeight = 0;

	// buffers and Mat
	unsigned char* g_rgbData = 0x0;
	cv::Mat rgbMat;
	bool showRgb;
	bool sendIntersection;

	bool jointAttentionState;
	unsigned int options;
};

