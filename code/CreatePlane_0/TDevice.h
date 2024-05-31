#pragma once
#include "TWindow.h"
#include <d3d11.h> // DirectX ���
//1����� : �Ӽ�->��Ŀ->�Է�->�߰����Ӽ�->d3d11.lib �߰�
//2�����
#pragma comment ( lib, "d3d11.lib")

class TDevice : public TWindow
{
public:
	//�Ҵ�, ����
	ID3D11Device*			m_pd3dDevice = nullptr;
	//����, �
	ID3D11DeviceContext*	m_pContext = nullptr;

	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;

	D3D11_VIEWPORT			m_ViewPort;
public:
	bool			CreateDevice(HWND hWnd);
	void			DeleteDevice();

	void            SetViewport();
};
