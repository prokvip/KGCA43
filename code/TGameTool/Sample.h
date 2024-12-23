#pragma once
#include "TCore.h"
#include "TCharacterObj.h"


class TMiniMap : public TPlaneObj
{
public:
	D3D11_VIEWPORT					    m_ViewPort;
	ComPtr<ID3D11ShaderResourceView>	m_pSRV_RT;
	TBoxObj								m_FrustumObj;
public:
	void   Render(ID3D11DeviceContext* pContext) override
	{
		/*D3D11_VIEWPORT vp;
		UINT    iNumVp;
		TDevice::m_pContext->RSGetViewports(&iNumVp, &vp);*/
			TDevice::m_pContext->RSSetViewports(
				1, &m_ViewPort);
			PreRender(pContext);
			TDevice::m_pContext->PSSetShaderResources(0,
				1, m_pSRV_RT.GetAddressOf());
			PostRender(pContext);
		//TDevice::m_pContext->RSSetViewports(iNumVp, &vp);
			TDevice::m_pContext->RSSetViewports(1, 
				&TDevice::m_RT.m_ViewPort);

	}
	TMiniMap()
	{
		m_ViewPort.TopLeftX = 0;
		m_ViewPort.TopLeftY = 0;
		m_ViewPort.Width = 300;
		m_ViewPort.Height = 300;
		m_ViewPort.MinDepth = 0;
		m_ViewPort.MaxDepth = 1;
		
	}
};

struct PickData
{
	T::TVector3 vPickPos;
	float	    fRaidus;
	int			iIndex;
	float		fTexWidth;
	float		fTexHeight;
};
class Sample : public TCore
{	
public:
	TRenderTarget	m_TopViewRT;
	TCamera			m_TopViewCamera;
	
public:
	TSelect		m_Select;
	TTexture*	m_pTexSplatting[4];
	TMap		m_Map;
	TQuadtree   m_Quadtree;
	TMiniMap	m_MiniMapObj;
	using tModel = std::shared_ptr<TObject>;
	std::vector<tModel>			m_pFbxfileList;
	std::vector<TMapObject>		m_pMapObjectList;
	std::vector<std::wstring>   m_LoadFiles;
public:
	TLightInfo  m_LightInfo;
	ComPtr<ID3D11Buffer>  m_pConstantBufferLight;	
public:
	void   Init() override;
	void   PreFrame() override;
	void   Frame() override;
	void   PostFrame() override;
	void   Render() override;
	void   Release() override;
	void   LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave);
	void   SetObject(T::TVector3 vPos);
	void   MapRender(TCamera* pCamera);
	void   MiniMapRander(ID3D11ShaderResourceView* pSRV);
	void   CreateTopViewRT();

public:
	UINT					m_iTextureWidth = 1024;
	UINT					m_iTextureHeight = 1024;
	void					Splatting();
	PickData				m_SelectPick;
	
	ComPtr<ID3D11Buffer>					m_pBuf0;
	ComPtr<ID3D11ShaderResourceView>		m_pBuf0SRV;
	ComPtr < ID3D11Texture2D >              m_pAlphaTexture;
	ComPtr<ID3D11ShaderResourceView>		m_pAlphaTexSRV;
	ComPtr<ID3D11UnorderedAccessView>		m_pAlphaTexUAV;

	ComPtr<ID3D11ComputeShader>  m_pCS;
	bool  CreateStructureBuffer(UINT uElementSize,
		UINT uCount,
		VOID* pInitData,
		ComPtr<ID3D11Buffer>& sBuffer);
	HRESULT  CreateAlphaTex(UINT iWidth, UINT iHeight);
	bool CreateBufferSRV(ComPtr<ID3D11Buffer>& pBuffer,
		ComPtr<ID3D11ShaderResourceView>& srv);
};