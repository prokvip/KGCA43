#include "pch.h"
#include "TSelect.h"
void		TSelect::Init()
{
}
void		TSelect::Frame()
{
	//1) 클라이언트 좌표
	POINT ptCursor;
	GetCursorPos(&ptCursor);// screen
	ScreenToClient(g_hWnd, &ptCursor); 
	// 2) 뷰좌표 변환 = 뷰포트 역행렬 * 투영행렬 역행렬
	T::TVector3  v;
	v.x = (((2.0f * ptCursor.x) / g_xClientSize) - 1 ) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / g_yClientSize) - 1 ) / m_matProj._22;
	v.z = 1.0f;

	T::TMatrix matInverseView;
	D3DXMatrixInverse(&matInverseView, NULL, &m_matView);
	// 3)월드변환
	T::TVector3 vViewOrigin = T::TVector3(0.0f, 0.0f, 0.0f);
	T::TVector3 vViewDir = v;
	// w = 1  vOrigin = vViewOrigtn * matInverseView;
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &vViewOrigin, &matInverseView);
	// w= 0
	D3DXVec3TransformNormal(&m_Ray.vDirecton, &vViewDir, &matInverseView);
}

bool    TSelect::GetIntersection(
	T::TVector3 v0,
	T::TVector3 v1, 
	T::TVector3 v2,
	T::TVector3 n,
	T::TVector3 s,
	T::TVector3 e)
{
	T::TVector3 d1 = e - s;
	T::TVector3 d2 = v0 - s;
	float fRayDot = D3DXVec3Dot(&n,&d1);
	float fPlaneDot = D3DXVec3Dot(&n, &d2);
	float t = fPlaneDot / fRayDot;
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}
	m_vIntersection = s + d1 * t;
	return true;
}
bool		TSelect::PointInPolygon(T::TVector3 v,
	T::TVector3 n,
	T::TVector3 v0,
	T::TVector3 v1,
	T::TVector3 v2)
{
	// v0    v1
	//   v   
	// v2    
	T::TVector3 e0, e1,iInter, vNormal;
	e0 = v0 - v2;
	e1 = v - v2;
	D3DXVec3Cross(&vNormal, &e0, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&n, &vNormal);
	if (fDot < 0.0f) return false;

	e0 = v1 - v0;
	e1 = v - v0;
	D3DXVec3Cross(&vNormal, &e0, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&n, &vNormal);
	if (fDot < 0.0f) return false;

	e0 = v2 - v1;
	e1 = v - v1;
	D3DXVec3Cross(&vNormal, &e0, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&n, &vNormal);
	if (fDot < 0.0f) return false;
	return true;
}
void		TSelect::Render()
{
}
void		TSelect::Release()
{
}
void		TSelect::SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
{
	if( pWorld != nullptr)	m_matWorld = *pWorld;
	if (pView != nullptr)m_matView = *pView;
	if (pProj != nullptr)m_matProj = *pProj;
}
