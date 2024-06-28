#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriterFont.h"
#include "TTexMgr.h"
#include "TShaderMgr.h"
#include "TSound.h"
#include "TSprite.h"
// �׽�Ʈ�� �Ϸ��ϰ�  TCore�� �����Ѵ�.
class TCore : public TWindow, public TBaseCore
{
public:
	TTimer m_Timer;
	TWriterFont  m_font;
	

	ComPtr<ID3D11BlendState> m_pAlphaBlend = nullptr;
public:
	virtual void Init();
	virtual void Frame();
	virtual void Render();
	virtual void Release();

	HRESULT  SetAlphaBlendState();
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

