#pragma once

#include <switch.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Update();
	u8* GetBuffer();
	u8* GetPixel(u32 x, u32 y);
	u32 GetWidth();
	u32 GetHeight();

private:
	u8* mBuffer;
	u32 mWidth;
	u32 mHeight;
};
