#pragma once

#include <vector>

#include "ItemList.hpp"
#include "Rectangle.hpp"
#include "HorizontalSprites.hpp"
#include "IScene.hpp"

class Input;
class FrameBuffer;
class ItemList;
class Text;

class BootScene : public IScene
{
public:
	BootScene() {}
	~BootScene() {}

	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer) {}
};

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

	void Entry();
	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer);

private:
	ItemList mTitleList;
	Rectangle mTitleDecorate;
	HorizontalSprites mTitle;
	HorizontalSprites mGuide;
};

class AccountScene : public IScene
{
public:
	AccountScene(u64 titleID);
	~AccountScene();

	void Entry();
	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer);

private:
	u64 mTitleID;
	ItemList mTitleList;
	ItemList mAccountList;
	Rectangle mTitleDecorate;
	HorizontalSprites mTitle;
};

class ActionScene : public IScene
{
public:
	ActionScene(u64 titleID, u128 userID);
	~ActionScene();

	void Entry();
	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer);

private:
	void CreateBackupList();

	u64 mTitleID;
	u128 mUserID;
	ItemList mBackupList;
	Rectangle mTitleDecorate;
	HorizontalSprites mTitle;
	HorizontalSprites mGuide;
	std::vector<Text*> mTextList;
};

class ConfirmScene : public IScene
{
public:
	ConfirmScene(u64 titleID, u128 userID, std::string filename);
	~ConfirmScene();

	void Entry();
	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer);

private:
	u64 mTitleID;
	u128 mUserID;
	std::string mFileName;

	Rectangle mTitleDecorate;
	HorizontalSprites mTitle;
	HorizontalSprites mGuide;
};

class DebugScene : public IScene
{
public:
	DebugScene();
	~DebugScene();
	void Entry();
	IScene* Update(Input& input);
	void Draw(FrameBuffer& frameBuffer);

private:
	ItemList mLogList;
	Rectangle mTitleDecorate;
	HorizontalSprites mTitle;
	std::vector<Text*> mTextList;
};