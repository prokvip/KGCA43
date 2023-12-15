#include "TEffect.h"
#include "TInput.h"
bool    TEffect::Create(W_STR name, T_STR_VECTOR texFileName)
{
    m_csName = name;
    m_rtRect.left = m_VertexList[0].pos.x;
    m_rtRect.top = m_VertexList[0].pos.y;
    m_rtRect.right = m_VertexList[2].pos.x;
    m_rtRect.bottom = m_VertexList[2].pos.y;
    //m_uiState = T_CONTROL_STATE::T_STATE_NORMAL;
    LoadTexture(texFileName);
    return true;
}
bool    TEffect::Create(W_STR name, 
    W_STR texFileName, UINT iNumWidth, UINT iNumHeight, float fAnimTime)
{
    m_csName = name;
    // p0, w, h
    // p0, p1, p2, p3
    m_rtRect.left = m_VertexList[0].pos.x;
    m_rtRect.top = m_VertexList[0].pos.y;
    m_rtRect.right = m_VertexList[2].pos.x;
    m_rtRect.bottom = m_VertexList[2].pos.y;

    m_iNumWidth = iNumWidth;
    m_iNumHeight= iNumHeight;
    float fWidthStep = 1.0f / iNumWidth;
    float fHeightStep = 1.0f / iNumHeight;
    for (int y = 0; y < iNumHeight; y++)
    {
        for (int x = 0; x < iNumWidth; x++)
        {
            tRECT tRt;
            tRt.top = y * fHeightStep;
            tRt.left = x * fWidthStep;
            tRt.right = tRt.left + fWidthStep;
            tRt.bottom = tRt.top + fHeightStep;
            m_pUVArray.push_back(tRt);
        }
    }

    m_iApplyIndex = 0;
    m_fTimer = 0.0f;
    m_fInitAnimationTime = m_fAnimationTime = fAnimTime;
    m_fStepTime = m_fAnimationTime / m_pUVArray.size();

    m_uvIndex = m_pUVArray[m_iApplyIndex];
    m_VertexList[0].tex = TVector2(m_uvIndex.left, m_uvIndex.top);      // 0
    m_VertexList[1].tex = TVector2(m_uvIndex.right, m_uvIndex.top);    // 1
    m_VertexList[2].tex = TVector2(m_uvIndex.right, m_uvIndex.bottom);  // 2
    m_VertexList[3].tex = TVector2(m_uvIndex.left, m_uvIndex.bottom);    // 3


    //m_uiState = T_CONTROL_STATE::T_STATE_NORMAL;
    LoadTexture(texFileName);
    return true;
}
bool    TEffect::Init() {
    return true;
};

bool TEffect::LoadTexture(T_STR_VECTOR texArray)
{
    for (int i = 0; i < texArray.size(); i++)
    {
        m_pTexArray.push_back(TTextureMgr::Get().Load(texArray[i]));
    }
    //m_ptTex = m_pTexArray[m_uiState];
    return true;
}
bool    TEffect::LoadTexture(std::wstring texFileName) {
    m_ptTex = TTextureMgr::Get().Load(texFileName);
    if (m_ptTex != nullptr)
        return true;
    return false;
};
bool    TEffect::PreRender() {
    return true;
};
bool    TEffect::Frame(float fElapsTime)
{
    if (m_fAnimationTime <= 0.0f)
    {
        //m_bStop = true;
         //return true;
        m_iApplyIndex = 0;
        m_fAnimationTime = m_fInitAnimationTime;
        return true;
    }
    m_fAnimationTime -= fElapsTime;
    m_fTimer += fElapsTime;
    if (m_fTimer >= m_fStepTime)
    {
        m_iApplyIndex++;
        m_iApplyIndex = min(m_pUVArray.size() - 1, m_iApplyIndex);
        m_fTimer -= m_fStepTime;

        m_uvIndex = m_pUVArray[m_iApplyIndex];
        m_VertexList[0].tex = TVector2(m_uvIndex.left, m_uvIndex.top);      // 0
        m_VertexList[1].tex = TVector2(m_uvIndex.right, m_uvIndex.top);    // 1
        m_VertexList[2].tex = TVector2(m_uvIndex.right, m_uvIndex.bottom);  // 2
        m_VertexList[3].tex = TVector2(m_uvIndex.left, m_uvIndex.bottom);    // 3
    }
    return true;
}
bool    TEffect::Render(ID3D11DeviceContext* pd3dContext) {
    if (m_ptTex == nullptr) return true;
    pd3dContext->PSSetShaderResources(0, 1, &m_ptTex->m_pTextureSRV);
    return true;
};
bool    TEffect::PostRender() { return true; };
bool    TEffect::Release() { return true; };

