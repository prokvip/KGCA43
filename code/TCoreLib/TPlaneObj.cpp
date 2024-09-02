#include "TPlaneObj.h"

void   TPlaneObj::SetVertexData()
{
	//local
	m_vVertexList.resize(4);
	// 0,1,2    2,3,0
	// 0     1

	// 3     2
	m_vVertexList[0].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vVertexList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[0].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[0].t = T_Math::FVector2(0.0f, 0.0f);

	m_vVertexList[1].p = T_Math::FVector3(1.0f, 1.0f, 0.0f);
	m_vVertexList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[1].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[1].t = T_Math::FVector2(1.0f, 0.0f);

	m_vVertexList[2].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vVertexList[2].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[2].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[2].t = T_Math::FVector2(1.0f, 1.0f);

	m_vVertexList[3].p = T_Math::FVector3(-1.0f, -1.0f, 0.0f);
	m_vVertexList[3].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[3].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[3].t = T_Math::FVector2(0.0f, 1.0f);
}
void   TPlaneObj::SetIndexData()
{
	m_vIndexList.resize(6);
	// 0,1,2    2,3,0
	m_vIndexList[0] = 0; m_vIndexList[1] = 1; m_vIndexList[2] = 2;
	m_vIndexList[3] = 2; m_vIndexList[4] = 3; m_vIndexList[5] = 0;
}
void   TPlaneObj::Frame() 
{
	T::TMatrix  matScale, matTrans, matRotate;
	T::D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	T::D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	T::D3DXMatrixRotationY(&matRotate, g_fGameTime);
	m_matWorld = matScale * matRotate * matTrans;
}