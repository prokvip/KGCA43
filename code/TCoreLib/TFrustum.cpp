#include "TFrustum.h"
void TFrustum::SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
{
	TDxObject3D::SetMatrix(pWorld, pView, pProj);
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

		fDistance = m_Plane[p].a * vAxisX.x +
					m_Plane[p].b * vAxisX.y +
					m_Plane[p].c * vAxisX.z;
		fDistance += m_Plane[p].a * vAxisY.x +
					m_Plane[p].b * vAxisY.y +
					m_Plane[p].c * vAxisY.z;
		fDistance += m_Plane[p].a * vAxisZ.x +
					m_Plane[p].b * vAxisZ.y +
					m_Plane[p].c * vAxisZ.z;
		if (fPlaneToCenter < -fDistance) return false;
	}
	return true;
};
bool TFrustum::FrustumInSphere(T_Sphere s)
{
	return true;
}