#ifndef SCREENNODEKITV_H
#define SCREENNODEKITV_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"

namespace KITV
{
	
class ScreenManager {
public:

	ScreenManager(std::string xmlFile) {
		loadConfigXML(xmlFile);
    }

    void printScreens() {
		if (m_screenNodes.empty())
		{
			std::cout << "No screen loaded" << std::endl;
			return;
		}
		
		for (unsigned int i = 0; i < m_screenNodes.size(); i++)
			std::cout << "Screen " << m_screenNodes[i].id 
				<< ": size: " << m_screenNodes[i].width 
				<< ", " << m_screenNodes[i].height 
				<< " reso " << m_screenNodes[i].resX 
				<< ", " << m_screenNodes[i].resY << std::endl;
    }

	std::pair<int, cv::Point3f> whichScreenBeingWatched(cv::Point3f gaze1, cv::Point3f gaze2)
	{
		for (unsigned int i = 0; i < m_screenNodes.size(); i++)
		{
			std::pair<bool, cv::Point3f> res = isScreenBeingWatched(i, gaze1, gaze2);
			if (res.first)
				return std::pair<int, cv::Point3f>(m_screenNodes[i].id, res.second);
		}
		return std::pair<int, cv::Point3f>(0, cv::Point3f());
	}

private:
	
	struct ScreenNode
	{
		int id;
		float centerx;
		float centery;
		float centerz;
		float width;
		float height;
		float extended_limit_factor;
		bool isWatched;
		std::string output;
		int resX;
		int resY;
		float interpixX;
		float interpixY;
		float angleX;
		float angleY;
		float angleZ;
	};

	std::vector<ScreenNode> m_screenNodes;

	bool loadConfigXML(std::string s) {

		tinyxml2::XMLDocument doc;
		int i = doc.LoadFile(s.c_str());
		if (i != 0)
			return false;
		else {
			tinyxml2::XMLElement* screen = doc.FirstChildElement("RoomConfig")->FirstChildElement("Screen");
			while (screen != NULL) {
				ScreenNode tempNode;
				tempNode.id = (int)(atoi(screen->FirstChildElement("id")->GetText()));
				tempNode.width = (float)atof(screen->FirstChildElement("width")->GetText());
				tempNode.height = (float)atof(screen->FirstChildElement("height")->GetText());
				tempNode.resX = (int)atoi(screen->FirstChildElement("resolutionX")->GetText());
				tempNode.resY = (int)atoi(screen->FirstChildElement("resolutionY")->GetText());

				tinyxml2::XMLElement* camPos = screen->FirstChildElement("CamPosition");
				if (camPos != NULL)
				{
					tempNode.centerx = (float)atof(camPos->FirstChildElement("centerx")->GetText());
					tempNode.centery = (float)atof(camPos->FirstChildElement("centery")->GetText());
					tempNode.centerz = (float)atof(camPos->FirstChildElement("centerz")->GetText());
					tempNode.angleX = (float)atof(camPos->FirstChildElement("angleX")->GetText());
					tempNode.angleY = (float)atof(camPos->FirstChildElement("angleY")->GetText());
					tempNode.angleZ = (float)atof(camPos->FirstChildElement("angleZ")->GetText());
				}
				m_screenNodes.push_back(tempNode);
				screen = screen->NextSiblingElement("Screen");
			}

			return true;
		}
	}

	cv::Point3f pixIntersection(unsigned int screenIndex, cv::Point3f p) {
		cv::Point3f r;
		r.z = 0;
		r.x = (-p.x + m_screenNodes[screenIndex].width / 2) / m_screenNodes[screenIndex].width * m_screenNodes[screenIndex].resX;
		r.y = (p.y) / m_screenNodes[screenIndex].height * m_screenNodes[screenIndex].resY;
		return r;

	}

