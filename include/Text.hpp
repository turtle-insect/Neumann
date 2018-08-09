#pragma once

#include <string>
#include <switch.h>
#include "Font.hpp"
#include "Sprite.hpp"

class Text : public Sprite
{
public:
	Text(Font::eType type, std::string text, u32 color);
	~Text();

	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);
	std::string& GetText();

private:
	u32 mColor;
	u8* mAlpha;
	std::string mText;
};
