#pragma once
#include "TWindow.h"
#include <d3d11.h>
class Sample : public TWindow
{
	ID3D11Device*			m_pd3dDevice = nullptr;
	ID3D11DeviceContext*	m_pd3dContext = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};