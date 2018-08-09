#include "Input.hpp"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Update()
{
	hidScanInput();
}

bool Input::KeyDown(HidControllerKeys key)
{
	u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	return (kDown & key) != 0;
}
