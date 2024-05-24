#include "TWindow.h"
#include <d3d11.h> // DirectX 기능
//1번방법 : 속성->링커->입력->추가종속성->d3d11.lib 추가
//2번방법
#pragma comment ( lib, "d3d11.lib")

class TDevice : public TWindow
{
public:
	//인터페이스 획득(할당, 생성)
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_pRTV = nullptr;

	bool  CreateDevice(HWND hWnd);
	void  DeleteDevice();

	virtual void   GameRun() override;
};




int WINAPI wWinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance, 
					PWSTR pCmdLine, 
					int nCmdShow)
{
	TDevice win;
	win.CreateRegisterClass(hInstance);
	if (win.CreateWin(1024, 768))
	{
		// 그래픽 처리를 위한 초기화 작업
		if (win.CreateDevice(win.m_hWnd))
		{
			win.WindowRun();
		}
		// 그래픽 처리를 위한 소멸 작업
		win.DeleteDevice();
	}	
	return 0;
}

bool  TDevice::CreateDevice(HWND hWnd)
{
	CONST D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	pSwapChainDesc.BufferDesc.Width = 1024;
	pSwapChainDesc.BufferDesc.Height = 768;
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator= 1;
	pSwapChainDesc.BufferDesc.Format= DXGI_FORMAT_R8G8B8A8_UNORM;
	pSwapChainDesc.SampleDesc.Count = 1;
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pSwapChainDesc.BufferCount = 1;
	pSwapChainDesc.OutputWindow = hWnd;
	pSwapChainDesc.Windowed= true;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&pFeatureLevels,
		1,
		D3D11_SDK_VERSION,
		&pSwapChainDesc,

		&g_pSwapChain,
		&g_pd3dDevice,
		nullptr,
		&g_pContext);

	if (FAILED(hr))
	{
		return false;
	}

	//ID3D11RenderTargetView* g_pRTV = nullptr;
	if (g_pd3dDevice != nullptr && g_pSwapChain != nullptr)
	{
		ID3D11Texture2D* pBackBuffer = nullptr;		
		g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

		ID3D11Resource* pResource = pBackBuffer;
		D3D11_RENDER_TARGET_VIEW_DESC* pDesc = nullptr;
		hr = g_pd3dDevice->CreateRenderTargetView(
			pResource,
			pDesc,			
			&g_pRTV);
		if (FAILED(hr))
		{
			return false;
		}

		pBackBuffer->Release();

	}

	if (g_pContext != nullptr)
	{
		g_pContext->OMSetRenderTargets(1, &g_pRTV, nullptr);
	}
	return true;
}
void  TDevice::DeleteDevice()
{
	if(g_pSwapChain)g_pSwapChain->Release(); 
	if (g_pd3dDevice)g_pd3dDevice->Release();
	if (g_pContext)g_pContext->Release();
	if (g_pRTV)g_pRTV->Release();
}

void   TDevice::GameRun()
{
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	g_pContext->ClearRenderTargetView( g_pRTV, clearColor);

	g_pSwapChain->Present(0, 0);
}