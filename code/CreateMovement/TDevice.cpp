#include "TDevice.h"

bool  TDevice::CreateDevice(HWND hWnd)
{
	// DWRITE 연동에 필요함.
	UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	CONST D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	pSwapChainDesc.BufferDesc.Width = g_xClientSize;
	pSwapChainDesc.BufferDesc.Height = g_yClientSize;
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pSwapChainDesc.SampleDesc.Count = 1;
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pSwapChainDesc.BufferCount = 1;
	pSwapChainDesc.OutputWindow = hWnd;
	pSwapChainDesc.Windowed = true;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		Flags,
		&pFeatureLevels,
		1,
		D3D11_SDK_VERSION,
		&pSwapChainDesc,
		m_pSwapChain.GetAddressOf(),
		m_pd3dDevice.GetAddressOf(),
		nullptr,
		m_pContext.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	//ID3D11RenderTargetView* m_pRTV = nullptr;
	if (m_pd3dDevice != nullptr && m_pSwapChain != nullptr)
	{
		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
			(void**)pBackBuffer.GetAddressOf());

		D3D11_RENDER_TARGET_VIEW_DESC* pDesc = nullptr;
		hr = m_pd3dDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			pDesc,
			&m_pRTV);
		if (FAILED(hr))
		{
			return false;
		}
	}
	
	SetViewport();

	return true;
}
void  TDevice::DeleteDevice()
{
}

void  TDevice::SetViewport()
{
	//DXGI_SWAP_CHAIN_DESC cd;
	//m_pSwapChain->GetDesc(&cd);
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = g_xClientSize;
	m_ViewPort.Height = g_yClientSize;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	m_pContext->RSSetViewports(1, &m_ViewPort);
}
