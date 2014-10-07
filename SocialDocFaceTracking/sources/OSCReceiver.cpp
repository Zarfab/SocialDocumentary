#include "OSCReceiver.h"

OSCReceiver::OSCReceiver()
{
}


OSCReceiver::~OSCReceiver()
{
}


void OSCReceiver::init(std::vector<int> ports, bool verbose)
{
	for (unsigned int i = 0; i < ports.size(); i++)
	{
		oscListeners.push_back(new OSCListener);
		oscListeners[i]->port = ports[i];
		oscListeners[i]->sock.bindTo(oscListeners[i]->port);
		if (!oscListeners[i]->sock.isOk())
		{
			std::cerr << "Error opening port " << oscListeners[i]->port << ": " << oscListeners[i]->sock.errorMessage() << std::endl;
		}
		else
		{
			if (verbose)
				std::cout << "Server started, will listen to packets on port " << oscListeners[i]->port << std::endl;
		}
	}
}


bool OSCReceiver::messageReceived(bool verbose)
{
	bool newMessage = false;
	for (unsigned int i = 0; i < oscListeners.size(); i++)
	{
		bool temp = false;
		if (oscListeners[i]->sock.isOk())
		{
			temp = oscListeners[i]->sock.receiveNextPacket(10 /* timeout, in ms */);
			if (temp && verbose)
				std::cout << "message received on port " << oscListeners[i]->port << std::endl;
		}
		newMessage |= temp;
	}
	return newMessage;
}


OSCMessage OSCReceiver::getMessage(bool verbose)
{
	OSCMessage mr;
	for (unsigned int i = 0; i < oscListeners.size(); i++)
	{
		if (oscListeners[i]->sock.isOk())
		{
			oscpkt::PacketReader pr;
			pr.init(oscListeners[i]->sock.packetData(), oscListeners[i]->sock.packetSize());
			
			oscpkt::Message *msg;
			if (pr.isOk() && (msg = pr.popMessage()) != 0)
			{	
				mr.text = msg->addressPattern();
				mr.values.clear();

				oscpkt::Message::ArgReader argRead(msg->arg());
				if (argRead.isOk())
				{
					while (!argRead.isOkNoMoreArgs())
					{
						float farg;
						argRead.popFloat(farg);
						mr.values.push_back(farg);
					}
				}
				if (verbose)
				{
					std::cout << "message received from " << oscListeners[i]->sock.packetOrigin() << std::endl;
					std::cout << mr.text;
					for (unsigned int k = 0; k < mr.values.size(); k++)
						std::cout << " " << mr.values[k];
					std::cout << std::endl;
				}
			}

		}
	}
	return mr;
}


void OSCReceiver::close(bool verbose)
{
	for (unsigned int i = 0; i < oscListeners.size(); i++)
	{
		if (oscListeners[i]->sock.isOk())
			oscListeners[i]->sock.close();
		delete oscListeners[i];
	}
}
