#include "Debug.hpp"

std::vector<std::string> gDebugLog;

void Debug::AppendLog(std::string message)
{
	gDebugLog.push_back(message);
}

std::vector<std::string>& Debug::GetLogs()
{
	return gDebugLog;
}
