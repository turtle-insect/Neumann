#pragma once

#include <vector>
#include <string>

class Debug
{
public:
	void AppendLog(std::string log);
	std::vector<std::string>& GetLogs();
};
