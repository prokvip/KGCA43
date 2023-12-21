#include "THeroObj.h"
bool    THeroObj::Create(TInitSet info, W_STR texFileName)
{
    m_InitSet = info;
    m_csName = info.name;    
    TPlaneShape::Create(m_csName, texFileName);
    return true;
}
bool THeroObj::CreateVertexBuffer()
{
    SetVB(m_InitSet.p, m_InitSet.uvPos, m_InitSet.size, 
        { (float)m_ptTex->m_tex->GetMetadata().width,
          (float)m_ptTex->m_tex->GetMetadata().height }
        );
    TPlaneShape::CreateVertexBuffer();
    return true;
}
bool   THeroObj::SetVB(TVector2 p,
	TVector2 uvPos,
	TVector2 size,
	TVector2 texSize)
{
    m_VertexList.clear();
    float fU0 = uvPos.x / texSize.x;
    float fV0 = uvPos.y / texSize.y;
    float fW = fU0 + size.x / texSize.x;
    float fH = fV0 + size.y / texSize.y;
    TVector2 uv0 = { fU0, fV0 };
    TVector2 uv1 = { fW, fV0 };
    TVector2 uv2 = { fW, fH };
    TVector2 uv3 = { fU0, fH };
    m_VertexList.emplace_back(TVector3(p.x, p.y, 0.5f), TVector4(1, 1, 1, 1), uv0);      // 0
    m_VertexList.emplace_back(TVector3(p.x + size.x, p.y, 0.5f), TVector4(1, 1, 1, 1), uv1);    // 1
    m_VertexList.emplace_back(TVector3(p.x + size.x, p.y + size.y, 0.5f), TVector4(1, 1, 1, 1), uv2);  // 2
    m_VertexList.emplace_back(TVector3(p.x, p.y + size.y, 0.5f), TVector4(1, 1, 1, 1), uv3);    // 3

	return true;
}