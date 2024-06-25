#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriterFont.h"
#include "TTexMgr.h"
#include "TShaderMgr.h"
#include "TSound.h"
#include "TSprite.h"
// 테스트를 완료하고  TCore에 포함한다.
class TCore : public TDevice, TBaseCore
{
public:
	TTimer m_Timer;
	TWriterFont  m_font;
	bool		 m_bGameRun = true;

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

