#pragma once

#include <vector>
#include <string>
#include <switch.h>

class SaveData
{
public:
	SaveData(u64 titleID, AccountUid userID);
	~SaveData();

	bool Backup();
	bool Delete();
	void GetBackupPaths(std::vector<std::string>& paths);
	bool Restore(std::string& filePath);
private:
	u64 mTitleID;
	AccountUid mUserID;
};
