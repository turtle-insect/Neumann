#include "Device.hpp"
#include "Input.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Bin.hpp"
#include "FrameBuffer.hpp"
#include "Title.hpp"
#include "Account.hpp"
#include "SaveData.hpp"
#include "Debug.hpp"
#include "Scene.hpp"

const u32 FONT_BLACK = 0xFF000000;
const u32 FONT_WHITE = 0xFFFFFFFF;
const u32 FONT_RED = 0xFF0000FF;

IScene* BootScene::Update(Input& input)
{
	return new TitleScene();
}

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Entry()
{
	mTitleList.Config(5, 2, 0);
	Device& device = Device::Instance();
	std::vector<Title*>& titles = device.GetTitles();
	for (size_t i = 0; i < titles.size(); i++)
	{
		Sprite* sprite = titles[i]->GetImage();
		if (sprite == nullptr) continue;
		mTitleList.AppendSprite(sprite);
	}

	mTitleDecorate.SetColor(0xFF53A834);
	mTitleDecorate.SetWidth(1280);
	mTitleDecorate.SetHeight(64);

	mGuide.AppendSprite(new Text(Font::eType24, "Select title and account by pressing ", FONT_BLACK), 0);
	mGuide.AppendSprite(new Bin(Bin::eType::eA), 0);
}

IScene* TitleScene::Update(Input& input)
{
	mTitleList.Update(input);
	if (mTitleList.GetCount())
	{
		mTitle.Clear();
		Device& device = Device::Instance();
		std::vector<Title*>& titles = device.GetTitles();
		mTitle.AppendSprite(new Text(Font::eType24, "Title : " + titles[mTitleList.GetCursor()]->GetName(), FONT_WHITE), 0);
	}

	if (input.KeyDown(KEY_A) && mTitleList.GetCount())
	{
		Device& device = Device::Instance();
		std::vector<Title*>& titles = device.GetTitles();
		return new AccountScene(titles[mTitleList.GetCursor()]->GetID());
	}
	else if (input.KeyDown(KEY_X))
	{
		return new DebugScene();
	}
	return this;
}

void TitleScene::Draw(FrameBuffer& frameBuffer)
{
	mTitleDecorate.Draw(frameBuffer, 0, 0);
	mTitle.Draw(frameBuffer, (frameBuffer.GetWidth() - mTitle.Width()) / 2, (mTitleDecorate.Height() - mTitle.Height()) / 2);
	mTitleList.Draw(frameBuffer, 0, mTitleDecorate.Height());
	mGuide.Draw(frameBuffer, (frameBuffer.GetWidth() - mGuide.Width()) / 2, mTitleDecorate.Height() + 512 + 64);
}

AccountScene::AccountScene(u64 titleID)
	: mTitleID(titleID)
{
}

AccountScene::~AccountScene()
{
}

void AccountScene::Entry()
{
	mTitleList.Config(5, 2, 0);
	mAccountList.Config(8, 1, 28);

	Device& device = Device::Instance();
	std::vector<Title*>& titles = device.GetTitles();
	for (size_t i = 0; i < titles.size(); i++)
	{
		Sprite* sprite = titles[i]->GetImage();
		if (sprite == nullptr) continue;
		mTitleList.AppendSprite(sprite);
		if (mTitleID == titles[i]->GetID()) mTitleList.SetCursor(i);
	}

	std::vector<Account*>& accounts = device.GetAccounts(mTitleID);
	for (size_t i = 0; i < accounts.size(); i++)
	{
		Sprite* sprite = accounts[i]->GetImage();
		if (sprite == nullptr) continue;
		mAccountList.AppendSprite(sprite);
	}

	mTitleDecorate.SetColor(0xFF05BCFB);
	mTitleDecorate.SetWidth(1280);
	mTitleDecorate.SetHeight(64);
}

IScene* AccountScene::Update(Input& input)
{
	mAccountList.Update(input);
	if (mAccountList.GetCount())
	{
		mTitle.Clear();
		Device& device = Device::Instance();
		std::vector<Account*>& Accounts = device.GetAccounts(mTitleID);
		mTitle.AppendSprite(new Text(Font::eType24, "Account : " + Accounts[mTitleList.GetCursor()]->GetName(), FONT_WHITE), 0);
	}

	if (input.KeyDown(KEY_A) && mAccountList.GetCount())
	{
		Device& device = Device::Instance();
		std::vector<Account*>& accounts = device.GetAccounts(mTitleID);
		return new SaveEditorScene(mTitleID, accounts[mAccountList.GetCursor()]->GetID());
	}
	else if (input.KeyDown(KEY_B))
	{
		return new TitleScene();
	}
	return this;
}

void AccountScene::Draw(FrameBuffer& frameBuffer)
{
	mTitleDecorate.Draw(frameBuffer, 0, 0);
	mTitle.Draw(frameBuffer, (frameBuffer.GetWidth() - mTitle.Width()) / 2, (mTitleDecorate.Height() - mTitle.Height()) / 2);
	mTitleList.Draw(frameBuffer, 0, mTitleDecorate.Height());
	mAccountList.Draw(frameBuffer, 30, 550);
}

