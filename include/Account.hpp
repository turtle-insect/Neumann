#pragma once

#include <string>
#include <switch.h>

class Image;

class Account
{
public:
	Account(u128 userID);
	~Account();

	u128 GetID();
	std::string GetName();
	Image* GetImage() const;

private:
	u128 mUserID;
	std::string mUserName;
	Image* mProfileImage;
};
