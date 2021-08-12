#pragma once

#include <string>

class Resource
{
public:
	static Resource& Instance();
	void Initialize();
	std::string GetMessage(const std::string& messageID);

private:
	Resource();
	~Resource();
};
