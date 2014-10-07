#include "OSCSender.h"


OSCSender::OSCSender()
{
}


OSCSender::~OSCSender()
{
}


bool OSCSender::init(std::vector<std::pair<std::string, int>> clientsIP, bool verbose)
{
	bool success = !clientsIP.empty();

	for (unsigned int i = 0; i<clientsIP.size(); i++)
	{
		oscClients.push_back(new OSCClient);
		oscClients[i]->address = clientsIP[i].first;
		oscClients[i]->port = clientsIP[i].second;
		oscClients[i]->sock.connectTo(oscClients[i]->address, oscClients[i]->port);
		oscClients[i]->connected = oscClients[i]->sock.isOk();

		if (!oscClients[i]->connected)
		{
			std::cerr << "Error connection to '" << oscClients[i]->address;
			std::cerr << "' on port " << oscClients[i]->port << "\n";
		}
		else
		{
			if (verbose)
				std::cout << "Client started, will send packets to '" << oscClients[i]->address << "' on port " << oscClients[i]->port << std::endl;
		}
		success &= oscClients[i]->connected;
	}
	return success;
}


void OSCSender::send(OSCMessage mes, bool verbose)
{
	for (unsigned int i = 0; i < oscClients.size(); i++)
	{
		if (oscClients[i]->connected)
		{
			oscpkt::PacketWriter pw;
			oscpkt::Message msg;

			if (verbose)
			{
				std::cout << "send message to " << oscClients[i]->address << ":" << oscClients[i]->port << std::endl;
				std::cout << mes.text;
				for (unsigned int j = 0; j<mes.values.size(); j++)
					std::cout << " " << mes.values[j];
				std::cout << std::endl;
			}
			msg.init(mes.text);

			for (unsigned int j = 0; j<mes.values.size(); j++)
				msg.pushFloat(mes.values[j]);

			pw.startBundle();
			pw.addMessage(msg);
			oscClients[i]->sock.sendPacket(pw.packetData(), pw.packetSize());
			pw.endBundle();
			msg.clear();
		}
	}
}


void OSCSender::close(bool verbose)
{
	for (unsigned int i = 0; i < oscClients.size(); i++)
	{
		if (oscClients[i]->connected)
		{
			oscClients[i]->sock.close();
			if (verbose)
				std::cout << "close connection with " << oscClients[i]->address << ":" << oscClients[i]->port << std::endl;
		}
		delete oscClients[i];
	}
}