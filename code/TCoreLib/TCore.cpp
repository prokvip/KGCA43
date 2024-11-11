#include "TCore.h"
bool		 g_bGameRun = true;

void TCore::Resize(UINT Width, UINT Height)
{
	HRESULT hr;
	// WM_SIZE 윈도우 생성시에도 호출된다.
	if (TDevice::m_pd3dDevice == nullptr) return;
	
	m_font.ResourceRelease();
	TDevice::m_pContext->OMSetRenderTargets(0, nullptr, nullptr);
	TDevice::m_RT.m_pRTV->Release();

	// 갱신 -> m_SwapChainDesc
	hr = TDevice::m_pSwapChain->ResizeBuffers(
		TDevice::m_SwapChainDesc.BufferCount,
		Width,
		Height,
		TDevice::m_SwapChainDesc.BufferDesc.Format,
		TDevice::m_SwapChainDesc.Flags);
	if (FAILED(hr)) return;

	TDevice::CreateRTV();
	CreateDepthBuffer();

	TDevice::SetViewport(TDevice::m_SwapChainDesc.BufferDesc.Width,
						 TDevice::m_SwapChainDesc.BufferDesc.Height);
	SetFontRTV();

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	g_xClientSize = m_rtClient.right;
	g_yClientSize = m_rtClient.bottom;

	m_MainCamera.SetProj(XM_PI * 0.25f,
		(float)g_xClientSize / (float)g_yClientSize, 0.1f, 10000.0f);

	Reset();
}
HRESULT  TCore::SetDepthStencilState()
{
	HRESULT hr;
	/// <summary>
	/// ID3D11DepthStencilState 생성
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	/*
	*   스텐실 버퍼에서 사용된다.
	*
	BOOL StencilEnable;
	UINT8 StencilReadMask;
	UINT8 StencilWriteMask;
	D3D11_DEPTH_STENCILOP_DESC FrontFace;
	D3D11_DEPTH_STENCILOP_DESC BackFace;*/
	hr = TDevice::m_pd3dDevice->CreateDepthStencilState(&dsd, m_pDepthEnable.GetAddressOf());
	if (FAILED(hr)) return hr;

	dsd.DepthEnable = FALSE;
	hr = TDevice::m_pd3dDevice->CreateDepthStencilState(&dsd, m_pDepthDisable.GetAddressOf());
	return hr;
}
HRESULT  TCore::SetRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode= D3D11_FILL_SOLID;
	rd.CullMode= D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(&rd, m_pRSSolid.GetAddressOf());
	if (FAILED(hr)) return hr;

	rd.FillMode = D3D11_FILL_WIREFRAME;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(&rd, m_pRSWireFrame.GetAddressOf());
	if (FAILED(hr)) return hr;

	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(&rd, 
		m_pRSCullNone.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}
