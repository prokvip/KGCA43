#pragma once
#include "TWindow.h"
#include <d3d11.h> // DirectX 기능
//1번방법 : 속성->링커->입력->추가종속성->d3d11.lib 추가
//2번방법
#pragma comment ( lib, "d3d11.lib")

class TDevice : public TWindow
{
public:
	//인터페이스 획득(할당, 생성)
	ID3D11Device*			g_pd3dDevice = nullptr;
	ID3D11DeviceContext*	g_pContext = nullptr;
	IDXGISwapChain*			g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_pRTV = nullptr;
public:
	bool			CreateDevice(HWND hWnd);
	void			DeleteDevice();
	virtual void	GameRun() override;
};
