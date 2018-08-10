#include "FrameBuffer.hpp"
#include "HorizontalSprites.hpp"

HorizontalSprites::HorizontalSprites()
{

}

HorizontalSprites::~HorizontalSprites()
{
	Clear();
}

void HorizontalSprites::Clear()
{
	for (size_t i = 0; i < mSprites.size(); i++)
	{
		delete mSprites[i];
	}
	mSprites.clear();
	mMargin.clear();
	mWidth = 0;
	mHeight = 0;
}

void HorizontalSprites::AppendSprite(Sprite* sprite, size_t margin)
{
	mSprites.push_back(sprite);
	mMargin.push_back(margin);

	mWidth += sprite->Width() + margin;
	if (mHeight < sprite->Height())mHeight = sprite->Height();
}

void HorizontalSprites::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	for (size_t i = 0; i < mSprites.size(); i++)
	{
		size_t y = (mHeight - mSprites[i]->Height()) / 2;
		mSprites[i]->Draw(frameBuffer, left, top + y);
		left += mSprites[i]->Width() + mMargin[i];
	}
}
