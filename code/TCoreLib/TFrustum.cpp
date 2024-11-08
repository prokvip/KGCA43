#include "TFrustum.h"
void TFrustum::Set(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
{
	if( pWorld != nullptr)	m_matWorld = *pWorld;
	if (pView != nullptr) m_matView = *pView;
	if (pProj != nullptr) m_matProj = *pProj;
	T::TMatrix matVP = m_matView * m_matProj;
	T::TMatrix matInverseVP;
	D3DXMatrixInverse(&matInverseVP, NULL, &matVP);
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

	for (int v = 0; v < 8; v++)
	{
		//v = v * matInverseVP;
		D3DXVec3TransformCoord(&m_vFrustum[v], &m_vFrustum[v], &matInverseVP);
	}
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	m_Plane[0].Set(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
	m_Plane[1].Set(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
	m_Plane[2].Set(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
	m_Plane[3].Set(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
	m_Plane[4].Set(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
	m_Plane[5].Set(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
}
void TFrustum::Draw(T::TMatrix& matView, T::TMatrix& matProj)
{
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	m_Box.m_vVertexList[0].p = m_vFrustum[0];
	m_Box.m_vVertexList[1].p = m_vFrustum[1];
	m_Box.m_vVertexList[2].p = m_vFrustum[2];
	m_Box.m_vVertexList[3].p = m_vFrustum[3];
	// 뒷면
	m_Box.m_vVertexList[4].p = m_vFrustum[6];
	m_Box.m_vVertexList[5].p = m_vFrustum[5];
	m_Box.m_vVertexList[6].p = m_vFrustum[1];
	m_Box.m_vVertexList[7].p = m_vFrustum[3];

	// 오른쪽
	m_Box.m_vVertexList[8].p = m_vFrustum[2];
	m_Box.m_vVertexList[9].p = m_vFrustum[6];
	m_Box.m_vVertexList[10].p = m_vFrustum[7];
	m_Box.m_vVertexList[11].p = m_vFrustum[3];

	// 왼쪽
	m_Box.m_vVertexList[12].p = m_vFrustum[5];
	m_Box.m_vVertexList[13].p = m_vFrustum[1];
	m_Box.m_vVertexList[14].p = m_vFrustum[0];
	m_Box.m_vVertexList[15].p = m_vFrustum[1];

	// 윗면
	m_Box.m_vVertexList[16].p = m_vFrustum[5];
	m_Box.m_vVertexList[17].p = m_vFrustum[6];
	m_Box.m_vVertexList[18].p = m_vFrustum[2];
	m_Box.m_vVertexList[19].p = m_vFrustum[1];

	// 아랫면
	m_Box.m_vVertexList[20].p = m_vFrustum[0];
	m_Box.m_vVertexList[21].p = m_vFrustum[3];
	m_Box.m_vVertexList[22].p = m_vFrustum[7];
	m_Box.m_vVertexList[23].p = m_vFrustum[1];

	TDevice::m_pContext->UpdateSubresource(
		m_Box.m_pVertexBuffer, 0, NULL, &m_Box.m_vVertexList.at(0), 0, 0);

	m_Box.SetMatrix(nullptr, &matView, &matProj);
	m_Box.Render(TDevice::m_pContext);
}
bool TFrustum::FrustumInPoint(T::TVector3 v)
{
	for (int p = 0; p < 6; p++)
	{
		float fDistance = m_Plane[p].a * v.x +
			m_Plane[p].b * v.y +
			m_Plane[p].c * v.z + m_Plane[p].d;
		if (fDistance < 0.0) return false;
	}
	return true;
}
bool TFrustum::FrustumInBox(T_Box b)
{
	T::TVector3 vAxisX = b.vAxis[0] * b.fDistance[0];
	T::TVector3 vAxisY = b.vAxis[1] * b.fDistance[1];
	T::TVector3 vAxisZ = b.vAxis[2] * b.fDistance[2];
	float fDistance = 0.0f;

	for (int p = 0; p < 6; p++)
	{
		float fPlaneToCenter = m_Plane[p].a * b.vCenter.x +
			m_Plane[p].b * b.vCenter.y +
			m_Plane[p].c * b.vCenter.z + m_Plane[p].d;

		fDistance = fabs(m_Plane[p].a * vAxisX.x +
					m_Plane[p].b * vAxisX.y +
					m_Plane[p].c * vAxisX.z);
		fDistance += fabs(m_Plane[p].a * vAxisY.x +
					m_Plane[p].b * vAxisY.y +
					m_Plane[p].c * vAxisY.z);
		fDistance += fabs(m_Plane[p].a * vAxisZ.x +
					m_Plane[p].b * vAxisZ.y +
					m_Plane[p].c * vAxisZ.z);
		if (fPlaneToCenter < -fDistance) return false;
	}
	return true;
};
bool TFrustum::FrustumInSphere(T_Sphere s)
{
	return true;
}