SaveEditorScene::SaveEditorScene(u64 titleID, u128 userID)
	: mTitleID(titleID)
	, mUserID(userID)
{
}

SaveEditorScene::~SaveEditorScene()
{
	for (size_t i = 0; i < mTextList.size(); i++)
	{
		delete mTextList[i];
	}
}

void SaveEditorScene::Entry()
{
	mBackupList.Config(1, 10, 5);
	CreateBackupList();

	mTitleDecorate.SetColor(0xFFF48542);
	mTitleDecorate.SetWidth(1280);
	mTitleDecorate.SetHeight(64);

	mGuide.AppendSprite(new Bin(Bin::eType::eX), 10);
	mGuide.AppendSprite(new Text(Font::eType24, "Backup", FONT_BLACK), 20);
	mGuide.AppendSprite(new Bin(Bin::eType::eY), 10);
	mGuide.AppendSprite(new Text(Font::eType24, "Resotre", FONT_BLACK), 20);
	mGuide.AppendSprite(new Bin(Bin::eType::eMinus), 10);
	mGuide.AppendSprite(new Text(Font::eType24, "Save Remove", FONT_RED), 20);
	mGuide.AppendSprite(new Bin(Bin::eType::eB), 10);
	mGuide.AppendSprite(new Text(Font::eType24, "Back", FONT_BLACK), 20);
}

IScene* SaveEditorScene::Update(Input& input)
{
	mBackupList.Update(input);
	if (input.KeyDown(KEY_Y) && mBackupList.GetCount())
	{
		// Resotre
		SaveData save(mTitleID, mUserID);
		save.Restore(mTextList[mBackupList.GetCursor()]->GetText());
	}
	else if (input.KeyDown(KEY_X))
	{
		// Backup
		SaveData save(mTitleID, mUserID);
		save.Backup();
		CreateBackupList();
	}
	else if (input.KeyDown(KEY_MINUS))
	{
		// Save Remove
		SaveData save(mTitleID, mUserID);
		save.Delete();
		CreateBackupList();
	}
	else if (input.KeyDown(KEY_B))
	{
		return new AccountScene(mTitleID);
	}
	return this;
}

void SaveEditorScene::Draw(FrameBuffer& frameBuffer)
{
	mTitleDecorate.Draw(frameBuffer, 0, 0);
	mBackupList.Draw(frameBuffer, 20, mTitleDecorate.Height() + 20);
	mGuide.Draw(frameBuffer, frameBuffer.GetWidth() - mGuide.Width() - 10, frameBuffer.GetHeight() - mGuide.Height() - 10);
}

void SaveEditorScene::CreateBackupList()
{
	for (size_t i = 0; i < mTextList.size(); i++)
	{
		delete mTextList[i];
	}
	mTextList.clear();
	mBackupList.Clear();

	SaveData save(mTitleID, mUserID);
	std::vector<std::string> paths;
	save.GetBackupPaths(paths);
	for (size_t i = 0; i < paths.size(); i++)
	{
		Text* text = new Text(Font::eType24, paths[i], FONT_BLACK);
		mTextList.push_back(text);
		mBackupList.AppendSprite(text);
	}
}

DebugScene::DebugScene()
{

}

DebugScene::~DebugScene()
{
	for (size_t i = 0; i < mTextList.size(); i++)
	{
		delete mTextList[i];
	}
}

void DebugScene::Entry()
{
	mLogList.Config(1, 10, 5);
	Debug debug;
	std::vector<std::string> logs = debug.GetLogs();
	for (size_t i = 0; i < logs.size(); i++)
	{
		Text* text = new Text(Font::eType24, logs[i], FONT_BLACK);
		mTextList.push_back(text);
		mLogList.AppendSprite(text);
	}

	mTitleDecorate.SetColor(0xFF3543EA);
	mTitleDecorate.SetWidth(1280);
	mTitleDecorate.SetHeight(64);

	mTitle.AppendSprite(new Text(Font::eType24, "Debug", FONT_WHITE), 0);
}

IScene* DebugScene::Update(Input& input)
{
	mLogList.Update(input);
	if (input.KeyDown(KEY_A))
	{
		// Any Debug Programming.
	}
	else if (input.KeyDown(KEY_B))
	{
		return new BootScene();
	}
	return this;
}

void DebugScene::Draw(FrameBuffer& frameBuffer)
{
	mTitleDecorate.Draw(frameBuffer, 0, 0);
	mTitle.Draw(frameBuffer, (frameBuffer.GetWidth() - mTitle.Width()) / 2, (mTitleDecorate.Height() - mTitle.Height()) / 2);
	mLogList.Draw(frameBuffer, 10, 100);
}
