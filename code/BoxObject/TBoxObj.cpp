#include "TBoxObj.h"
void   TBoxObj::SetVertexData()
{
	//local
	m_vVertexList.resize(6);
	// 0,5    1

	// 4     2,3
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


	m_vVertexList[3].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vVertexList[3].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[3].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[3].t = T_Math::FVector2(1.0f, 1.0f);

	m_vVertexList[4].p = T_Math::FVector3(-1.0f, -1.0f, 0.0f);
	m_vVertexList[4].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[4].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[4].t = T_Math::FVector2(0.0f, 1.0f);

	m_vVertexList[5].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vVertexList[5].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[5].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[5].t = T_Math::FVector2(0.0f, 0.0f);

}
void   TBoxObj::SetIndexData()
{
	m_vIndexList.resize(6);
	// 0,1,2    2,3,0
	m_vIndexList[0] = 0; m_vIndexList[1] = 1; m_vIndexList[2] = 2;
	m_vIndexList[3] = 2; m_vIndexList[4] = 3; m_vIndexList[5] = 0;
}