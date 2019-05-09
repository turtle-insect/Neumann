#include <map>
#include <switch.h>
#include "Language.hpp"
#include "MessageID.hpp"
#include "Resource.hpp"

typedef std::map<int, std::string> MessegeContainer;
MessegeContainer messageList;

Resource::Resource()
{
}

Resource::~Resource()
{
}

Resource& Resource::Instance()
{
	static Resource instance;
	return instance;
}

void Resource::Initialize()
{
	Language& lang = Language::Instance();
	lang.Initialize();

	std::string name = lang.GetLanguageName();

	if(1)
	{
		messageList.insert(MessegeContainer::value_type(MSGID_TITLESCENE_TITLE, "Title : "));
		messageList.insert(MessegeContainer::value_type(MSGID_TITLESCENE_DESCRIPTION, "Select title and account by pressing "));
		messageList.insert(MessegeContainer::value_type(MSGID_ACCOUNTSCENE_TITLE, "Account : "));
		messageList.insert(MessegeContainer::value_type(MSGID_ACTIONSCENE_TITLE, "Action"));
		messageList.insert(MessegeContainer::value_type(MSGID_CONFIRMSCENE_TITLE, "Confirm"));
		messageList.insert(MessegeContainer::value_type(MSGID_CONFIRMSCENE_DESCRIPTION, "If you Resotre pressing "));
		messageList.insert(MessegeContainer::value_type(MSGID_DEBUGSCENE_TITLE, "Debug"));
		messageList.insert(MessegeContainer::value_type(MSGID_SUCCESS, "Success"));
		messageList.insert(MessegeContainer::value_type(MSGID_FAIL, "Fail"));
		messageList.insert(MessegeContainer::value_type(MSGID_BACKUP, "Backup"));
		messageList.insert(MessegeContainer::value_type(MSGID_RESOTRE, "Resotre"));
		messageList.insert(MessegeContainer::value_type(MSGID_REMOVE, "Save Remove"));
		messageList.insert(MessegeContainer::value_type(MSGID_BACK, "Back"));
	}
}

std::string Resource::GetMessage(int messageID)
{
	MessegeContainer::iterator ite = messageList.find(messageID);
	if(ite == messageList.end()) return "";

	return (*ite).second;
}
