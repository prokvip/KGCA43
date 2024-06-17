#include "TCore.h"
void TCore::Init(){}
void TCore::Frame() {}
void TCore::Render() {}
void TCore::Release() {}

void   TCore::GamePreFrame()
{ 
	m_Timer.Frame();
	if (m_isActive)
	{
		m_Input.Frame(m_hWnd);
	}
}
void   TCore::GameFrame()
{
	GamePreFrame();
	Frame();
	GamePostFrame();
}
void   TCore::GamePostFrame()
{
}
void  TCore::GamePreRender()
{
	float clearColor[] = { 0.3640f, 0.4543545322f, 0.645672321f, 1.0f };
	m_pContext->ClearRenderTargetView(m_pRTV, clearColor);

	m_pContext->OMSetRenderTargets(1, &m_pRTV, nullptr);
	m_pContext->RSSetViewports(1, &m_ViewPort);
}
void  TCore::GamePostRender()
{
	m_font.DrawText(m_Timer.m_csBuffer, { 0,0 });
	m_pSwapChain->Present(0, 0);
}
void   TCore::GameRender()
{
	GamePreRender();
	Render();
	GamePostRender();
	DebugRender();
}
void   TCore::DebugRender()
{
	/*m_Timer.DebugTimer();
	m_Input.DebugMousePos();
	m_Input.KeyTest();*/
}
void   TCore::GameInit()
{
	// 그래픽 처리를 위한 초기화 작업
	if (TDevice::CreateDevice(m_hWnd))
	{
		m_font.Init();
		// 3D 백버퍼를 얻어서 전달해야 한다.
		IDXGISurface* dxgiSurface = nullptr;
		m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
			(void**)&dxgiSurface);
		m_font.ResetDevice(dxgiSurface);
		dxgiSurface->Release();
	}
	Init();
}
void   TCore::GameRun()
{
	GameInit();
	while(m_bGameRun)
	{
		if(TWindow::WindowRun()==false)
		{
			break;
		}
		GameFrame();
		GameRender();
	}
	GameRelease();
}

void   TCore::GameRelease()
{
	Release();
	m_font.Release();
	DeleteDevice();
}