#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriterFont.h"
#include "TTexMgr.h"
#include "TShaderMgr.h"
#include "TSound.h"
#include "TPlaneObj.h"
#include "TBoxObj.h"
#include "TLineObj.h"
#include "TCamera.h"
// 테스트를 완료하고  TCore에 포함한다.
class TCore : public TWindow, public TBaseCore
{
public:
	TCamera			m_MainCamera;
	TLineObj		m_Line;
public:
	bool			m_bDepthEnable = true;
	bool			m_bSolidEnable = true;
	bool			m_bLinearEnable = true;
	TTimer			m_Timer;
	TWriterFont		m_font;
	ComPtr<ID3D11DepthStencilView>  m_pDSV = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthEnable = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthDisable = nullptr;
	ComPtr<ID3D11BlendState>		m_pAlphaBlend = nullptr;
	ComPtr<ID3D11RasterizerState>	m_pRSWireFrame = nullptr;
	ComPtr<ID3D11RasterizerState>	m_pRSSolid = nullptr;
	ComPtr<ID3D11SamplerState>		m_pSSPoint = nullptr;
	ComPtr<ID3D11SamplerState>		m_pSSLinear= nullptr;
public:
	virtual void Resize(UINT w, UINT h) override;
	virtual void SetFontRTV();
public:
	virtual void Init();
	virtual void PreFrame() {};
		virtual void Frame();
	virtual void PostFrame() {};
	virtual void PreRender() {};
		virtual void Render();
	virtual void PostRender() {};
	virtual void Release();
	virtual void Reset() {};

	
	HRESULT  CreateDepthBuffer();
	HRESULT  SetDepthStencilState();
	HRESULT  SetAlphaBlendState();	
	HRESULT  SetRasterizerState();
	HRESULT  SetSamplerState();
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
	void   ToolGameInit()
	{
		GetWindowRect(m_hWnd, &m_rtWindow);
		GetClientRect(m_hWnd, &m_rtClient);
		g_xClientSize = m_rtClient.right;
		g_yClientSize = m_rtClient.bottom;
		GameInit();
	};
	void   ToolGameRelease()
	{
		GameRelease();
	}
	void   ToolGameFrame()
	{
		GameFrame();
	}
	void   ToolGameRender()
	{
		GameRender();
	}
	void   GameRun();
};

