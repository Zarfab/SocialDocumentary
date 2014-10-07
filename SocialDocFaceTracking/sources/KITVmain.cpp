#include "KISDapp.h"
#include "commandParser.h"
#include <windows.h>


int main(int argc, char ** argv)
{
	std::vector<std::string> paths;
	std::vector<std::pair<std::string, int>> clientsIP;
	std::vector<int> ports;
	bool display = true;
	bool sendCoord = false;

	if (argc > 1)
	{
		// xml files for screens
		std::string screenFile;
		if (CommandParser::parse_argument(argc, argv, "-screens", screenFile))
				paths.push_back(screenFile);
		else
		{
			std::cout << "No path for screen definition in argument, taking default value" << std::endl;
			std::cout << "Please, use -screens option to specify this path\n" << std::endl;
		}
		// display rgb from kinect and faces
		std::string disp;
		if (CommandParser::parse_argument(argc, argv, "-display", disp))
		{
			if (disp == "on" || disp == "ON")
				display = true;
			else
				display = false;
		}
		else
		{
			std::cout << "No display option specified, rgb image will be displayed (default)" << std::endl;
			std::cout << "Please, use -display [on/off] option to enable/disable it\n" << std::endl;
		}

		
		int i = 0;
		bool ok(true);
		while (ok && i < 12)
		{
			i++;
			std::string clientIPandPort;
			std::string cmd = "-oscclient";
			cmd += std::to_string(i);
			if (CommandParser::parse_argument(argc, argv, cmd.c_str(), clientIPandPort) > 0)
			{
				OSCClient tempClient;
				int found = clientIPandPort.find(":");
				if (found != std::string::npos)
				{
					std::string address = clientIPandPort.substr(0, found);
					int port = std::stoi(clientIPandPort.substr(found + 1));
					clientsIP.push_back(std::pair<std::string, int>(address, port));
				}
				else
				{
					std::cout << "Please, use -oscclientX [IP:port] option to use them with X = [1; 2; 3...]\n" << std::endl;
				}
			}
			else
				ok = false;
		}
		if (clientsIP.empty())
		{
			std::cout << "No OSC client found" << std::endl;
			std::cout << "Please, use -oscclientX [IP:port] option to use them with X = [1; 2; 3...]\n" << std::endl;
		}
		// send gaze/screen intersection coordinates OSC messages
		std::string gaze;
		if (CommandParser::parse_argument(argc, argv, "-gaze", gaze))
		{
			if (gaze == "on" || gaze == "ON")
				sendCoord = true;
			else
				sendCoord = false;
		}
		else
		{
			std::cout << "Not sending OSC messages for user's gaze and screen intersection" << std::endl;
			std::cout << "Please, use -gaze [on/off] option to enable/disable it\n" << std::endl;
		}

		i = 0;
		ok = true;
		while (ok && i < 12)
		{
			i++;
			std::string portToListen;
			std::string cmd = "-osclistener";
			cmd += std::to_string(i);
			if (CommandParser::parse_argument(argc, argv, cmd.c_str(), portToListen) > 0)
				ports.push_back(std::stoi(portToListen));
			else
				ok = false;
		}
		if (ports.empty())
		{
			std::cout << "No OSC listener found" << std::endl;
			std::cout << "Please, use -osclistenerX [port] option to use them with X = [1; 2; 3...]\n" << std::endl;
		}
	}
	else
	{
		std::cout << "No argument, taking default values for paths and OSC\n" << std::endl;
		paths.push_back("numediartConfigRoom.xml");
	}

	try
	{
		KISDapp kisd(display);
		DisplayOptions dopt;
		dopt.skeletons = true;
		dopt.useFilter = true;
		dopt.faceDetails = true;
		dopt.fingers = true;

		kisd.init(paths, Fubi::SensorType::KINECTSDK, true, dopt, clientsIP, sendCoord, ports);
		kisd.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
			  
		Sleep(2000);
	}
	return 0;
}
