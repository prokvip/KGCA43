#pragma once
#include "TFbxLoader.h"
#include "TObjectMgr.h"
#include "TQuadtree.h"
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
	TKgcObject* m_pMesh = nullptr;
	TKgcObject* m_pAnimMatrix = nullptr;
	T::TVector3		vPos;
	T::TVector3		vScale;
	T::TVector3		vRotate;
	T::TMatrix		matWorld;
	T::TVector3	    vLook = { 0.0f,0.0f,-1.0f };
	T::TVector3	    vSide = { 1.0f,0.0f,0.0f };
	T::TVector3	    vUp = { 0.0f,1.0f,0.0f };
	float			m_fFrameAnimation = 0.0f;
	UINT			m_fStartFrame = 1.0f;
	UINT			m_fLastFrame = 1.0f;
	TKgcFileHeader	m_FileHeader;

	std::vector< TInstance>  m_InstanceData;
	ComPtr<ID3D11Buffer>	 m_pInstanceBuffer;
	void       Init();
	void       Frame();
	void       InstanceUpdate(int iIndex, T::TVector3 v);
public:
	void       SetAnimFrame(UINT s, UINT e);
	void       Render(TMap& map, TCamera& cam);
	TMapObject()
	{
		m_InstanceData.resize(10);
	}
};