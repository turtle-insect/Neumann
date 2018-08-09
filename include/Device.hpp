#pragma once

#include <vector>
#include <map>
#include <switch.h>

class Title;
class Account;

class Device
{
public:
	static Device& Instance();
	void Initialize();
	std::vector<Title*>& GetTitles();
	std::vector<Account*>& GetAccounts(u64 titleID);

private:
	Device();
	~Device();
	void Clear();

	std::vector<Title*> mTitles;
	std::vector<Account*> mAccounts;
	std::map< u64, std::vector<Account*> > mTitle2Accounts;
};
