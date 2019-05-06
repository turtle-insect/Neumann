#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer()
	: mBuffer(nullptr)
	, mWidth(1280)
	, mHeight(720)
{
}

FrameBuffer::~FrameBuffer()
{
	framebufferClose(&mFrameBuffer);
	nwindowClose(mWindow);
}

void FrameBuffer::Init()
{
	mWindow = nwindowGetDefault();
	nwindowSetDimensions(mWindow, mWidth, mHeight);
	framebufferCreate(&mFrameBuffer, mWindow, mWidth, mHeight, PIXEL_FORMAT_RGBA_8888, 2);
	 framebufferMakeLinear(&mFrameBuffer);
}

void FrameBuffer::Begin()
{
	mBuffer = (u8*)framebufferBegin(&mFrameBuffer, nullptr);
}

void FrameBuffer::End()
{
	framebufferEnd(&mFrameBuffer);
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
