#include "Sample.h"


HRESULT Sample::CreateDepthBuffer()
{
	HRESULT hr;
	/// <summary>
	/// 텍스처 생성
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = TDevice::m_SwapChainDesc.BufferDesc.Width;
	td.Height= TDevice::m_SwapChainDesc.BufferDesc.Height;
	td.MipLevels =1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS; // D,S
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;	
	hr = TDevice::m_pd3dDevice->CreateTexture2D(&td, nullptr, tex.GetAddressOf());
	if( FAILED(hr)) return hr;

	/// <summary>
	/// DepthStencilView 생성
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension= D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Texture2D.MipSlice  -> ShaderResourceVie에서 사용됨.
	hr = TDevice::m_pd3dDevice->CreateDepthStencilView(tex.Get(), &dsvd, m_pDSV.GetAddressOf());
	if (FAILED(hr)) return hr;

	/// <summary>
	/// ID3D11DepthStencilState 생성
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask= D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc= D3D11_COMPARISON_LESS_EQUAL;
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
	if (FAILED(hr)) return hr;

	return hr;
}
void   Sample::Init()
{
	CreateDepthBuffer();
	//2>D:\00_43\code\Basic2\pnct.hlsl(45,2-26):error X3017: cannot convert from 'const float4' to 'struct PS_Out'
	//2 > FXC : error X3501 : 'main' : entrypoint not found
	//2 > compilation object save succeeded; see D : \00_43\output\pnct.cso
	this->m_obj.Create(L"../../data/1234.jpg", L"pnct.hlsl");
	this->m_objFar.Create(L"../../data/obj.jpg", L"pnct.hlsl");	

	m_obj.m_vPos = { 0.0f,0.0f, 0.0f };
	m_objFar.m_vPos = { 5.0f,0.0f, 1.0f };
}
void  Sample::PreRender()
{
	TDevice::m_pContext->ClearDepthStencilView(m_pDSV.Get(), 
		   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		   1.0f                  , 0);
	TDevice::m_pContext->OMSetRenderTargets(1, &TDevice::m_pRTV, 
												m_pDSV.Get());
	TDevice::m_pContext->OMSetDepthStencilState(m_pDepthEnable.Get(), 0);
}
void    Sample::Frame()
{
	this->m_obj.Frame();
	this->m_objFar.Frame();
}
void    Sample::Render()
{	
	if (I_Input.m_dwMouseState[0] == KEY_HOLD)
	{
		TDevice::m_pContext->OMSetDepthStencilState(m_pDepthDisable.Get(), 0);
	}
	else
	{
		TDevice::m_pContext->OMSetDepthStencilState(m_pDepthEnable.Get(), 0);
	}
	
	this->m_obj.Render(TDevice::m_pContext);
	this->m_objFar.Render(TDevice::m_pContext);
}
void    Sample::Release()
{
	this->m_obj.Release();
	this->m_objFar.Release();
}

T_GAME_START(800, 600);
