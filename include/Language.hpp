#pragma once

#include <string>

class Language
{
public:
	static Language& Instance();
	void Initialize();
	std::string& GetLanguageName();

private:
	Language();
	~Language();

	std::string mLanguageName;
};
