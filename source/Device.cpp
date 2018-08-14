#include <cstdio>
#include <set>
#include <switch.h>

#include "Title.hpp"
#include "Account.hpp"
#include "Device.hpp"

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

	FsSaveDataIterator iterator;
	Result rc = fsOpenSaveDataIterator(&iterator, FsSaveDataSpaceId_NandUser);
	if (R_FAILED(rc)) return;

	size_t totalCount = 0;
	FsSaveDataInfo info;
	rc = fsSaveDataIteratorRead(&iterator, &info, 1, &totalCount);

	std::set<u64> titles;
	std::map<u128, Account*> accounts;
	for (; R_SUCCEEDED(rc) && totalCount > 0; rc = fsSaveDataIteratorRead(&iterator, &info, 1, &totalCount))
	{
		if (info.SaveDataType != FsSaveDataType_SaveData) continue;

		u64 titleID = info.titleID;
		if (titles.find(titleID) == titles.end())
		{
			titles.insert(titleID);
			mTitles.push_back(new Title(titleID));
			mTitle2Accounts.insert(std::map< u64, std::vector<Account*> >::value_type(titleID, std::vector<Account*>()));
		}

		u128 userID = info.userID;
		std::map<u128, Account*>::iterator ite = accounts.find(userID);
		Account* account = nullptr;
		if (ite == accounts.end())
		{
			account = new Account(userID);
			mAccounts.push_back(account);
			accounts.insert(std::map<u128, Account*>::value_type(userID, account));
		}
		else
		{
			account = ite->second;
		}
		mTitle2Accounts.find(titleID)->second.push_back(account);
	}
	fsSaveDataIteratorClose(&iterator);
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
