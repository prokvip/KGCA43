#pragma once
#include "TCore.h"
#include "TQuadtree.h"
#include "TFbxLoader.h"
#include "TObjectMgr.h"
#include "TSelect.h"

struct TLightInfo
{
	//T::TVector4 m_vLightInfo; // x = num, y = 
	T::TVector4 m_vLightDir;	
	T::TVector4 m_vLightPos;
};
struct TMapObject
{
	int				iObjectType = 0;
	TObject*		pObject = nullptr;
	T::TVector3		vPos;
	T::TVector3		vScale;
	T::TVector3		vRotate;
	T::TMatrix		matWorld;
	T::TVector3	    vLook = { 0.0f,0.0f,1.0f };
	T::TVector3	    vSide = { 1.0f,0.0f,0.0f };
	T::TVector3	    vUp = { 0.0f,1.0f,0.0f };
	float			m_fFrameAnimation = 61.0f;
	TKgcFileHeader	m_FileHeader;	
	/*void     Frame()
	{
		T::TMatrix matAnim;
		for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
		{
			auto pModel = m_ChildModel[iChild];
			for (int iBone = 0; iBone < pModel->m_pFbxNodeBindPoseMatrixList.size(); iBone++)
			{
				matAnim = pModel->m_pFbxNodeBindPoseMatrixList[iBone] *
					m_pBoneAnimMatrix[iBone][m_fFrameAnimation];
				D3DXMatrixTranspose(&pModel->m_matBoneList.matBone[iBone], &matAnim);
			}
			TDevice::m_pContext->UpdateSubresource(pModel->m_pBoneCB.Get(),
				0, NULL, &pModel->m_matBoneList, 0, 0);
		}
	}*/
};

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
class Sample : public TCore
{	
public:
	TRenderTarget	m_TopViewRT;
	TCamera			m_TopViewCamera;
public:
	TSelect		m_Select;
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
};