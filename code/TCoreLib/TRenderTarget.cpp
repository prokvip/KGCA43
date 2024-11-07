#include "TRenderTarget.h"
#include "TDevice.h"
bool  TRenderTarget::CreateRTV(ID3D11Device* pDevice,
							   IDXGISwapChain* pSwapChain)
{
	HRESULT hr;
	//ID3D11RenderTargetView* m_pRTV = nullptr;
	if (pDevice != nullptr && pSwapChain != nullptr)
	{
		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)pBackBuffer.GetAddressOf());

		D3D11_RENDER_TARGET_VIEW_DESC* pDesc = nullptr;
		hr = pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			pDesc,
			m_pRTV.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (pSwapChain)
		pSwapChain->GetDesc(&m_SwapChainDesc);
	return true;
}
void  TRenderTarget::Begin()
{
	float clearColor[] = { 0.343f, 0.45f, 0.74f, 1.0f };
	TDevice::m_pContext->ClearRenderTargetView(
		m_pRTV.Get(), clearColor);
	TDevice::m_pContext->ClearDepthStencilView(m_pDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	TDevice::m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());
}
void  TRenderTarget::End()
{

}
void  TRenderTarget::DeleteDevice()
{	
	m_pRTV = nullptr;
}
void  TRenderTarget::SetViewport(UINT iWidth, UINT iHeight)
{
	//DXGI_SWAP_CHAIN_DESC cd;
	//m_pSwapChain->GetDesc(&cd);
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	TDevice::m_pContext->RSSetViewports(1, &m_ViewPort);
}
HRESULT TRenderTarget::CreateDepthBuffer(UINT iWidth, UINT iHeight)
{
	HRESULT hr;
	/// <summary>
	/// 텍스처 생성
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = iWidth;
	td.Height = iHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS; // D,S
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = TDevice::m_pd3dDevice->CreateTexture2D(&td, nullptr, tex.GetAddressOf());
	if (FAILED(hr)) return hr;

	/// <summary>
	/// DepthStencilView 생성
	/// </summary>
	/// <returns></returns>
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Texture2D.MipSlice  -> ShaderResourceVie에서 사용됨.
	hr = TDevice::m_pd3dDevice->CreateDepthStencilView(tex.Get(),
		&dsvd, m_pDSV.ReleaseAndGetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
void  TRenderTarget::Release()
{

}
bool	TRenderTarget::Create(ID3D11Device* pDevice, UINT iWidth, UINT iHeight)
{
	return false;
}
ID3D11Texture2D* TRenderTarget::CreateTexture(ID3D11Device* pDevice, UINT iWidth, UINT iHeight)
{
	return nullptr;
}
bool	TRenderTarget::CreateRTV(ID3D11Device* pDevice, ID3D11Texture2D* pTex)
{
	return false;
}