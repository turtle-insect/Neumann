#include <switch.h>
#include "Device.hpp"
#include "FrameBuffer.hpp"
#include "Input.hpp"
#include "Resource.hpp"
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
	accountInitialize(AccountServiceType_System);
	plInitialize(PlServiceType_User);
	timeInitialize();
	nsInitialize();

	Device::Instance().Initialize();
	Resource::Instance().Initialize();
}

Application::~Application()
{
	nsExit();
	timeExit();
	plExit();
	accountExit();
	nsExit();
	fsExit();
	setsysExit();
}

void Application::Run()
{
	IScene* scene = new BootScene();

	Input input;
	FrameBuffer frameBuffer;
	frameBuffer.Init();

	while (appletMainLoop())
	{
		input.Update();
		frameBuffer.Begin();

		if (input.KeyDown(HidNpadButton_Plus)) break;

		IScene* next = scene->Update(input);
		if (next != scene)
		{
			scene->Leave();
			next->Entry();
			delete scene;
			scene = next;
		}
		clearScreen(frameBuffer, 0xFFEAEAEA);
		scene->Draw(frameBuffer);
		frameBuffer.End();
	}

	delete scene;
}
