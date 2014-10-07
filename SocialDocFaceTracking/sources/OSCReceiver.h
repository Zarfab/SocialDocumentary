#pragma once

#include "OSCUtils.h"

struct OSCListener
{
	int port;
	oscpkt::UdpSocket sock;
};

class OSCReceiver
{
public:
	OSCReceiver();
	~OSCReceiver();

	void init(std::vector<int> ports, bool verbose = true);
	bool messageReceived(bool verbose = true);
	OSCMessage getMessage(bool verbose = true);
	void close(bool verbose = true);

private:
	std::vector<OSCListener*> oscListeners;
};

