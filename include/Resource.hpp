#pragma once

#include <string>

class Resource
{
public:
	static Resource& Instance();
	void Initialize();
	std::string GetMessage(int messageID);

private:
	Resource();
	~Resource();
};
