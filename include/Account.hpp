#pragma once

#include <string>
#include <switch.h>

class Image;

class Account
{
public:
	Account(AccountUid userID);
	~Account();

	AccountUid GetID();
	std::string GetName();
	Image* GetImage() const;

private:
	AccountUid mUserID;
	std::string mUserName;
	Image* mProfileImage;
};
