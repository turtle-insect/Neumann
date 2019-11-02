#include <cstdio>
#include <set>
#include <switch.h>
#include "Title.hpp"
#include "Account.hpp"
#include "Device.hpp"

static u128 AccountUid2u128(AccountUid userID)
{
	return ((u128)userID.uid[0] << 64) + userID.uid[1];
}

Device::Device()
{
}

Device::~Device()
{
	Clear();
}

Device& Device::Instance()
{
	static Device instance;
	return instance;
}

void Device::Initialize()
{
	Clear();

	FsSaveDataInfoReader iterator;
	Result rc = fsOpenSaveDataInfoReader(&iterator, FsSaveDataSpaceId_NandUser);
	if (R_FAILED(rc)) return;

	size_t totalCount = 0;
	FsSaveDataInfo info;
	rc = fsSaveDataInfoReaderRead(&iterator, &info, 1, &totalCount);

	std::set<u64> titles;
	std::map<u128, Account*> accounts;
	for (; R_SUCCEEDED(rc) && totalCount > 0; rc = fsSaveDataInfoReaderRead(&iterator, &info, 1, &totalCount))
	{
		if (info.saveDataType != FsSaveDataType_SaveData) continue;

		u64 titleID = info.application_id;
		if (titles.find(titleID) == titles.end())
		{
			titles.insert(titleID);
			mTitles.push_back(new Title(titleID));
			mTitle2Accounts.insert(std::map< u64, std::vector<Account*> >::value_type(titleID, std::vector<Account*>()));
		}

		AccountUid userID = info.uid;
		std::map<u128, Account*>::iterator ite = accounts.find(AccountUid2u128(userID));
		Account* account = nullptr;
		if (ite == accounts.end())
		{
			account = new Account(userID);
			mAccounts.push_back(account);
			accounts.insert(std::map<u128, Account*>::value_type(AccountUid2u128(userID), account));
		}
		else
		{
			account = ite->second;
		}
		mTitle2Accounts.find(titleID)->second.push_back(account);
	}
	fsSaveDataInfoReaderClose(&iterator);
}

std::vector<Title*>& Device::GetTitles()
{
	return mTitles;
}

std::vector<Account*>& Device::GetAccounts(u64 titleID)
{
	return mTitle2Accounts[titleID];
}

void Device::Clear()
{
	for (size_t i = 0; i < mTitles.size(); i++)
	{
		delete mTitles[i];
	}
	for (size_t i = 0; i < mAccounts.size(); i++)
	{
		delete mAccounts[i];
	}

	mTitles.clear();
	mAccounts.clear();
	mTitle2Accounts.clear();
}
