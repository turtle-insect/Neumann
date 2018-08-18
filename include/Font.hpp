#pragma once

#include <switch.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph
{
	u8 width;
	u8 height;
	int8_t posX;
	int8_t posY;
	int8_t advanceX;
	int8_t pitch;
	const u8* data;
};

class Font
{
public:
	static Font& Instance();
	bool GetGlyph(Glyph& glyph, size_t size, u32 code);

private:
	Font();
	~Font();

	FT_Library mFontLibrary;
	FT_Face mFontFace;
	FT_Error mFontFaceState;
};
