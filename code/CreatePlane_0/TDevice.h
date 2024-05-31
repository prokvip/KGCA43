#pragma once
#include "TWindow.h"
#include <d3d11.h> // DirectX 扁瓷
//1锅规过 : 加己->傅目->涝仿->眠啊辆加己->d3d11.lib 眠啊
//2锅规过
#pragma comment ( lib, "d3d11.lib")

class TDevice : public TWindow
{
public:
	//且寸, 积己
	ID3D11Device*			m_pd3dDevice = nullptr;
	//包府, 款康
	ID3D11DeviceContext*	m_pContext = nullptr;

	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;

	D3D11_VIEWPORT			m_ViewPort;
public:
	bool			CreateDevice(HWND hWnd);
	void			DeleteDevice();

	void            SetViewport();
};
