#include "TActor.h"
TActor& TActor::Trans(T_Math::FMatrix& m)
{
	for (int iv = 0; iv < m_vList.size(); iv++)
	{
		m_vList[iv].p = m_vList[iv].p * m;		
	}
	UpdateVertexBuffer();
	return *this;
}
void   TActor::SetVertexData(RECT rt)
{
	TDxObject::SetVertexData(rt);

	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;
	m_rt = rt;
}
TActor& TActor::Move(float dx, float dy)
{
	// È­¸éÁÂÇ¥°è
	for (auto& v : m_vList)
	{
		v.p += { dx, dy };
	}
	m_vPos = { dx,dy };

	UpdateVertexBuffer();
	return *this;
}