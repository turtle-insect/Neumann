#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "SaveData.hpp"

const std::string SAVE_DEV = "save";
const std::string ROOT_DIR = "/Neumann/";

bool MountDeviceSaveData(FsFileSystem& fs, u64 titleID, u128 userID)
{
	Result rc;
	rc = fsMount_SaveData(&fs, titleID, userID);
	if(R_FAILED(rc))
	{
		fsdevUnmountDevice(SAVE_DEV.c_str());
		return false;
	}

	int ret = fsdevMountDevice(SAVE_DEV.c_str(), fs);
	if(ret == -1)
	{
		fsdevUnmountDevice(SAVE_DEV.c_str());
		return false;
	}

	return true;
}

std::string getBackupTitlePath(u64 titleID)
{
	std::stringstream ss;
	ss << ROOT_DIR << std::uppercase << std::setfill('0') << std::setw(sizeof(titleID)*2)
		<< std::hex << titleID << "/";
	return ss.str();
}

std::string makeBackupPath(u64 titleID)
{
	mkdir(ROOT_DIR.c_str(), 0700);

	std::stringstream ss;
	ss << getBackupTitlePath(titleID);

	mkdir(ss.str().c_str(), 0700);

	u64 time = 0;
	timeGetCurrentTime(TimeType_UserSystemClock, &time);
	time_t t = static_cast<time_t>(time);
	ss << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S") << "/";
	mkdir(ss.str().c_str(), 0700);

	return ss.str();
}

bool isDirectory(const std::string& path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0) return false;

	return S_ISDIR(statbuf.st_mode);
}

bool copyFile(const std::string& srcFile, const std::string& dstFile)
{
	FILE* src = fopen(srcFile.c_str(), "rb");
	FILE* dst = fopen(dstFile.c_str(), "wb+");

	if (src == nullptr || dst == nullptr)
	{
		fclose(src);
		fclose(dst);
		return false;
	}

	fseek(src, 0, SEEK_END);
	size_t size = ftell(src);
	rewind(src);

	u64 offset = 0;
	char* buf = new char[0x50000];
	while(offset < size)
	{
		size_t read = fread(buf, 1, 0x50000, src);
		fwrite(buf, 1, read, dst);
		offset += read;
	}

	delete [] buf;
	fclose(src);
	fclose(dst);

	return true;
}

void copyPath(const std::string& srcPath, const std::string& dstPath, bool isWriteNAND)
{
	DIR* dir = opendir(srcPath.c_str());
	if(dir == nullptr) return;

	struct dirent* ent;
	while((ent = readdir(dir)))
	{
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

		std::string srcFile = srcPath + "/" + ent->d_name;
		std::string dstFile = dstPath + "/" + ent->d_name;

		if(isDirectory(srcFile))
		{
			mkdir(dstFile.c_str(), 0700);
			if(isWriteNAND) fsdevCommitDevice(SAVE_DEV.c_str());
			copyPath(srcFile, dstFile, isWriteNAND);
		}
		else
		{
			copyFile(srcFile, dstFile);
			if(isWriteNAND) fsdevCommitDevice(SAVE_DEV.c_str());
		}
	}
	closedir(dir);
}

void deletePath(const std::string& srcPath, bool isWriteNAND)
{
	DIR* dir = opendir(srcPath.c_str());
	if(dir == nullptr) return;

	struct dirent* ent;
	while((ent = readdir(dir)))
	{
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

		std::string srcFile = srcPath + "/" + ent->d_name;
		if(isDirectory(srcFile))
		{
			deletePath(srcFile, isWriteNAND);
			if(isWriteNAND) fsdevCommitDevice(SAVE_DEV.c_str());
		}
		else
		{
			unlink(srcFile.c_str());
			if(isWriteNAND) fsdevCommitDevice(SAVE_DEV.c_str());
		}
	}
	closedir(dir);

	rmdir(srcPath.c_str());
	if(isWriteNAND) fsdevCommitDevice(SAVE_DEV.c_str());
}

SaveData::SaveData(u64 titleID, u128 userID)
	: mTitleID(titleID)
	, mUserID(userID)
{
}

SaveData::~SaveData()
{
}

bool SaveData::Backup()
{
	FsFileSystem fs;
	if(! MountDeviceSaveData(fs, mTitleID, mUserID)) return false;
	std::string dstPath = makeBackupPath(mTitleID);
	copyPath(SAVE_DEV + ":/", dstPath, false);
	fsdevUnmountDevice(SAVE_DEV.c_str());
	return true;
}

bool SaveData::Delete()
{
	FsFileSystem fs;
	if (!MountDeviceSaveData(fs, mTitleID, mUserID)) return false;
	deletePath(SAVE_DEV + ":/", true);
	fsdevUnmountDevice(SAVE_DEV.c_str());
	return true;
}

void SaveData::GetBackupPaths(std::vector<std::string>& paths)
{
	std::string titlePath = getBackupTitlePath(mTitleID);
	DIR *dir = opendir(titlePath.c_str());
	if(dir == nullptr) return;

	struct dirent* ent;
	while((ent = readdir(dir)))
	{
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
		std::string file = titlePath + "/" + ent->d_name;
		if(isDirectory(file)) paths.push_back(ent->d_name);
	}
	closedir(dir);
}

bool SaveData::Restore(std::string& filePath)
{
	FsFileSystem fs;
	if(! MountDeviceSaveData(fs, mTitleID, mUserID)) return false;
	deletePath(SAVE_DEV + ":/", true);
	std::string titlePath = getBackupTitlePath(mTitleID);
	copyPath(titlePath + "/" + filePath, SAVE_DEV + ":/", true);
	fsdevUnmountDevice(SAVE_DEV.c_str());
	return true;
}