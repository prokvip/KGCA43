#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriterFont.h"
#include "TTexMgr.h"
#include "TShaderMgr.h"
#include "TSound.h"
// 테스트를 완료하고  TCore에 포함한다.
class TCore : public TWindow, public TBaseCore
{
public:
	bool			m_bDepthEnable = true;
	TTimer			m_Timer;
	TWriterFont		m_font;
	ComPtr<ID3D11DepthStencilView> m_pDSV = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthEnable = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthDisable = nullptr;
	ComPtr<ID3D11BlendState> m_pAlphaBlend = nullptr;
public:
	virtual void Init();
	virtual void PreFrame() {};
		virtual void Frame();
	virtual void PostFrame() {};
	virtual void PreRender() {};
		virtual void Render();
	virtual void PostRender() {};
	virtual void Release();

	HRESULT  SetAlphaBlendState();
	HRESULT  CreateDepthBuffer();
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

