#include <cstring>
#include "FrameBuffer.hpp"
#include "Text.hpp"

inline u32 decodeUTF8(const char** ptr)
{
	u32 r, cnt, i;
	char ch;

	cnt = 1;
	ch = **ptr;

	if ((ch & 0xE0) == 0xE0) cnt = 3;
	else if ((ch & 0xC0) == 0xC0) cnt = 2;

	for (i = r = 0; i < cnt; i++) {
		ch = **ptr;
		(*ptr)++;
		r = ch + (r << 8);
	}
	return r;
}

Text::Text(Font::eType type, std::string text, u32 color)
	: mColor(color)
	, mAlpha(nullptr)
{
	mText = text;
	Font& font = Font::Instance();
	Glyph glyph;

	// Dimensions
	mHeight = font.GetHeight(type);
	const char* ch = text.c_str();
	while (*ch)
	{
		u32 code = decodeUTF8(&ch);
		if (!font.GetGlyph(glyph, type, code))
			if (!font.GetGlyph(glyph, type, '?'))
				continue;

		mWidth += glyph.advance;
	}

	mAlpha = new u8[mWidth * mHeight];
	memset(mAlpha, 0, mWidth * mHeight);
	size_t width = 0;
	size_t baseline = font.GetBaseLine(type);
	ch = text.c_str();
	while (*ch)
	{
		u32 code = decodeUTF8(&ch);
		if (!font.GetGlyph(glyph, type, code))
			if (!font.GetGlyph(glyph, type, '?'))
				continue;
		
		const u8* data = glyph.data;
		for (size_t y = 0; y < glyph.height; y++)
		{
			for (size_t x = 0; x < glyph.width; x++)
			{
				mAlpha[(y + baseline + glyph.posY) * mWidth + x + glyph.posX + width] = *data;
				data++;
			}
		}
		width += glyph.advance;
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
