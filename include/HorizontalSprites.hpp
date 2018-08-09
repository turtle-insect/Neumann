#pragma once

#include <vector>
#include "Sprite.hpp"

class HorizontalSprites : public Sprite
{
public:
	HorizontalSprites();
	~HorizontalSprites();

	void Clear();
	void AppendSprite(Sprite* sprite, size_t margin);
	void Draw(FrameBuffer& frameBuffer, size_t left, size_t top);

private:
	std::vector<size_t> mMargin;
	std::vector<Sprite*> mSprites;
};
