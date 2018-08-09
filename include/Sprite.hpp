#pragma once

#include <switch.h>

class FrameBuffer;

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

inline u8 blendColor(u32 src, u32 dst, u8 alpha) {
	u8 one_minus_alpha = static_cast<u8>(255) - alpha;
	return (dst * alpha + src * one_minus_alpha) / static_cast<u8>(255);
}
