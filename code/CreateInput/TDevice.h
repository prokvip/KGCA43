#pragma once
#include "TWindow.h"
#include <d3d11.h> // DirectX ���
//1����� : �Ӽ�->��Ŀ->�Է�->�߰����Ӽ�->d3d11.lib �߰�
//2�����
#pragma comment ( lib, "d3d11.lib")

class TDevice : public TWindow
{
public:
	//�������̽� ȹ��(�Ҵ�, ����)
	ID3D11Device*			g_pd3dDevice = nullptr;
	ID3D11DeviceContext*	g_pContext = nullptr;
	IDXGISwapChain*			g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_pRTV = nullptr;
public:
	bool			CreateDevice(HWND hWnd);
	void			DeleteDevice();
	virtual void	GameRun() override;
};
