#include <map>
#include <switch.h>
#include "Language.hpp"
#include "MessageID.hpp"
#include "Resource.hpp"

typedef std::map<int, std::string> MessageContainer;
MessageContainer messageList;

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
		messageList.insert(MessageContainer::value_type(MSGID_TITLESCENE_TITLE, "Title : "));
		messageList.insert(MessageContainer::value_type(MSGID_TITLESCENE_DESCRIPTION, "Select title and account by pressing "));
		messageList.insert(MessageContainer::value_type(MSGID_ACCOUNTSCENE_TITLE, "Account : "));
		messageList.insert(MessageContainer::value_type(MSGID_ACTIONSCENE_TITLE, "Action"));
		messageList.insert(MessageContainer::value_type(MSGID_CONFIRMSCENE_TITLE, "Confirm"));
		messageList.insert(MessageContainer::value_type(MSGID_CONFIRMSCENE_DESCRIPTION, "If you Restore pressing "));
		messageList.insert(MessageContainer::value_type(MSGID_DEBUGSCENE_TITLE, "Debug"));
		messageList.insert(MessageContainer::value_type(MSGID_SUCCESS, "Success"));
		messageList.insert(MessageContainer::value_type(MSGID_FAIL, "Fail"));
		messageList.insert(MessageContainer::value_type(MSGID_BACKUP, "Backup"));
		messageList.insert(MessageContainer::value_type(MSGID_RESTORE, "Restore"));
		messageList.insert(MessageContainer::value_type(MSGID_REMOVE, "Save Remove"));
		messageList.insert(MessageContainer::value_type(MSGID_BACK, "Back"));
	}
}

std::string Resource::GetMessage(int messageID)
{
	MessageContainer::iterator ite = messageList.find(messageID);
	if(ite == messageList.end()) return "";

	return (*ite).second;
}
