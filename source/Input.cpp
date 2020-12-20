#include "Input.hpp"

Input::Input()
{
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&mPad);
}

Input::~Input()
{
}

void Input::Update()
{
	padUpdate(&mPad);
}

bool Input::KeyDown(HidNpadButton key)
{
	u64 kDown = padGetButtonsDown(&mPad);
	return (kDown & key) != 0;
}
