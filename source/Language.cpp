#include <switch.h>
#include "Language.hpp"

Language::Language()
{
	mLanguageName = "";
}

Language::~Language()
{
}

Language& Language::Instance()
{
	static Language instance;
	return instance;
}

void Language::Initialize()
{
	NsApplicationControlData data = {0};
	NacpLanguageEntry *langentry = nullptr;

	Result rc = nacpGetLanguageEntry(&data.nacp, &langentry);
	if (R_FAILED(rc) || langentry == nullptr) return;

	mLanguageName = langentry->name;
}

std::string& Language::GetLanguageName()
{
	return mLanguageName;
}
