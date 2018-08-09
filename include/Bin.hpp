#pragma once

#include <switch.h>
#include "Sprite.hpp"

class Bin : public Sprite
{
public:
	enum eType
	{
		eA,
		eB,
		eX,
		eY,
		eMinus,
	};

	Bin(eType type);
	~Bin();

	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);
private:
	const u8* mBuffer;
};
