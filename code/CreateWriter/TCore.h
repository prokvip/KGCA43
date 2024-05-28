#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
class TCore : public TDevice
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

