#include <switch.h>
#include "Language.hpp"

eLanguage getLanguage()
{
	SetLanguage sl;
	u64 l;
	setGetSystemLanguage(&l);
	setMakeLanguage(l, &sl);

	eLanguage lang = eLanguage::eOther;
	switch(sl)
	{
		case SetLanguage_ENGB:
		case SetLanguage_ENUS:
			lang = eLanguage::eEnglish;
			break;

		case SetLanguage_JA:
			lang = eLanguage::eJapanese;
			break;

		default:
			break;
	}

	return lang;
}
