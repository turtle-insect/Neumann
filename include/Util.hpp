#pragma once

#include <FrameBuffer.hpp>
#include <switch.h>

static const u32 COLOR_BLACK = 0xFF000000;
static const u32 COLOR_WHITE = 0xFFFFFFFF;
static const u32 COLOR_RED = 0xFF0000FF;
static const u32 CURSOR_COLOR = 0xFF00FF00;

inline u8 blendColor(u32 src, u32 dst, u8 alpha)
{
	u8 one_minus_alpha = static_cast<u8>(255) - alpha;
	return (dst * alpha + src * one_minus_alpha) / static_cast<u8>(255);
}

inline void DrawPixel(FrameBuffer& frameBuffer, size_t x, size_t y, u32 color, u8 alpha)
{
	u8* buffer = frameBuffer.GetPixel(x, y);
	if (buffer == nullptr) return;
	*buffer = blendColor(*buffer, color & 0xFF, alpha); buffer++;
	*buffer = blendColor(*buffer, (color >> 8) & 0xFF, alpha); buffer++;
	*buffer = blendColor(*buffer, (color >> 16) & 0xFF, alpha); buffer++;
	*buffer = 0xFF;
}
