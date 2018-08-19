#pragma once

#include <switch.h>
#include "Sprite.hpp"

class Image : public Sprite
{
public:
	Image(u8* jpeg, size_t size);
	~Image();

	void Resize(size_t width, size_t height);
	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);

private:
	u8* mBuffer;
};
