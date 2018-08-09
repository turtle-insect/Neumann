#include "Image.hpp"
#include "Account.hpp"

Account::Account(u128 userID)
	: mUserID(userID)
	, mProfileImage(nullptr)
{
	AccountProfile profile;
	AccountUserData userData;
	AccountProfileBase profileBase;
	size_t profileImageSize;

	accountGetProfile(&profile, userID);
	accountProfileGet(&profile, &userData, &profileBase);
	accountProfileGetImageSize(&profile, &profileImageSize);

	mUserName = std::string(profileBase.username, 0x20);

	u8* buffer = new u8[profileImageSize];
	size_t imageSize = 0;

	accountProfileLoadImage(&profile, buffer, profileImageSize, &imageSize);
	mProfileImage = new Image(buffer, imageSize);
	mProfileImage->Resize(128, 128);

	delete [] buffer;

	accountProfileClose(&profile);
}

Account::~Account()
{
	delete mProfileImage;
}

u128 Account::GetID()
{
	return mUserID;
}

std::string Account::GetName()
{
  return mUserName;
}

Image* Account::GetImage() const
{
  return mProfileImage;
}
