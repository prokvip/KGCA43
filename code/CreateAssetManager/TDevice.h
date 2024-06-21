#pragma once
#include <d3d11.h> // DirectX ���
#include <d3d11.h>
#include <d3dcompiler.h> 
#include <wrl.h> // windows runtime c++ template library
//1����� : �Ӽ�->��Ŀ->�Է�->�߰����Ӽ�->d3d11.lib �߰�
//2�����
#pragma comment ( lib, "d3d11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
using namespace Microsoft::WRL;

class TDevice 
{
public:
	//�Ҵ�, ����
	static ComPtr<ID3D11Device>				m_pd3dDevice;
	//����, �
	static ComPtr<ID3D11DeviceContext>		m_pContext;
	static ComPtr<IDXGISwapChain>			m_pSwapChain;
	static ComPtr<ID3D11RenderTargetView>	m_pRTV;
	static D3D11_VIEWPORT					m_ViewPort;
public:
	static bool			CreateDevice(HWND hWnd, UINT xSize, UINT ySize);
	static void			DeleteDevice();
	static void         SetViewport(UINT xSize, UINT ySize);
};
