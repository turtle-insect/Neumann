#include <algorithm>
#include <cstring>
#include <cstdint>
#include "Util.hpp"
#include "Image.hpp"
#include "Input.hpp"
#include "FrameBuffer.hpp"
#include "ItemList.hpp"

const size_t CursorWeight = 5;


ItemList::ItemList()
	: mColumn(0)
	, mRow(0)
	, mMaxHeight(0)
	, mMargin(0)
	, mCursor(0)
	, mCursorColor(CURSOR_COLOR)
{
	mFrame.SetMode(Rectangle::eMode::eFrame);
	mFrame.SetWeight(CursorWeight);
	mFrame.SetColor(mCursorColor);
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
	mMaxHeight = 0;
	mSprites.clear();
}

void ItemList::AppendSprite(Sprite* sprite)
{
	mSprites.push_back(sprite);
	size_t height = sprite->Height();
	if(mMaxHeight < height) mMaxHeight = height;
}

void ItemList::Update(Input& input)
{
	if(mSprites.size() == 0) return;

	int index = mCursor;
	if(input.KeyDown(HidNpadButton_AnyUp)) index -= mColumn;
	if(input.KeyDown(HidNpadButton_AnyDown)) index += mColumn;
	if(input.KeyDown(HidNpadButton_AnyLeft)) index -= 1;
	if(input.KeyDown(HidNpadButton_AnyRight)) index += 1;

	if (index < 0) index = mSprites.size() - 1;
	if (index >= (int)mSprites.size()) index = 0;
	mCursor = index;

	mCursorColor += 0x02000000;
	mFrame.SetColor(mCursorColor);
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
			y += mMaxHeight + mMargin;
		}
		mSprites[index + cnt]->Draw(frameBuffer, x, y);

		x += width + mMargin;
	}

	x = left + (mCursor % mColumn) * (width + mMargin);
	y = top + (mCursor / mColumn + offset) * (mMaxHeight + mMargin);
	mFrame.SetWidth(width + CursorWeight * 2);
	mFrame.SetHeight(mMaxHeight + CursorWeight * 2);
	mFrame.Draw(frameBuffer, x - CursorWeight, y - CursorWeight);
}
