#pragma once

#include <string>

class Base64
{
public:
	std::string Encode(const std::string& strData);
	std::string Decode(const std::string& strData);
};