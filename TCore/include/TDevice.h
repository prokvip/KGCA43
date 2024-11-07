#pragma once
#include "TRenderTarget.h"



class TDevice 
{
public:
	static ComPtr<ID3D11Device>		m_pd3dDevice;
	static ID3D11DeviceContext*		m_pContext;
	static IDXGISwapChain*			m_pSwapChain;

	static TRenderTarget			m_RT;
	/*static ID3D11RenderTargetView*	m_pRTV ;

	static D3D11_VIEWPORT			m_ViewPort;*/

	static DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;
public:
	static bool			CreateDevice(HWND hWnd);
	static void			DeleteDevice();
	static void         SetViewport(UINT iWidth, UINT iHeight);
	static bool			CreateRTV();
};
