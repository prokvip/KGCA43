#include "THeroObj.h"
#include "TInput.h"
bool    THeroObj::Create(TInitSet info, W_STR texFileName)
{
    m_InitSet = info;
    m_csName = info.name;    
    m_vPos = info.p;
    m_fSpeed = info.fMaxSpeed*0.5f;
    m_csDefaultVSFileName = L"../../data/shader/object.txt";
    m_csDefaultPSFileName = L"../../data/shader/object.txt";
    m_ptexMask = TTextureMgr::Get().Load(info.szMaskTex);
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
void   THeroObj::SetPosion(TVector2 p)
{
    m_VertexList[0].pos = TVector3(p.x, p.y, 0.5f);
    m_VertexList[1].pos = TVector3(p.x + m_InitSet.size.x, p.y, 0.5f);
    m_VertexList[2].pos = TVector3(p.x + m_InitSet.size.x, p.y + m_InitSet.size.y, 0.5f);
    m_VertexList[3].pos = TVector3(p.x, p.y + m_InitSet.size.y, 0.5f);
    m_pd3dContext->UpdateSubresource(m_pVertexBuffer,
        0,
        nullptr,
        &m_VertexList.at(0),
        0,
        0);
}
void   THeroObj::SetPosion(float x, float y)
{

}
bool THeroObj::Frame()
{
    //  위치 = 현 위치 + 이동방향 * 시간의 동기화( 1초당 최대 거리 이동)
    //  g_fSecPerFrame * m_fSpeed;
    //  1프레임  -> 1초 * 300     -> 300
    //  10프레임 -> 0.1초 * 300   -> 300
    //  100프레임 -> 0.01초 *300  -> 300
    m_fSpeed -= g_fSecPerFrame* m_fSpeed*0.9f;
    m_fSpeed = max(0.0f, m_fSpeed);
    m_fSpeed = min(m_InitSet.fMaxSpeed, m_fSpeed);
    if (TInput::Get().m_dwKeyState['W'] == KEY_HOLD)
    {
        m_fSpeed = max(m_InitSet.fMaxSpeed * 0.5f, m_fSpeed);
        m_fSpeed += g_fSecPerFrame * m_fSpeed;
        m_vDirection += TVector2(0, -1.0f);
    }       
    if (TInput::Get().m_dwKeyState['S'] == KEY_HOLD)
    {
        m_fSpeed = max(m_InitSet.fMaxSpeed * 0.5f, m_fSpeed);
        m_fSpeed += g_fSecPerFrame * m_fSpeed;
        m_vDirection += TVector2(0, 1.0f);
    }   
    if (TInput::Get().m_dwKeyState['A'] == KEY_HOLD)
    {
        m_fSpeed = max(m_InitSet.fMaxSpeed * 0.5f, m_fSpeed);
        m_fSpeed += g_fSecPerFrame * m_fSpeed;
        m_vDirection += TVector2(-1, 0.0f);
    }  
    if (TInput::Get().m_dwKeyState['D'] == KEY_HOLD)
    {
        m_fSpeed = max(m_InitSet.fMaxSpeed * 0.5f, m_fSpeed);
        m_fSpeed += g_fSecPerFrame * m_fSpeed;
        m_vDirection += TVector2(1, 0.0f);
    }
    if (TInput::Get().m_dwKeyState[VK_LBUTTON] == KEY_UP)
    {

    }
    if (TInput::Get().m_dwKeyState[VK_RBUTTON] == KEY_UP)
    {
    }
    
    m_vDirection.Normalized();
    m_vVelocity = m_vDirection *g_fSecPerFrame* m_fSpeed;
    m_vPos = m_vPos + m_vVelocity;
    SetPosion(m_vPos);
    return true;
}

bool THeroObj::PreRender()
{
    if (m_ptexMask != nullptr)
    {
        m_pd3dContext->PSSetShaderResources(1, 1, &m_ptexMask->m_pTextureSRV);
    }
    return TPlaneShape::PreRender();
}
THeroObj::THeroObj()
{
    m_fAccel = 0.0f;
}