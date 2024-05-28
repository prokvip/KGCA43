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
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	g_pContext->ClearRenderTargetView(g_pRTV, clearColor);
}
void  TCore::GamePostRender()
{
	g_pSwapChain->Present(0, 0);
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
	Init();
}
void   TCore::GameRun()
{
	GameFrame();	// 매프레임 마다 계산하고
	GameRender();
}
void   TCore::GameRelease()
{
	Release();
}