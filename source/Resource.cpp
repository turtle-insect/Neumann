#include <vector>
#include <cstring>
#include <switch.h>
#include "Language.hpp"
#include "MessageID.hpp"
#include "Resource.hpp"

struct stKeyValue
{
	std::string key;
	std::string value;
};

typedef std::vector<stKeyValue*> MessageContainer;

MessageContainer messageList;

void appendMessage(std::string path)
{
	FILE* f = fopen(path.c_str(), "r");
	if(f == nullptr) return;

	char oneline[1000] = {};
	for(; fgets(oneline, sizeof(oneline), f) != nullptr;)
	{
		if(oneline[0] == '#') continue;

		size_t len = strlen(oneline);
		if(len <= 2) continue;

		if(oneline[len - 2] == '\r') oneline[len - 2] = 0;
		else if(oneline[len - 1] == '\n') oneline[len - 1] = 0;

		char* index = strchr(oneline, '\t');
		if(index == nullptr) continue;

		*index = 0;
		std::string key(oneline);
		std::string value(index + 1);

		std::string msg = Resource::Instance().GetMessage(key);
		if(msg == "")
		{
			stKeyValue* item = new stKeyValue;
			item->key = key;
			item->value = value;
			messageList.push_back(item);
		}
	}
	fclose(f);
}

Resource::Resource()
{
}

Resource::~Resource()
{
	for(size_t i = 0; i < messageList.size(); i++)
	{
		delete messageList[i];
	}
}

Resource& Resource::Instance()
{
	static Resource instance;
	return instance;
}

void Resource::Initialize()
{
	eLanguage lang = getLanguage();

	switch(lang)
	{
		case eJapanese:
			appendMessage("romfs:/lang/jp.txt");
			break;

		default:
			break;
	}

	appendMessage("romfs:/lang/en.txt");
}

std::string Resource::GetMessage(const std::string& messageID)
{
	for(size_t i = 0; i < messageList.size(); i++)
	{
		if(messageID == messageList[i]->key) return messageList[i]->value;
	}

	return "";
}
