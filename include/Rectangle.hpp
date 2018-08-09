#pragma once

#include "Sprite.hpp"

class Rectangle : public Sprite
{
public:
	enum eMode
	{
		eFill,
		eFrame,
	};
	Rectangle();
	Rectangle(size_t width, size_t height);
	~Rectangle();

	void SetWidth(size_t width);
	void SetHeight(size_t height);
	void SetColor(u32 color);
	void SetMode(eMode mode);
	void SetWeight(size_t weight);
	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);

private:
	void Fill(FrameBuffer& frameBuffer, size_t left, size_t top);
	void Frame(FrameBuffer& frameBuffer, size_t left, size_t top);

	u32 mColor;
	eMode mMode;
	size_t mWeight;
};