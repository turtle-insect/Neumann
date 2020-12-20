#pragma once

#include <switch.h>

class Input
{
public:
	Input();
	~Input();

	void Update();
	bool KeyDown(HidNpadButton key);

private:
	PadState mPad;
};
