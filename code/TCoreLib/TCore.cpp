#include "TCore.h"
bool		 g_bGameRun = true;

void TCore::Resize(UINT Width, UINT Height)
{
	HRESULT hr;
	// WM_SIZE ������ �����ÿ��� ȣ��ȴ�.
	if (TDevice::m_pd3dDevice == nullptr) return;
	
	m_font.ResourceRelease();
	TDevice::m_pContext->OMSetRenderTargets(0, nullptr, nullptr);
	TDevice::m_pRTV->Release();

	// ���� -> m_SwapChainDesc
	hr = TDevice::m_pSwapChain->ResizeBuffers(
		TDevice::m_SwapChainDesc.BufferCount,
		Width,
		Height,
		TDevice::m_SwapChainDesc.BufferDesc.Format,
		TDevice::m_SwapChainDesc.Flags);
	if (FAILED(hr)) return;

	TDevice::CreateRTV();
	CreateDepthBuffer();

	TDevice::SetViewport();	
	SetFontRTV();

	T::D3DXMatrixPerspectiveFovLH(&m_matProj, TBASIS_PI * 0.25f,
		(float)g_xClientSize / (float)g_yClientSize, 1.0f, 10000.0f);

	Reset();
}
HRESULT  TCore::SetDepthStencilState()
{
	HRESULT hr;
	/// <summary>
	/// ID3D11DepthStencilState ����
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	/*
	*   ���ٽ� ���ۿ��� ���ȴ�.
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
	if (FAILED(hr)) return hr;

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
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT TCore::CreateDepthBuffer()
{
	HRESULT hr;
	/// <summary>
	/// �ؽ�ó ����
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = TDevice::m_SwapChainDesc.BufferDesc.Width;
	td.Height = TDevice::m_SwapChainDesc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS; // D,S
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = TDevice::m_pd3dDevice->CreateTexture2D(&td, nullptr, tex.GetAddressOf());
	if (FAILED(hr)) return hr;

	/// <summary>
	/// DepthStencilView ����
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Texture2D.MipSlice  -> ShaderResourceVie���� ����.
	hr = TDevice::m_pd3dDevice->CreateDepthStencilView(tex.Get(), &dsvd, m_pDSV.ReleaseAndGetAddressOf());
	if (FAILED(hr)) return hr;

	
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
	float clearColor[] = { 0.343f, 0.45f, 0.74f, 1.0f };
	TDevice::m_pContext->ClearRenderTargetView(TDevice::m_pRTV, clearColor);
	TDevice::m_pContext->ClearDepthStencilView(m_pDSV.Get(),
							D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0);
	TDevice::m_pContext->OMSetRenderTargets(1, &TDevice::m_pRTV,  m_pDSV.Get());
	
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
	PreRender();
}
void  TCore::GamePostRender()
{
	PostRender();
	DebugRender();
	m_font.DrawText(m_Timer.m_csBuffer, { 0,0 });
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
	m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f),
		FVector3(10000.0f, 0.0f, 0.0f),
		FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f),
		FVector3(0.0f, 10000.0f, 0.0f),
		FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f),
		FVector3(0.0f, 0.0f, 10000.0f),
		FVector4(0.0f, 0.0f, 1.0f, 1.0f));

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

		T::TVector3 eye = { 0.0f, 0.0f, -25.0f };
		T::TVector3 target = { 0.0f, 0.0f, 0.0f };
		T::TVector3 up = { 0.0f, 1.0f, 0.0f };
		// ���� '=': ������ �ǿ����ڷ� 'T_Math::FMatrix' ������ ����ϴ� �����ڰ� ���ų� ���Ǵ� ��ȯ�� �����ϴ�.
		m_MainCamera.SetView(eye, target, up);

		T::D3DXMatrixPerspectiveFovLH(&m_matProj, TBASIS_PI * 0.25f,
			(float)g_xClientSize / (float)g_yClientSize, 1.0f, 10000.0f);

		Reset();
	}
	Init();
}
void   TCore::SetFontRTV()
{
	// 3D ����۸� �� �����ؾ� �Ѵ�.
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
	m_Line.Release();

	I_Sound.Release();
	I_Shader.Release();
	I_Tex.Release();

	m_font.Release();
	TDevice::DeleteDevice();
}