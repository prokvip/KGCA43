#include "TCore.h"
bool		 g_bGameRun = true;

HRESULT  TCore::SetAlphaBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

	bd.AlphaToCoverageEnable = FALSE;//discard;���� ���.
	bd.IndependentBlendEnable = FALSE;
	//D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
	// ������� �÷���(DestBlend) ��  ���� ��� �÷�(SrcBlend)���� ȥ�տ����Ѵ�.
	bd.RenderTarget[0].BlendEnable = TRUE;
	// RGA �÷��� ����( �⺻ ���ĺ��� ����) ���Ĺ���( 0 ~ 1 )
	// ���� �÷��� = �ҽ��÷�*�ҽ����� 	+  ����ũ�÷�* (1.0 - �ҽ�����)
	//  ������ġ = ��������ġ*S + ������ġ* (1- S); S=0, S=0.5, S = 1
	// 
	// ���� �ҽ��÷� = 1,0,0,1(����)   ����÷� = 0,0,1,1(�Ķ�)
	// 1)�ҽ����� = 1.0F ( ����������)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// ���� �÷��� = ����*1.0F 	+  �Ķ�* (1.0 - 1.0F)
		// ->���� �÷���(�ҽ���) = [1,0,0] 	+  [0,0,0]
	// 2)�ҽ����� = 0.0F ( ��������)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// ���� �÷��� = ����*0.0F 	+  �Ķ�* (1.0 - 0.0F)
		// ->���� �÷���(����) = [0,0,0] +  [0,0,1]
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// A ���İ� ����
	// A = SRC Alpha*1 + DestAlpha*0;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = TDevice::m_pd3dDevice->CreateBlendState(&bd, m_pAlphaBlend.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		TDevice::m_pContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);
	}
	return hr;
}

void TCore::Init(){}
void TCore::Frame() {}
void TCore::Render() {}
void TCore::Release() {}

void   TCore::GamePreFrame()
{ 
	m_Timer.Frame();
	if (m_isActive)
	{
		I_Input.Frame(m_hWnd);
	}
	I_Sound.Frame();
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
	TDevice::m_pContext->ClearRenderTargetView(TDevice::m_pRTV, clearColor);

	TDevice::m_pContext->OMSetRenderTargets(1, &TDevice::m_pRTV, nullptr);
	TDevice::m_pContext->RSSetViewports(1, &TDevice::m_ViewPort);
	TDevice::m_pContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);
}
void  TCore::GamePostRender()
{
	m_font.DrawText(m_Timer.m_csBuffer, { 0,0 });
	TDevice::m_pSwapChain->Present(0, 0);
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
	I_Input.DebugMousePos();
	I_Input.KeyTest();*/
	I_Sound.Render();
}
void   TCore::GameInit()
{
	// �׷��� ó���� ���� �ʱ�ȭ �۾�
	if (TDevice::CreateDevice(m_hWnd))
	{
		I_Tex.Set(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext);
		I_Shader.Set(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext);
		SetAlphaBlendState();

		I_Sound.Set(nullptr, nullptr);
		//I_Sprite.Load(L"../../data/Sprite/SpriteInfo.txt");

		m_font.Init();
		// 3D ����۸� �� �����ؾ� �Ѵ�.
		IDXGISurface* dxgiSurface = nullptr;
		TDevice::m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
			(void**)&dxgiSurface);
		m_font.ResetDevice(dxgiSurface);
		dxgiSurface->Release();
	}
	Init();
}
void   TCore::GameRun()
{
	GameInit();
	while(g_bGameRun)
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

	I_Sound.Release();
	I_Shader.Release();
	I_Tex.Release();

	m_font.Release();
	TDevice::DeleteDevice();
}