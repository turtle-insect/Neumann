#include "Image.hpp"
#include "Account.hpp"

Account::Account(AccountUid userID)
	: mUserID(userID)
	, mProfileImage(nullptr)
{
	AccountProfile profile;
	AccountUserData userData;
	AccountProfileBase profileBase;
	u32 profileImageSize;

	accountGetProfile(&profile, &userID);
	accountProfileGet(&profile, &userData, &profileBase);
	accountProfileGetImageSize(&profile, &profileImageSize);

	mUserName = std::string(profileBase.nickname);

	u8* buffer = new u8[profileImageSize];
	u32 imageSize = 0;

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

AccountUid Account::GetID()
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
