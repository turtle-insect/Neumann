#pragma once

class Input;
class FrameBuffer;

class IScene
{
public:
	IScene(){};
	virtual ~IScene(){};

	virtual void Entry(){};
	virtual void Leave(){};
	virtual IScene* Update(Input& input) = 0;
	virtual void Draw(FrameBuffer& frameBuffer) = 0;
};
