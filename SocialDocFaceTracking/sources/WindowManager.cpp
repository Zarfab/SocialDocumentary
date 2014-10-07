#include "WindowManager.h"


WindowManager::WindowManager()
{
	m_userDisplayZones = std::vector<int>(4, -1);
}


WindowManager::~WindowManager()
{
}

void WindowManager::show(const cv::Mat & frame, WindowType type, int userID)
{
	Window wind(type, userID);
	
	// find the window in m_window
	int index = getWindowIndex(wind);

	if (index == -1) // the window does not exist
	{
		// get size
		wind.w = frame.size().width;
		wind.h = frame.size().height;
		
		// set title and initial values for x and y
		std::ostringstream oss;
		int x, y;
		switch (type)
		{
		case FACE:
			x = 0;
			y = 0;
			oss << "User " << userID;
			break;
		case AUS:
			x = 260;
			y = 0;
			oss << "AUs: User " << userID;
			break;
		case EMOTIONS:
			x = 260;
			y = 180;
			oss << "expressions : User " << userID;
			break;
		}
		// add title
		wind.title = oss.str();

		// calculate x and y
		int dispIndex = -1;
		for (int i = 0; i < 4; i++)
		{
			if (m_userDisplayZones[i] == userID)
				dispIndex = i;
		}
		if (dispIndex == -1)
		{
			dispIndex = nextAvailableZone();
			m_userDisplayZones[dispIndex] = userID;
		}
			

		switch (dispIndex)
		{
		case 0:
			break;
		case 1:
			y += 420;
			break;
		case 2:
			x += 680;
			break;
		case 3:
			y += 420;
			x += 680;
			break;
		default:
			break;
		}

		// move the window to the correct place
		wind.x = x;
		wind.y = y;
		cv::namedWindow(wind.title);
		std::cout << "moving window " << wind.title << " to " << wind.x << ", " << wind.y << std::endl;
		cv::moveWindow(wind.title, wind.x, wind.y);

		// store it in m_windows
		m_windows.push_back(wind);
		index = m_windows.size() - 1;
	}
	// display the window
	cv::imshow(m_windows[index].title, frame);
}

void WindowManager::clearUnused(std::deque<unsigned int> activeUsers)
{
	for (unsigned int i = 0; i < m_windows.size(); i++)
	{
		// if the windows number i in the vector has a userID different
		// from all in the activeUsers deque, destroy it and remove it from the vector
		if (std::find(activeUsers.begin(), activeUsers.end(), m_windows[i].userID) == activeUsers.end())
		{
			// free the display zone
			for (int j = 0; j < 4; j++)
			{
				if (m_userDisplayZones[j] == m_windows[i].userID)
					m_userDisplayZones[j] = -1;
			}

			// destroy the window
			cv::destroyWindow(m_windows[i].title);
			m_windows.erase(m_windows.begin() + i);
			i--; // decrement i to adjust to the new size of m_windows
		}
	}
}

int WindowManager::getWindowIndex(const Window & window)
{
	int index = -1;
	for (unsigned int i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i] == window)
			index = i;
	}
	return index;
}


int WindowManager::nextAvailableZone()
{
	int index = -1;
	for (int i = 0; i < 4; i++)
	{
		if (m_userDisplayZones[i] == -1)
			return i;
	}
	return index;
}

bool operator==(Window const& a, Window const& b)
{
	if (a.type == b.type && a.userID == b.userID)
		return true;
	else
		return false;
}
