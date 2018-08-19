#include "Util.hpp"
#include "Text.hpp"
#include "Rectangle.hpp"
#include "Toast.hpp"

static const uint MAX_TIMER = 300;

Toast::Toast()
	: mStart(false)
	, mTimer(0)
	, mText(nullptr)
{
	mBody.SetColor(COLOR_WHITE);
	mFrame.SetColor(COLOR_BLACK);
	mFrame.SetMode(Rectangle::eMode::eFrame);
	mFrame.SetWeight(8);
}

Toast::~Toast()
{
	delete mText;
}

void Toast::Popup(std::string message, u32 color)
{
	mTimer = 0;
	mStart = true;
	delete mText;
	mText = new Text(message, 7, color);
	mWidth = mText->Width() + 30;
	mHeight = mText->Height() + 30;

	mBody.SetWidth(mWidth);
	mBody.SetHeight(mHeight);
	mFrame.SetWidth(mWidth);
	mFrame.SetHeight(mHeight);
}

bool Toast::Update()
{
	if(!mStart) return false;

	mTimer++;
	if(mTimer >= MAX_TIMER)
	{
		mStart = false;
		mTimer = 0;
		delete mText;
		mText = nullptr;
		return true;
	}

	return false;
}

void Toast::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	if(!mStart) return;
	if(mText == nullptr) return;

	mBody.Draw(frameBuffer, left, top);
	mFrame.Draw(frameBuffer, left, top);
	mText->Draw(frameBuffer, left + 15, top + 15);
}
