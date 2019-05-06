#include <switch.h>

#include "Device.hpp"
#include "Input.hpp"
#include "FrameBuffer.hpp"
#include "Scene.hpp"

#include "Application.hpp"

void clearScreen(FrameBuffer& frameBuffer, u32 color)
{
	size_t count = frameBuffer.GetWidth() * frameBuffer.GetHeight();
	u32* buffer = (u32*)frameBuffer.GetBuffer();
	for (size_t i = 0; i < count; i++)
	{
		*buffer = color;
		buffer++;
	}
}

Application::Application()
{
	setsysInitialize();
	fsInitialize();
	nsInitialize();
	accountInitialize();
	plInitialize();
	timeInitialize();
}

Application::~Application()
{
	timeExit();
	plExit();
	accountExit();
	nsExit();
	fsExit();
	setsysExit();
}

void Application::Run()
{
	Device& device = Device::Instance();
	device.Initialize();

	IScene* scene = new BootScene();
	mFrameBuffer.Init();

	while (appletMainLoop())
	{
		mInput.Update();
		mFrameBuffer.Begin();

		if (mInput.KeyDown(KEY_PLUS)) break;

		IScene* next = scene->Update(mInput);
		if (next != scene)
		{
			scene->Leave();
			next->Entry();
			delete scene;
			scene = next;
		}
		clearScreen(mFrameBuffer, 0xFFEAEAEA);
		scene->Draw(mFrameBuffer);
		mFrameBuffer.End();
	}
}
