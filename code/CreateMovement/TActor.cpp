#include "TActor.h"
void   TActor::SetVertexData(RECT rt)
{
	TDxObject::SetVertexData(rt);

	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;
	m_rt = rt;
}
TActor& TActor::Move(float dx, float dy)
{
	// 화면좌표계
	for (auto& v : m_vList)
	{
		v.p += { dx, dy };
	}
	m_vPos = { dx,dy };

	// NDC 좌표계 변환
	for (int i = 0; i < m_vList.size(); i++)
	{
		m_vListNDC[i].p = ConvertScreenToNDC(m_vList[i].p);
	}
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	return *this;
}