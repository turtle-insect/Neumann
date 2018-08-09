#include "FrameBuffer.hpp"
#include "Bin.hpp"

extern const u8 A_dark_bin[];
extern const u8 B_dark_bin[];
extern const u8 X_dark_bin[];
extern const u8 Y_dark_bin[];
extern const u8 minus_dark_bin[];

struct
{
	Bin::eType type;
	u8 width;
	u8 height;
	const u8* buffer;
} static sBinInfo[] =
{
	{ Bin::eType::eA, 25, 25, A_dark_bin,},
	{ Bin::eType::eB, 25, 25, B_dark_bin,},
	{ Bin::eType::eX, 25, 25, X_dark_bin,},
	{ Bin::eType::eY, 25, 25, Y_dark_bin,},
	{ Bin::eType::eMinus, 25, 25, minus_dark_bin, },
};

Bin::Bin(eType type)
	: mBuffer(nullptr)
{
 	for (size_t i = 0; i < sizeof(sBinInfo) / sizeof(sBinInfo[0]); i++)
	{
		if (type == sBinInfo[i].type)
		{
			mWidth = sBinInfo[i].width;
			mHeight = sBinInfo[i].height;
			mBuffer = sBinInfo[i].buffer;
			break;
		}
	}
}

Bin::~Bin()
{
	// don't delete [] mBuffer
} 

void Bin::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	for (size_t y = 0; y < mHeight; y++)
	{
		size_t dy = top + y;
		if (dy >= frameBuffer.GetHeight()) return;
		if (dy < 0)continue;

		for (size_t x = 0; x < mWidth; x++)
		{
			size_t dx = left + x;
			if (dx < 0)continue;
			if (dx >= frameBuffer.GetWidth())break;

			const u8* pixel = mBuffer + (y * mWidth + x) * 4;
			if (*pixel == 0) continue;
			u8* buffer = frameBuffer.GetPixel(dx, dy);

			*buffer = blendColor(*buffer, pixel[3], *pixel); buffer++;
			*buffer = blendColor(*buffer, pixel[2], *pixel); buffer++;
			*buffer = blendColor(*buffer, pixel[1], *pixel); buffer++;
			*buffer = 0xFF;
		}
	}
}
