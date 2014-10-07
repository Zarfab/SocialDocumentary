#pragma once
#include <opencv2/opencv.hpp>

enum WindowType
{
	FACE,
	AUS,
	EMOTIONS
};

struct Window
{
	WindowType type;
	unsigned int userID;
	std::string title;
	int x, y;
	int w, h;

	Window(WindowType _type, int _userID): type(_type), userID(_userID){};
};

bool operator==(Window const& a, Window const& b);


class WindowManager
{
public:
	WindowManager();
	~WindowManager();
	void show(const cv::Mat & frame, WindowType type, int userID);
	void clearUnused(std::deque<unsigned int> activeUsers);

private:
	int getWindowIndex(const Window & window);
	int nextAvailableZone();

	std::vector<Window> m_windows;
	std::vector<int> m_userDisplayZones;
};

