#include "TLineObj.h"
void   TLineObj::SetVertexData()
{
	//local
	m_vVertexList.resize(2);
	m_vVertexList[0].p = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[0].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[0].t = T_Math::FVector2(0.0f, 0.0f);

	m_vVertexList[1].p = T_Math::FVector3(1.0f, 0.0f, 0.0f);
	m_vVertexList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[1].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vVertexList[1].t = T_Math::FVector2(1.0f, 0.0f);
}
void   TLineObj::SetIndexData()
{
	m_vIndexList.resize(2);
	// 0,1,2    2,3,0
	m_vIndexList[0] = 0; m_vIndexList[1] = 1;
}
void TLineObj::Draw(FVector3 s, FVector3 e, FVector4 c)
{
	m_vVertexList[0].p = s;
	m_vVertexList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[0].c = c;
	m_vVertexList[0].t = T_Math::FVector2(0.0f, 0.0f);

	m_vVertexList[1].p = e;
	m_vVertexList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vVertexList[1].c = c;
	m_vVertexList[1].t = T_Math::FVector2(1.0f, 0.0f);

	TDevice::m_pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr,	&m_vVertexList.at(0), 0, 0);
	TDevice::m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	Render(TDevice::m_pContext);

	TDevice::m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}