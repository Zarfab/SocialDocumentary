#pragma once

#include <vector>
#include <opencv2\opencv.hpp>

#include <Fubi\Fubi.h>
#include <Fubi\FubiUser.h>

#include "screen.h"

#include "WindowManager.h"

class UserManager
{
public:
	UserManager(bool display, const int & width, const int & height, const std::vector<std::string>& paths);
	~UserManager();
	
	bool nbUsersHasChanged() { return m_nbUsers != m_nbUsersPrev; };
	std::vector<unsigned short> attentionChanged() { return m_attentionChanged; };
	std::map<int, cv::Point3f> intersectionCoordinates() { return m_intersectionCoordinates; };
	void update(const cv::Mat & rgb, bool resetTimers = false);
	unsigned short getNbUsers() { return m_nbUsers; };
	std::pair<int, cv::Point3f> userWatchingScreen(int userID);
	bool jointAttentionStart() { return m_allUsersLookingSameScreen && !m_allUsersLookingSameScreenPrev; };
	bool jointAttentionEnd() { return !m_allUsersLookingSameScreen && m_allUsersLookingSameScreenPrev; };
	std::vector<unsigned short> getFaceTrackedUsers() {return m_faceTrackedUsers;};
	bool nbFaceTrackedUsersChanged() { return m_nbFaceTrackedUsers != m_nbFaceTrackedUsersPrev; };

	std::deque<FubiUser*> m_users;

private:
	cv::Rect getFaceRect(FubiUser* user);
	float getHeadRotationConfidence(FubiUser* user);
	void addInfoToFace(FubiUser* user, cv::Mat& image);
	

	bool m_allUsersLookingSameScreen;
	bool m_allUsersLookingSameScreenPrev;
	unsigned short m_nbUsers;
	unsigned short m_nbUsersPrev;
	int m_nbFaceTrackedUsers;
	int m_nbFaceTrackedUsersPrev;
	cv::Mat m_rgbImg;
	int m_width, m_height;
	bool m_display;
	double lastTime;


	std::vector<unsigned short> m_attentionChanged;
	std::vector<unsigned short> m_faceTrackedUsers;
	std::map<unsigned short, std::deque<bool>> m_faceTrackedUsersFilter;
	unsigned int m_faceTrackedUsersFilterSize;
	std::map<int, cv::Point3f> m_intersectionCoordinates;

	KITV::ScreenManager* m_screenManager;

	WindowManager m_windowManager;


	std::vector<std::pair<float, float> > m_confidenceByDistance;
};

