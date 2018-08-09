#pragma once

#include <switch.h>

class Input
{
public:
	Input();
	~Input();

	void Update();
	bool KeyDown(HidControllerKeys key);
};