	cv::Point3f linePlaneIntersection(unsigned int screenIndex, cv::Point3f p4, cv::Point3f p5)
	{
		cv::Point3f p1, p2, p3;

		p1.x = m_screenNodes[screenIndex].centerx - m_screenNodes[screenIndex].width / 2;
		p1.y = -m_screenNodes[screenIndex].centery - m_screenNodes[screenIndex].height / 2;
		p1.z = m_screenNodes[screenIndex].centerz;

		p2.x = m_screenNodes[screenIndex].centerx - m_screenNodes[screenIndex].width / 2;
		p2.y = -m_screenNodes[screenIndex].centery + m_screenNodes[screenIndex].height / 2;
		p2.z = m_screenNodes[screenIndex].centerz;

		p3.x = m_screenNodes[screenIndex].centerx + m_screenNodes[screenIndex].width / 2;
		p3.y = -m_screenNodes[screenIndex].centery + m_screenNodes[screenIndex].height / 2;
		p3.z = m_screenNodes[screenIndex].centerz;

		cv::Mat mat_t_nom = cv::Mat::eye(4, 4, CV_32F);
		cv::Mat mat_t_denom = cv::Mat::eye(4, 4, CV_32F);

		mat_t_nom.at<float>(0, 0) = 1;
		mat_t_nom.at<float>(0, 1) = 1;
		mat_t_nom.at<float>(0, 2) = 1;
		mat_t_nom.at<float>(0, 3) = 1;
		mat_t_nom.at<float>(1, 0) = p1.x;
		mat_t_nom.at<float>(1, 1) = p2.x;
		mat_t_nom.at<float>(1, 2) = p3.x;
		mat_t_nom.at<float>(1, 3) = p4.x;
		mat_t_nom.at<float>(2, 0) = p1.y;
		mat_t_nom.at<float>(2, 1) = p2.y;
		mat_t_nom.at<float>(2, 2) = p3.y;
		mat_t_nom.at<float>(2, 3) = p4.y;
		mat_t_nom.at<float>(3, 0) = p1.z;
		mat_t_nom.at<float>(3, 1) = p2.z;
		mat_t_nom.at<float>(3, 2) = p3.z;
		mat_t_nom.at<float>(3, 3) = p4.z;

		mat_t_denom.at<float>(0, 0) = 1;
		mat_t_denom.at<float>(0, 1) = 1;
		mat_t_denom.at<float>(0, 2) = 1;
		mat_t_denom.at<float>(0, 3) = 0;
		mat_t_denom.at<float>(1, 0) = p1.x;
		mat_t_denom.at<float>(1, 1) = p2.x;
		mat_t_denom.at<float>(1, 2) = p3.x;
		mat_t_denom.at<float>(1, 3) = p5.x - p4.x;
		mat_t_denom.at<float>(2, 0) = p1.y;
		mat_t_denom.at<float>(2, 1) = p2.y;
		mat_t_denom.at<float>(2, 2) = p3.y;
		mat_t_denom.at<float>(2, 3) = p5.y - p4.y;
		mat_t_denom.at<float>(3, 0) = p1.z;
		mat_t_denom.at<float>(3, 1) = p2.z;
		mat_t_denom.at<float>(3, 2) = p3.z;
		mat_t_denom.at<float>(3, 3) = p5.z - p4.z;

		float t = -((float)cv::determinant(mat_t_nom) / (float)cv::determinant(mat_t_denom));

		cv::Point3f inter;

		inter.x = p4.x + (p5.x - p4.x) * t;
		inter.y = p4.y + (p5.y - p4.y) * t;
		inter.z = p4.z + (p5.z - p4.z) * t;

		return inter;
	}

	std::pair<bool, cv::Point3f> isScreenBeingWatched(unsigned int screenIndex, cv::Point3f gaze1, cv::Point3f gaze2)
	{

		cv::Point3f inter = linePlaneIntersection(screenIndex, gaze1, gaze2);
		//        std::cout<<resX<<" "<<resY<<std::endl;
		//        std::cout<<"            "<<inter.x<<","<<inter.y<<std::endl;
		//        std::cout<<"                    delta"<<fabs((inter.x-(centerx - width / 2))/width)<<","<<fabs((inter.y- (-centery - height / 2))/height)<<std::endl;
		//        
		//        if (inter.x > centerx - width / 2 && inter.x < centerx + width / 2) {
		//            if (inter.y>-centery - height / 2 && inter.y<-centery + height / 2) {
		if (inter.x > m_screenNodes[screenIndex].centerx - m_screenNodes[screenIndex].width / 2 
			&& inter.x < m_screenNodes[screenIndex].centerx + m_screenNodes[screenIndex].width / 2 
			&& inter.y>-m_screenNodes[screenIndex].centery - m_screenNodes[screenIndex].height / 2 
			&& inter.y<-m_screenNodes[screenIndex].centery + m_screenNodes[screenIndex].height / 2)
		{
			//                std::cout << "LOOKING IN THE TV: " << id << std::endl;
			m_screenNodes[screenIndex].isWatched = true;
			m_screenNodes[screenIndex].interpixX = fabs((inter.x - (m_screenNodes[screenIndex].centerx - (float)m_screenNodes[screenIndex].width / 2)) / (float)m_screenNodes[screenIndex].width);
			m_screenNodes[screenIndex].interpixY = fabs((inter.y - (-m_screenNodes[screenIndex].centery - (float)m_screenNodes[screenIndex].height / 2)) / (float)m_screenNodes[screenIndex].height);

			//            std::cout<<interpixX<<";"<<interpixY<<std::endl; 

			//std::cout<<(inter.x-(centerx-width/2))*resX/width<<" "<<((-centery + height / 2)-inter.y)*resY/height<<std::endl;
			//                interpixX=(int) floor( fabs(inter.x -(centerx+width/2)) *resX/width );
			//interpixX=(int)floor(resY-((-centery + height / 2)-inter.y)*resY/height);

			//                std::cout<<" **************** "<<-inter.x +(centerx+width/2)<<std::endl;


		}
		else {
			//            std::cout << "NOT" << std::endl;
			m_screenNodes[screenIndex].isWatched = false;
			m_screenNodes[screenIndex].interpixX = 0;
			m_screenNodes[screenIndex].interpixY = 0;
		}
		inter.z = (float)screenIndex+1;
		return std::pair<bool, cv::Point3f>(m_screenNodes[screenIndex].isWatched, inter);
	}
};

}
#endif
