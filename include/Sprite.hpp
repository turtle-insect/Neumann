#pragma once

#include <switch.h>
#include "FrameBuffer.hpp"

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
