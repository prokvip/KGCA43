#include "TPlaneObj.h"

void   TPlaneObj::SetVertexData()
{
	//local
	m_vList.resize(6);
	// 0,5    1

	// 4     2,3
	m_vList[0].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[0].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[0].t = T_Math::FVector2(0.0f, 0.0f);

	m_vList[1].p = T_Math::FVector3(1.0f, 1.0f, 0.0f);
	m_vList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[1].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[1].t = T_Math::FVector2(1.0f, 0.0f);

	m_vList[2].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vList[2].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[2].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[2].t = T_Math::FVector2(1.0f, 1.0f);


	m_vList[3].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vList[3].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[3].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[3].t = T_Math::FVector2(1.0f, 1.0f);

	m_vList[4].p = T_Math::FVector3(-1.0f, -1.0f, 0.0f);
	m_vList[4].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[4].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[4].t = T_Math::FVector2(0.0f, 1.0f);

	m_vList[5].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vList[5].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[5].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[5].t = T_Math::FVector2(0.0f, 0.0f);

}

void   TPlaneObj::Frame() 
{
	/*static FVector3 vPos = { 0, 0, 0 };
	vPos.X = cosf(g_fGameTime) * 2.0f;*/
	
	FMatrix  matScale, matTrans, matRotate;
	matTrans.Translation(m_vPos);
	matScale.Scale(m_vScale);
	//matRotate.rotateZ(g_fGameTime);
	m_matWorld = matScale * matRotate * matTrans;	
}