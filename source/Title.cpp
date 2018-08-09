#include <cstring>
#include "Image.hpp"
#include "Title.hpp"

Title::Title(u64 titleID)
	: mTitleID(titleID)
	, mTitleIcon(nullptr)
{
	NsApplicationControlData data = { 0 };
	size_t outsize = 0;
	Result rc = nsGetApplicationControlData(1, titleID, &data, sizeof(data), &outsize);
	if (R_FAILED(rc)) return;
	if (outsize < sizeof(data.nacp))return;

	NacpLanguageEntry* entry = nullptr;
	rc = nacpGetLanguageEntry(&data.nacp, &entry);
	if (R_FAILED(rc)) return;
	if (entry == nullptr) return;

	mTitleName = std::string(entry->name);
	mTitleAuthor = std::string(entry->author);
	mTitleVersion = std::string(data.nacp.version);

	mTitleIcon = new Image(data.icon, outsize - sizeof(data.nacp));
}

Title::~Title()
{
	delete mTitleIcon;
}

u64 Title::GetID()
{
	return mTitleID;
}

std::string Title::GetName()
{
	return mTitleName;
}

std::string Title::GetAuthor()
{
	return mTitleAuthor;
}

std::string Title::GetVersion()
{
	return mTitleVersion;
}

Image* Title::GetImage() const
{
	return mTitleIcon;
}