HRESULT  TCore::SetSamplerState()
{
	HRESULT hr = S_OK;
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter= D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MaxLOD = FLT_MAX;
	sd.MinLOD = FLT_MIN;
	hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, m_pSSPoint.GetAddressOf());
	if (FAILED(hr)) return hr;
	
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, m_pSSLinear.GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
HRESULT  TCore::SetAlphaBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

	bd.AlphaToCoverageEnable = FALSE;//discard;같은 결과.
	bd.IndependentBlendEnable = FALSE;
	//D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
	// 백버퍼의 컬러값(DestBlend) 과  현재 출력 컬러(SrcBlend)값을 혼합연산한다.
	bd.RenderTarget[0].BlendEnable = TRUE;
	// RGA 컬러값 연산( 기본 알파블랭딩 공식) 알파범위( 0 ~ 1 )
	// 최종 컬러값 = 소스컬러*소스알파 	+  데스크컬러* (1.0 - 소스알파)
	//  정점위치 = 목적지위치*S + 현재위치* (1- S); S=0, S=0.5, S = 1
	// 
	// 만약 소스컬러 = 1,0,0,1(빨강)   배경컬러 = 0,0,1,1(파랑)
	// 1)소스알파 = 1.0F ( 완전불투명)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// 최종 컬러값 = 빨강*1.0F 	+  파랑* (1.0 - 1.0F)
		// ->최종 컬러값(소스색) = [1,0,0] 	+  [0,0,0]
	// 2)소스알파 = 0.0F ( 완전투명)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// 최종 컬러값 = 빨강*0.0F 	+  파랑* (1.0 - 0.0F)
		// ->최종 컬러값(배경색) = [0,0,0] +  [0,0,1]
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// A 알파값 연산
	// A = SRC Alpha*1 + DestAlpha*0;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = TDevice::m_pd3dDevice->CreateBlendState(&bd, m_pAlphaBlend.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT TCore::CreateDepthBuffer()
{
	HRESULT hr = TDevice::m_RT.CreateDepthBuffer(
		TDevice::m_SwapChainDesc.BufferDesc.Width,
		TDevice::m_SwapChainDesc.BufferDesc.Height);

	hr = m_MainRT.Create(TDevice::m_pd3dDevice.Get(),1024, 1024);	
	return S_OK;
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

	if (I_Input.KeyCheck(VK_INSERT) == KEY_PUSH)
	{
		m_bDepthEnable = !m_bDepthEnable;
	}
	if (I_Input.KeyCheck(VK_DELETE) == KEY_PUSH)
	{
		m_bSolidEnable = !m_bSolidEnable;
	}
	if (I_Input.KeyCheck(VK_END) == KEY_PUSH)
	{
		m_bLinearEnable = !m_bLinearEnable;
	}

	m_MainCamera.Frame();
	m_SkyObject->Frame();
	PreFrame();
}
void   TCore::GameFrame()
{
	GamePreFrame();
	Frame();
	GamePostFrame();
}
void   TCore::GamePostFrame()
{
	PostFrame();
}
void  TCore::GamePreRender()
{
	if (m_isTexRender)
	{
		m_MainRT.Begin();
	}
	else
	{
		TDevice::m_RT.Begin();		
		///*float clearColor[] = { 0.343f, 0.45f, 0.74f, 1.0f };
		//TDevice::m_pContext->ClearRenderTargetView(TDevice::m_pRTV, clearColor);
		//TDevice::m_pContext->ClearDepthStencilView(m_pDSV.Get(),
		//						D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0);
		//TDevice::m_pContext->OMSetRenderTargets(1, &TDevice::m_pRTV,  m_pDSV.Get());
		//*/
	}
	TDevice::m_pContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);
	
	if (m_bDepthEnable)
		TDevice::m_pContext->OMSetDepthStencilState(m_pDepthEnable.Get(), 0);		
	else
		TDevice::m_pContext->OMSetDepthStencilState(m_pDepthDisable.Get(), 0);

	if (m_bSolidEnable)
		TDevice::m_pContext->RSSetState(m_pRSSolid.Get());
	else
		TDevice::m_pContext->RSSetState(m_pRSWireFrame.Get());
	
	TDevice::m_pContext->PSSetSamplers(1, 1, m_pSSPoint.GetAddressOf());
	if (m_bLinearEnable)
	{
		TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSLinear.GetAddressOf());
	}
	else
	{
		TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSPoint.GetAddressOf());
	}
	TDevice::m_pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_SkyObject->SetMatrix(nullptr, &m_MainCamera.m_matView,
		&m_MainCamera.m_matProj);
	m_SkyObject->Render();

	PreRender();
}
void  TCore::GamePostRender()
{
	PostRender();
	DebugRender();
	m_font.DrawText(m_Timer.m_csBuffer, { 0,0 });

	if (m_isTexRender)
	{
		m_MainRT.End();

		TDevice::m_RT.Begin();
		// screen quad 
		m_ViewportObj.SetMatrix(nullptr, nullptr, nullptr);
		m_ViewportObj.PreRender(TDevice::m_pContext);
		TDevice::m_pContext->PSSetShaderResources(0,
			1, m_MainRT.m_pSRV_RT.GetAddressOf());
		m_ViewportObj.PostRender(TDevice::m_pContext);
		TDevice::m_RT.End();
	}
	TDevice::m_pSwapChain->Present(0, 0);
}
void   TCore::GameRender()
{
	GamePreRender();
	Render();
	GamePostRender();	
}
void   TCore::DebugRender()
{
	m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, 
							  &m_MainCamera.m_matProj);
	m_Line.Draw(T::TVector3(0.0f, 0.0f, 0.0f),
		T::TVector3(10000.0f, 0.0f, 0.0f),
		T::TVector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_Line.Draw(T::TVector3(0.0f, 0.0f, 0.0f),
		T::TVector3(0.0f, 10000.0f, 0.0f),
		T::TVector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_Line.Draw(T::TVector3(0.0f, 0.0f, 0.0f),
		T::TVector3(0.0f, 0.0f, 10000.0f),
		T::TVector4(0.0f, 0.0f, 1.0f, 1.0f));

	/*m_Timer.DebugTimer();
	I_Input.DebugMousePos();
	I_Input.KeyTest();*/
	I_Sound.Render();
}
void   TCore::GameInit()
{
	// 그래픽 처리를 위한 초기화 작업
	if (TDevice::CreateDevice(m_hWnd))
	{		
		CreateDepthBuffer();
		SetDepthStencilState();

		SetAlphaBlendState();
		SetRasterizerState();
		SetSamplerState();

		I_Tex.Set(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext);
		I_Shader.Set(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext);
		I_Sound.Set(nullptr, nullptr);
		//I_Sprite.Load(L"../../data/Sprite/SpriteInfo.txt");
		m_font.Init();
		SetFontRTV();

		m_Line.Create(L"../../data/obj.jpg", L"../../data/shader/line.hlsl");


		m_MainCamera.Init();
		T::TVector3 eye = { 0.0f, 0.0f, -25.0f };
		T::TVector3 target = { 0.0f, 0.0f, 0.0f };
		T::TVector3 up = { 0.0f, 1.0f, 0.0f };
		// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
		m_MainCamera.SetProj(XM_PI * 0.25f,
			(float)g_xClientSize / (float)g_yClientSize,
			0.1f, 10000.0f);
		m_MainCamera.SetView(eye, target, up);
		
		m_SkyObject = std::make_shared<TSkyBox>();
		m_SkyObject->Create(L"../../data/obj.jpg",
			L"../../data/shader/skyobject.hlsl");

		m_ViewportObj.Create(L"", L"../../data/shader/ScreenQuad.txt");
		Reset();
	}
	Init();
}
void   TCore::SetFontRTV()
{
	// 3D 백버퍼를 얻어서 전달해야 한다.
	IDXGISurface* dxgiSurface = nullptr;
	TDevice::m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(void**)&dxgiSurface);
	m_font.ResetDevice(dxgiSurface);
	dxgiSurface->Release();
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
	m_ViewportObj.Release();
	m_Line.Release();
	m_MainCamera.Release();
	m_SkyObject->Release();

	I_Sound.Release();
	I_Shader.Release();
	I_Tex.Release();

	m_font.Release();
	m_MainRT.Release();
	TDevice::DeleteDevice();
}