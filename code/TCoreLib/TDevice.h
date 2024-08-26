#pragma once
#include <d3d11.h>
#include <d3dcompiler.h> 
#include <wrl.h> // windows runtime c++ template library
//../../dxtk/include  �⺻��η� �����Ѵ�. (�Ӽ�)
//1����� : �Ӽ�->��Ŀ->�Է�->�߰����Ӽ�->d3d11.lib �߰�
//2�����
#pragma comment ( lib, "d3d11.lib")
#pragma comment ( lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class TDevice 
{
public:
	static ComPtr<ID3D11Device>		m_pd3dDevice;
	static ID3D11DeviceContext*		m_pContext;
	static IDXGISwapChain*			m_pSwapChain;
	static ID3D11RenderTargetView*	m_pRTV ;

	static D3D11_VIEWPORT			m_ViewPort;

	static DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;
public:
	static bool			CreateDevice(HWND hWnd);
	static void			DeleteDevice();
	static void         SetViewport();
};
