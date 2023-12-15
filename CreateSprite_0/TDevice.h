#pragma once
#include "TWindow.h"

class TDevice : public TWindow
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pd3dContext = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
public:
	virtual bool    Init() {
		return true;
	};
	virtual bool    Frame() {
		return true;
	};
	virtual bool    Render() {
		return true;
	};
	virtual bool    Release() {
		return true;
	};
	virtual bool	GameInit() override;
	virtual bool	GameFrame() override;
	virtual bool	GameRender()override;
	virtual bool	GameRelease()override;
};

