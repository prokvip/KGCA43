#pragma once
#include <d3d11.h>
#include <d3dcompiler.h> 
#include <wrl.h> // windows runtime c++ template library
//../../dxtk/include  기본경로로 설정한다. (속성)
//1번방법 : 속성->링커->입력->추가종속성->d3d11.lib 추가
//2번방법
#pragma comment ( lib, "d3d11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
using namespace Microsoft::WRL;


class TRenderTarget
{
public:
	UINT			m_iWidthVP = 0;
	UINT			m_iHeightVP = 0;
	// backbuffer -> Set RT
	ComPtr<ID3D11Texture2D> m_pTexRT = nullptr;
	ComPtr<ID3D11RenderTargetView>	m_pRTV = nullptr;
	ComPtr<ID3D11ShaderResourceView>	m_pSRV_RT = nullptr;

	ComPtr<ID3D11Texture2D> m_pTexDS = nullptr;
	ComPtr<ID3D11DepthStencilView>  m_pDSV = nullptr;
	D3D11_VIEWPORT					m_ViewPort;
	DXGI_SWAP_CHAIN_DESC			m_SwapChainDesc;
	HRESULT CreateDepthBuffer(UINT iWidth, UINT iHeight);
	void	SetViewport(UINT iWidth, UINT iHeight);
	void    DeleteDevice();
	bool	CreateRTV(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain);
	virtual void  Begin();
	virtual void  End();
	// Create Texture - > Set RT
	ComPtr<ID3D11Texture2D>  m_pTextureRT;
	bool	Create(ID3D11Device* pDevice, UINT iWidth, UINT iHeight);
	ID3D11Texture2D*	CreateTexture(ID3D11Device* pDevice, UINT iWidth, UINT iHeight );
	bool	CreateRTV(ID3D11Device* pDevice, ID3D11Texture2D* pTex);
	void	Release();
};

