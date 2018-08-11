#pragma once

#include <FrameBuffer.hpp>
#include <switch.h>

class Sprite
{
public:
	Sprite() : mWidth(0), mHeight(0) {}
	virtual ~Sprite() {}

	size_t Width() {return mWidth;}
	size_t Height() { return mHeight; }

	virtual void Draw(FrameBuffer& frameBuffer, size_t left, size_t top) = 0;

protected:
	size_t mWidth;
	size_t mHeight;
};

inline u8 blendColor(u32 src, u32 dst, u8 alpha)
{
	u8 one_minus_alpha = static_cast<u8>(255) - alpha;
	return (dst * alpha + src * one_minus_alpha) / static_cast<u8>(255);
}

inline void DrawPixel(FrameBuffer& frameBuffer, size_t x, size_t y, u32 color, u8 alpha)
{
	u8* buffer = frameBuffer.GetPixel(x, y);
	if (buffer == nullptr) return;
	*buffer = blendColor(*buffer, color & 0xFF, alpha); buffer++;
	*buffer = blendColor(*buffer, (color >> 8) & 0xFF, alpha); buffer++;
	*buffer = blendColor(*buffer, (color >> 16) & 0xFF, alpha); buffer++;
	*buffer = 0xFF;
}
