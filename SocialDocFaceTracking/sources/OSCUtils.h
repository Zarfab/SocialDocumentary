#pragma once

#include <oscpkt\oscpkt.hh>
#include <oscpkt\udp.hh>

#include <string>
#include <vector>
#include <iostream>

struct OSCMessage
{
	std::string text;
	std::vector<float> values;
};
