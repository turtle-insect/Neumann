#include <cstring>
#include "Util.hpp"
#include "FrameBuffer.hpp"
#include "Text.hpp"

Text::Text(std::string text, size_t size, u32 color)
	: mColor(color)
	, mAlpha(nullptr)
{
	mText = text;
	Font& font = Font::Instance();
	Glyph glyph;

	// Dimensions
	const uint8_t* str = (const uint8_t*)text.c_str();
	int upper = 0;
	int lower = 0;
	for(size_t i = 0; i < text.length();)
	{
		u32 code = 0;
		ssize_t count = decode_utf8(&code, &str[i]);
		if(count <= 0) break;
		i += count;

		if (!font.GetGlyph(glyph, size, code))
			if (!font.GetGlyph(glyph, size, '?'))
				continue;

		mWidth += glyph.advanceX;
		if(upper < glyph.posY) upper = glyph.posY;
		int tmp = (int)glyph.height - (int)glyph.posY;
		if(lower < tmp) lower = tmp;
	}

	mHeight = upper + lower;
	mAlpha = new u8[mWidth * mHeight];
	memset(mAlpha, 0, mWidth * mHeight);
	size_t width = 0;
	str = (const uint8_t*)text.c_str();
	for(size_t i = 0; i < text.length();)
	{
		u32 code = 0;
		ssize_t count = decode_utf8(&code, &str[i]);
		if(count <= 0) break;
		i += count;

		if (!font.GetGlyph(glyph, size, code))
			if (!font.GetGlyph(glyph, size, '?'))
				continue;

		const u8* data = glyph.data;
		for (size_t y = 0; y < glyph.height; y++)
		{
			for (size_t x = 0; x < glyph.width; x++)
			{
				int index = (y + upper - glyph.posY) * mWidth + x + width + glyph.posX;
				if(index >= 0 && index < (int)(mWidth * mHeight)) mAlpha[index] = data[x];
			}
			data += glyph.pitch;
		}
		width += glyph.advanceX;
	}
}

Text::~Text()
{
	delete [] mAlpha;
}

void Text::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	for (size_t y = 0; y < mHeight; y++)
	{
		size_t dy = top + y;
		for (size_t x = 0; x < mWidth; x++)
		{
			u8 alpha = mAlpha[y * mWidth + x];
			if (!alpha) continue;

			size_t dx = left + x;
			DrawPixel(frameBuffer, dx, dy, mColor, alpha);
		}
	}
}

std::string& Text::GetText()
{
	return mText;
}
