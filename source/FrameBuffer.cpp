#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer()
	: mBuffer(nullptr)
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Update()
{
	mBuffer = gfxGetFramebuffer(&mWidth, &mHeight);
}

u8* FrameBuffer::GetBuffer()
{
	return mBuffer;
}

u8* FrameBuffer::GetPixel(u32 x, u32 y)
{
	if(x < 0 || x >= mWidth) return nullptr;
	if(y < 0 || y >= mHeight) return nullptr;

	return mBuffer + (y * mWidth + x) * 4;
}

u32 FrameBuffer::GetWidth()
{
	return mWidth;
}

u32 FrameBuffer::GetHeight()
{
	return mHeight;
}
