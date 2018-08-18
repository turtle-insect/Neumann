#pragma once

#include <vector>
#include <switch.h>
#include "Rectangle.hpp"

class Sprite;
class Input;
class FrameBuffer;

class ItemList
{
public:
	ItemList();
	~ItemList();

	void Config(size_t column, size_t row, size_t margin);
	void Clear();
	void AppendSprite(Sprite* sprite);
	void Update(Input& input);
	size_t GetCursor();
	void SetCursor(size_t cursor);
	size_t GetCount();
	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);

private:
	size_t mColumn;
	size_t mRow;
	size_t mMaxHeight;
	size_t mMargin;
	size_t mCursor;
	u32 mCursorColor;
	std::vector<Sprite*> mSprites;
	Rectangle mFrame;
};
