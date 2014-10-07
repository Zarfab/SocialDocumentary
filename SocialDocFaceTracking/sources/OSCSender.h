#pragma once
#include <string>
#include "OSCUtils.h"

struct OSCClient
{
	std::string address;
	int port;
	oscpkt::UdpSocket sock;
	bool connected;
};

class OSCSender
{
public:
	OSCSender();
	~OSCSender();
	bool init(std::vector<std::pair<std::string, int>> clientsIP, bool verbose = true);
	void send(OSCMessage mes, bool verbose = true);
	void close(bool verbose = true);

private:
	std::vector<OSCClient*> oscClients;
};

