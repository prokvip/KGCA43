#include "TActor.h"
void    TActor::Frame()
{
	Transform(m_matWorld);
}
void    TActor::SetPos(T_Math::FVector2& p)
{
	m_vPos = p;
	m_matWorld._31 = m_vPos.X;
	m_matWorld._32 = m_vPos.Y;	
}
void    TActor::SetPos(float& dx, float& dy)
{
	m_vPos.X = dx;
	m_vPos.Y = dx;
}
void TActor::SetWorld(T_Math::FMatrix& m)
{
	m_matWorld = m;
}
TActor& TActor::Transform(T_Math::FMatrix& m)
{	
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vListScreen[iv].p * m;
	}
	UpdateVertexBuffer();
	return *this;
}
TActor& TActor::Trans(T_Math::FMatrix& m)
{
	//for (int iv = 0; iv < m_vListScreen.size(); iv++)
	//{
	//	m_vListScreen[iv].p = m_vListScreen[iv].p * m;		
	//}
	//UpdateVertexBuffer();
	return *this;
}
void   TActor::SetVertexData(RECT rt)
{
	TDxObject::SetVertexData(rt);

	// 사각형의 센터를 포지션으로
	//m_vPos.X = (rt.left + rt.right) * 0.5f;
	//m_vPos.Y = (rt.bottom + rt.top) * 0.5f;
	// 왼쪽 상단 모서리를 포지션으로
	m_vPos.X = rt.left;
	m_vPos.Y = rt.top;

	m_rt = rt;
}
TActor& TActor::Move(float dx, float dy)
{
	// 화면좌표계
	for (auto& v : m_vListScreen)
	{
		v.p += { dx, dy };
	}
	m_vPos = { dx,dy };

	UpdateVertexBuffer();
	return *this;
}
TActor& TActor::Move(T_Math::FVector2 d)
{
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos += vOffset;
	m_vOffset += vOffset;

	float halfW = (m_rt.right - m_rt.left) * 0.5f;
	float halfH = (m_rt.bottom - m_rt.top) * 0.5f;

	m_rt.left = m_vListScreen[0].p.X;
	m_rt.right = m_vListScreen[1].p.X;
	m_rt.top = m_vListScreen[0].p.Y;
	m_rt.bottom = m_vListScreen[2].p.Y;
	return *this;
}