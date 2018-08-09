#include <cstring>
#include "FrameBuffer.hpp"
extern "C" {
	#include "nanojpeg.h"
}
#include "Image.hpp"

inline u8 getPixel(u8* in, size_t width, size_t height, u16 x, u16 y, s32 channel)
{
    if (x < width && y < height)
        return in[(x * 3 * width) + (3 * y) + channel];

    return 0;
}

Image::Image(u8* jpeg, size_t size)
	: mBuffer(nullptr)
{
	njInit();
	njDecode(jpeg, size);

	size_t imageSize = njGetImageSize();
	mWidth = njGetWidth();
	mHeight = njGetHeight();
	mBuffer = new u8[imageSize];
	memcpy(mBuffer, njGetImage(), imageSize);
	njDone();
}

Image::~Image()
{
	delete [] mBuffer;
}

void Image::Resize(size_t width, size_t height)
{
	u8* out = new u8[width * height * 3];

	const float tx = static_cast<float>(mWidth) / width;
	const float ty = static_cast<float>(mHeight) / height;
	const s32 channels = 3;
	const size_t row_stride = width * channels;

	u8 C[5] = { 0 };

	for (u32 i = 0; i < height; ++i)
	{
		for (u32 j = 0; j < width; ++j)
		{
			const s32 x = static_cast<u32>(tx * j);
			const s32 y = static_cast<u32>(ty * i);
			const float dx = tx * j - x;
			const float dy = ty * i - y;

			for (s32 k = 0; k < 3; ++k)
			{
				for (s32 jj = 0; jj < 4; ++jj)
				{
					const s32 z = y - 1 + jj;

					u8 a0 = getPixel(mBuffer, mWidth, mHeight, z, x + 0, k);
					u8 d0 = getPixel(mBuffer, mWidth, mHeight, z, x - 1, k) - a0;
					u8 d2 = getPixel(mBuffer, mWidth, mHeight, z, x + 1, k) - a0;
					u8 d3 = getPixel(mBuffer, mWidth, mHeight, z, x + 2, k) - a0;
					u8 a1 = -1.0 / 3 * d0 + d2 - 1.0 / 6 * d3;
					u8 a2 = 1.0 / 2 * d0 + 1.0 / 2 * d2;
					u8 a3 = -1.0 / 6 * d0 - 1.0 / 2 * d2 + 1.0 / 6 * d3;
					C[jj] = a0 + a1 * dx + a2 * dx * dx + a3 * dx * dx * dx;

					d0 = C[0] - C[1];
					d2 = C[2] - C[1];
					d3 = C[3] - C[1];
					a0 = C[1];
					a1 = -1.0 / 3 * d0 + d2 -1.0 / 6 * d3;
					a2 = 1.0 / 2 * d0 + 1.0 / 2 * d2;
					a3 = -1.0 / 6 * d0 - 1.0 / 2 * d2 + 1.0 / 6 * d3;
					out[i * row_stride + j * channels + k] = a0 + a1 * dy + a2 * dy * dy + a3 * dy * dy * dy;
				}
			}
		}
	}
	
	delete [] mBuffer;
	mBuffer = out;
	mWidth = width;
	mHeight = height;
}

void Image::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	for (size_t y = 0; y < mHeight; y++)
	{
		size_t dy = top + y;
		if (dy >= frameBuffer.GetHeight()) return;
		if (dy < 0)continue;

		u8* buffer = frameBuffer.GetPixel(left, dy);
		for (size_t x = 0; x < mWidth; x++)
		{
			size_t dx = left + x;
			if (dx < 0)continue;
			if (dx >= frameBuffer.GetWidth())break;

			u8* source = mBuffer + (y * mWidth + x) * 3;
			for (size_t i = 0; i < 3; i++)*buffer++ = *source++;
			*buffer++ = 0xFF;
		}
	}
}
