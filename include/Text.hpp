#pragma once

#include <string>
#include <switch.h>
#include "Font.hpp"
#include "Sprite.hpp"

class Text : public Sprite
{
public:
	Text(const std::string& text, size_t size, u32 color);
	~Text();

	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);
	std::string& GetText();

private:
	u32 mColor;
	u8* mAlpha;
	std::string mText;
};
