#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriterFont.h"

// �׽�Ʈ�� �Ϸ��ϰ�  TCore�� �����Ѵ�.
class TCore : public TDevice, TBaseCore
{
public:
	TInput m_Input;
	TTimer m_Timer;
	TWriterFont  m_font;
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

	void   GameInit();	
	void   GameRelease();
public:
	void   GameRun();
};

