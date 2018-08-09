#include "FrameBuffer.hpp"
#include "Rectangle.hpp"

Rectangle::Rectangle()
	: mColor(0xFF000000)
	, mMode(eMode::eFill)
	, mWeight(1)
{}

Rectangle::Rectangle(size_t width, size_t height)
	: mColor(0xFF000000)
	, mMode(eMode::eFill)
	, mWeight(1)
{
	mWidth = width;
	mHeight = height;
}

Rectangle::~Rectangle()
{

}

void Rectangle::SetWidth(size_t width)
{
	mWidth = width;
}

void Rectangle::SetHeight(size_t height)
{
	mHeight = height;
}

void Rectangle::SetColor(u32 color)
{
	mColor = color;
}

void Rectangle::SetMode(eMode mode)
{
	mMode = mode;
}

void Rectangle::SetWeight(size_t weight)
{
	mWeight = weight;
}

void Rectangle::Draw(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	switch (mMode)
	{
		case eMode::eFill:
			Fill(frameBuffer, left, top);
			break;

		case eMode::eFrame:
			Frame(frameBuffer, left, top);
			break;
	}
}

void Rectangle::Fill(FrameBuffer& frameBuffer, size_t left, size_t top)
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

			u32* pixel = (u32*)(frameBuffer.GetPixel(dx, dy));
			*pixel = mColor;
		}
	}
}

void Rectangle::Frame(FrameBuffer& frameBuffer, size_t left, size_t top)
{
	for (size_t y = 0; y < mHeight; y++)
	{
		size_t dy = top + y;
		if (dy >= frameBuffer.GetHeight()) break;
		if (dy < 0)continue;

		for (size_t weight = 0; weight < mWeight; weight++)
		{
			u32* pixel = (u32*)(frameBuffer.GetPixel(left + weight, dy));
			if(pixel) *pixel = mColor;
			pixel = (u32*)(frameBuffer.GetPixel(left + mWidth - weight - 1, dy));
			if (pixel) *pixel = mColor;
		}
	}
	for (size_t x = 0; x < mWidth; x++)
	{
		size_t dx = left + x;
		if (dx >= frameBuffer.GetWidth()) break;
		if (dx < 0)continue;

		for (size_t weight = 0; weight < mWeight; weight++)
		{
			u32* pixel = (u32*)(frameBuffer.GetPixel(dx, top + weight));
			if (pixel) *pixel = mColor;
			pixel = (u32*)(frameBuffer.GetPixel(dx, top + mHeight + weight - 1));
			if (pixel) *pixel = mColor;
		}
	}
}
