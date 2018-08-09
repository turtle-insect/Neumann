#include <algorithm>
#include <cstring>
#include <cstdint>
#include "Image.hpp"
#include "Input.hpp"
#include "FrameBuffer.hpp"
#include "ItemList.hpp"

ItemList::ItemList()
	: mColumn(0)
	, mRow(0)
	, mMargin(0)
	, mCursor(0)
{
	mFrame.SetMode(Rectangle::eMode::eFrame);
	mFrame.SetWeight(3);
	mFrame.SetColor(0xFF00FF00);
}

ItemList::~ItemList()
{
}

void ItemList::Config(size_t column, size_t row, size_t margin)
{
	mColumn = column;
	mRow = row;
	mMargin = margin;
}

void ItemList::Clear()
{
	mCursor = 0;
	mSprites.clear();
}

void ItemList::AppendSprite(Sprite* sprite)
{
	mSprites.push_back(sprite);
}

void ItemList::Update(Input& input)
{
	if(mSprites.size() == 0) return;

	int index = mCursor;
	if(input.KeyDown(KEY_UP)) index -= mColumn;
	if(input.KeyDown(KEY_DOWN)) index += mColumn;
	if(input.KeyDown(KEY_LEFT)) index -= 1;
	if(input.KeyDown(KEY_RIGHT)) index += 1;

	if (index < 0) index = mSprites.size() - 1;
	if (index >= (int)mSprites.size()) index = 0;
	mCursor = index;
}

size_t ItemList::GetCursor()
{
	return mCursor;
}

void ItemList::SetCursor(size_t cursor)
{
	mCursor = cursor;
}

size_t ItemList::GetCount()
{
	return mSprites.size();
}

void ItemList::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	if(mSprites.size() == 0) return;

	size_t width = mSprites[mCursor]->Width();
	size_t height = mSprites[mCursor]->Height();
	size_t x = left;
	size_t y = top;

	int index = 0;
	int offset = 0;
	if (mRow < (mSprites.size() + mColumn - 1) / mColumn)
	{
		offset = mRow / 2 - (mCursor / mColumn);
		offset = std::min(offset, 0);
		offset = std::max(offset, (int)mRow - (int)((mSprites.size() + mColumn - 1) / mColumn));
		index = (-offset) * mColumn;
	}

	for(size_t cnt = 0; cnt < mRow * mColumn && index + cnt < mSprites.size(); cnt++)
	{
		if(cnt != 0 && cnt % mColumn == 0)
		{
			x = left;
			y += height + mMargin;
		}
		mSprites[index + cnt]->Draw(frameBuffer, x, y);

		x += width + mMargin;
	}

	x = left + (mCursor % mColumn) * (width + mMargin);
	y = top + (mCursor / mColumn + offset) * (height + mMargin);
	mFrame.SetWidth(width);
	mFrame.SetHeight(height);
	mFrame.Draw(frameBuffer, x, y);
}
