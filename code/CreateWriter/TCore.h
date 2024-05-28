#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"

class TBaseCore
{
	virtual void Init() = 0;
	virtual void Frame() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};
// 테스트를 완료하고  TCore에 포함한다.
class TCore : public TDevice, TBaseCore
{
public:
	TInput m_Input;
	TTimer m_Timer;
public:
	virtual void Init();
	virtual void Frame();
	virtual void Render();
	virtual void Release();
private:
	void   GamePreFrame();
	void   GameFrame();
	void   GamePostFrame();

	void   GamePreRender();
	void   GameRender();
	void   GamePostRender();

	void   DebugRender();

	virtual void   GameInit()override;	
		virtual void   GameRun() override;
	virtual void   GameRelease()override;
};

