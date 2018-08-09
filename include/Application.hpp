#pragma once

#include "Input.hpp"
#include "FrameBuffer.hpp"

class Application
{
public:
	Application();
	~Application();
	void Run();

private:
	Input mInput;
	FrameBuffer mFrameBuffer;
};
