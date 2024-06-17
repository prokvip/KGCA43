#include "TActor.h"
void    TActor::Frame()
{
	m_matWorld = m_matCenter * m_matScale * m_matRotate * m_matTrans;
	SetWorldTransform();
	m_vOffset = { 0.0f,0.0f };
}
void    TActor::SetTrans(T_Math::FVector2& p)
{
	m_matTrans.Translation(p);
}
void    TActor::SetScale(T_Math::FVector2& s)
{
	m_matScale.Scale(s);
}
void    TActor::SetRotate(float r)
{
	m_matRotate.rotate(r);
}
void    TActor::SetCenterMove(T_Math::FVector2& p)
{	
	m_matCenter.Translation(p);	
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
void TActor::SetWorldMatrix(T_Math::FMatrix& m)
{
	m_matWorld = m;
}
void TActor::SetViewTransform(T_Math::FMatrix& matCamera)
{	
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vList[iv].p * matCamera;
	}	
}
TActor& TActor::SetWorldTransform()
{
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vListScreen[iv].p * m_matWorld;
	}
	return *this;
}
TActor& TActor::SetWorldTransform(T_Math::FMatrix& m)
{
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vListScreen[iv].p * m;
	}
	return *this;
}
void   TActor::SetVertexData(RECT rt)
{
	TDxObject::SetVertexData(rt);

	// 사각형의 센터를 포지션으로	
	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;

	T_Math::FVector2 vCenter = { -m_vPos.X, -m_vPos.Y };
	// 초기 배치의 정점버퍼 각 정점의 위치를 중점으로 한다.
	SetCenterMove(vCenter);
	SetTrans(m_vPos);
	m_rt = rt;
}
TActor& TActor::Move(float dx, float dy)
{
	Move({dx,dy});
	//// 화면좌표계
	//for (auto& v : m_vListScreen)
	//{
	//	v.p += { dx, dy };
	//}
	//m_vPos = { dx,dy };

	//UpdateVertexBuffer();
	return *this;
}
TActor& TActor::Move(T_Math::FVector2 d)
{
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos += vOffset;
	m_vOffset += vOffset;

	SetTrans(m_vPos);

	m_rt.left	= m_vList[0].p.X;
	m_rt.right	= m_vList[1].p.X;
	m_rt.top	= m_vList[0].p.Y;
	m_rt.bottom = m_vList[2].p.Y;
	return *this;
}