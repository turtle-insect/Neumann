#pragma once

#include <string>
#include <switch.h>

class Image;

class Title
{
public:
	Title(u64 titleID);
	~Title();

	u64 GetID();
	std::string GetName();
	std::string GetAuthor();
	std::string GetVersion();
	Image* GetImage() const;

private:
	u64 mTitleID;
	std::string mTitleName;
	std::string mTitleAuthor;
	std::string mTitleVersion;
	Image* mTitleIcon;
};
