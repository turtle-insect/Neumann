#pragma once

#include <switch.h>

struct Glyph
{
	u8 width;
	u8 height;
	int8_t posX;
	int8_t posY;
	int8_t advance;
	const u8* data;
};

class Font
{
public:
	enum eType
	{
		eType14,
		eType18,
		eType20,
		eType24,
	};

	static Font& Instance();
	bool GetGlyph(Glyph& glyph, eType type, u32 code);
	u8 GetHeight(eType type);
	u8 GetBaseLine(eType type);

private:
	Font();
	~Font();
};
