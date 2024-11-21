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
	TKgcObject*		m_pAnimMesh = nullptr;
	TKgcObject*		m_pAnimMatrix  = nullptr;
	T::TVector3		vPos;
	T::TVector3		vScale;
	T::TVector3		vRotate;
	T::TMatrix		matWorld;
	T::TVector3	    vLook = { 0.0f,0.0f,-1.0f };
	T::TVector3	    vSide = { 1.0f,0.0f,0.0f };
	T::TVector3	    vUp = { 0.0f,1.0f,0.0f };
	float			m_fFrameAnimation = 61.0f;
	UINT			m_fStartFrame = 1.0f;
	UINT			m_fLastFrame  = 1.0f;
	TKgcFileHeader	m_FileHeader;	
	void       SetAnimFrame(UINT s, UINT e)
	{
		if (m_fStartFrame != s && m_fLastFrame != e)
		{
			m_fStartFrame = s;
			m_fLastFrame = e;
			m_fFrameAnimation = s;
		}
	}

	void       Render(TMap& map, TCamera& cam)
	{
		T::TMatrix matScale, matBaseRotate, matRotate, matTrans;
		//D3DXMatrixScaling(&matScale,vScale.x,	vScale.y,	vScale.z);		
		//D3DXMatrixRotationY(&matBaseRotate, DegreeToRadian(180.0f));
		//D3DXMatrixRotationY(&matRotate, g_fGameTime * 0.5f);
		//D3DXVec3TransformCoord(&vPos, &vPos, &matRotate);
		
		vLook.x = -matWorld._31;
		vLook.y = -matWorld._32;
		vLook.z = -matWorld._33;
		D3DXVec3Normalize(&vLook, &vLook);
		
		// += vLook * g_fSecondPerFrame * 100.0f;
		vPos.y = map.GetHeight(vPos);
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		matWorld = matScale * matBaseRotate * matRotate * matTrans;

		float FrameSpeed = 30.0f;
		m_fFrameAnimation += g_fSecondPerFrame * FrameSpeed * 0.5f;
		if (m_fFrameAnimation > m_fLastFrame)
		{
			m_fFrameAnimation = m_fStartFrame;
		}

		if (m_pAnimMesh)
		{
			m_pAnimMesh->Get()->SetAnimFrame(m_fStartFrame, m_fLastFrame);
			TFbxModel* pAnim = m_pAnimMatrix->Get().get();
			m_pAnimMesh->Get()->AnimFrame(m_fFrameAnimation, pAnim);
			m_pAnimMesh->Get()->m_matParentWorld = matWorld;
			m_pAnimMesh->Get()->SetMatrix(nullptr, &cam.m_matView, &cam.m_matProj);
			m_pAnimMesh->Get()->Render(TDevice::m_pContext);
		}
	}	
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