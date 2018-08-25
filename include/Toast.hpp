#pragma once

#include <string>
#include <switch.h>
#include "Sprite.hpp"
#include "Rectangle.hpp"

class Text;
class Rectangle;

class Toast : public Sprite
{
public:
	Toast();
	~Toast();

	void Popup(std::string message, u32 color);
	bool Update();
	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);

private:
	uint mTimer;
	Text* mText;
	Rectangle mBody;
	Rectangle mFrame;
};
