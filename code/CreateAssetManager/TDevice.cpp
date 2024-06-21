#include "TDevice.h"

ComPtr<ID3D11Device>			TDevice::TDevice::m_pd3dDevice = nullptr;
//관리, 운영
ComPtr<ID3D11DeviceContext>		TDevice::TDevice::m_pContext = nullptr;
ComPtr<IDXGISwapChain>			TDevice::TDevice::m_pSwapChain = nullptr;
ComPtr<ID3D11RenderTargetView>	TDevice::m_pRTV = nullptr;
D3D11_VIEWPORT					TDevice::m_ViewPort;

bool  TDevice::CreateDevice(HWND hWnd, UINT xSize, UINT ySize)
{
	// DWRITE 연동에 필요함.
	UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	CONST D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	pSwapChainDesc.BufferDesc.Width  = xSize;
	pSwapChainDesc.BufferDesc.Height = ySize;
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

		TDevice::m_pSwapChain.GetAddressOf(),
		TDevice::m_pd3dDevice.GetAddressOf(),
		nullptr,
		TDevice::m_pContext.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	//ID3D11RenderTargetView* m_pRTV = nullptr;
	if (TDevice::m_pd3dDevice != nullptr && TDevice::m_pSwapChain != nullptr)
	{
		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		TDevice::m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
			(void**)pBackBuffer.GetAddressOf());

		D3D11_RENDER_TARGET_VIEW_DESC* pDesc = nullptr;
		hr = TDevice::m_pd3dDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			pDesc,
			m_pRTV.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}
	}
	
	SetViewport(xSize, ySize);

	return true;
}
void  TDevice::DeleteDevice()
{	
}

void  TDevice::SetViewport(UINT xSize, UINT ySize)
{
	//DXGI_SWAP_CHAIN_DESC cd;
	//TDevice::m_pSwapChain->GetDesc(&cd);
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = xSize;
	m_ViewPort.Height = ySize;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	TDevice::m_pContext->RSSetViewports(1, &m_ViewPort);
}
