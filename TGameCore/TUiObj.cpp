#include "TUiObj.h"
#include "TInput.h"
bool    TUiObj::Create(W_STR name, T_STR_VECTOR texFileName)
{
    m_csName = name;
    m_rtRect.left = m_VertexList[0].pos.x;
    m_rtRect.top = m_VertexList[0].pos.y;
    m_rtRect.right = m_VertexList[2].pos.x;
    m_rtRect.bottom = m_VertexList[2].pos.y;

    m_uiState = T_CONTROL_STATE::T_STATE_NORMAL;
    LoadTexture(texFileName);
    return true;
}
bool    TUiObj::Create(W_STR name, W_STR texFileName)
{
    m_csName = name;
    // p0, w, h
    // p0, p1, p2, p3
    m_rtRect.left = m_VertexList[0].pos.x;
    m_rtRect.top =  m_VertexList[0].pos.y;
    m_rtRect.right = m_VertexList[2].pos.x;
    m_rtRect.bottom = m_VertexList[2].pos.y;

    m_uiState = T_CONTROL_STATE::T_STATE_NORMAL;
    LoadTexture(texFileName);
    return true;
}
bool    TUiObj::Init() {
    return true;
};
bool    TUiObj::Load(std::wstring LoadFileName) {
    return true;
};
bool TUiObj::LoadTexture(T_STR_VECTOR texArray)
{
    for (int i = 0; i < texArray.size(); i++)
    {
        m_pTexArray.push_back(TTextureMgr::Get().Load(texArray[i]));
    }
    m_ptTex = m_pTexArray[m_uiState];
    return true;
}
bool    TUiObj::LoadTexture(std::wstring texFileName) {
    m_ptTex = TTextureMgr::Get().Load(texFileName);
    if (m_ptTex != nullptr)
        return true;
    return false;
};
bool    TUiObj::PreRender() {
    return true;
};
bool    TUiObj::Frame(float fElapsTime) 
{    
    if (m_uiState == T_STATE_PRESSED && m_pTexArray.size() > 0)
    {
        if (TInput::Get().m_dwKeyState[VK_LBUTTON] == KEY_UP)
        {
            m_isSelected = TRUE;
            MessageBox(NULL, m_csName.c_str(), L"Selected!", MB_OK);
            return true;
        }
    }
    m_uiState = T_STATE_NORMAL;
    POINT tpMouse = TInput::Get().m_ptMouse;
    if (m_rtRect.left <= tpMouse.x &&  m_rtRect.right >= tpMouse.x)
    {
        if (m_rtRect.top <= tpMouse.y && m_rtRect.bottom >= tpMouse.y)
        {
            m_uiState = T_STATE_MOUSEOVER;     
            if (TInput::Get().m_dwKeyState[VK_LBUTTON] == KEY_HOLD)
            {
                m_uiState = T_STATE_PRESSED;
            }
        }
    }   
    if (m_pTexArray.size() > 0)
    {
        m_ptTex = m_pTexArray[m_uiState];
    }
    return true;
};
bool    TUiObj::Render(ID3D11DeviceContext* pd3dContext) {
    pd3dContext->PSSetShaderResources(0, 1, &m_ptTex->m_pTextureSRV);
    return true;
};
bool    TUiObj::PostRender() { return true; };
bool    TUiObj::Release() { return true; };




/// <summary>
/// 
/// </summary>
/// <param name="texFileName"></param>
/// <returns></returns>
bool    TUiNumber::Create(W_STR name, T_STR_VECTOR texArray)
{
    LoadTexture(texArray);
    return true;
}
bool    TUiNumber::Create(W_STR name)
{
    LoadTexture();
    return true;
}
bool TUiNumber::LoadTexture(T_STR_VECTOR texArray)
{
    TUiObj::LoadTexture(texArray);

    m_iApplyIndex = 0;
    m_fTimer = 0.0f;
    m_fAnimationTime = 10.0f;
    m_fStepTime = m_fAnimationTime / m_pTexArray.size();
    return true;
}
bool TUiNumber::LoadTexture()
{   
    for (int i = 0; i < 10; i++)
    {
        std::wstring filename = L"../../data/";
        filename += std::to_wstring(i);
        filename += L".png";
        m_pTexArray.push_back(TTextureMgr::Get().Load(filename));
    }

    m_iApplyIndex = 0;
    m_fTimer = 0.0f;
    m_fAnimationTime = 10.0f;
    m_fStepTime = m_fAnimationTime / m_pTexArray.size();
    return true;
}
bool    TUiNumber::Frame(float fElapsTime)
{
    if (m_fAnimationTime <= 0.0f)
    {
        //m_bStop = true;
         //return true;
        m_iApplyIndex = 0;
        m_fAnimationTime = 10.0f; 
        return true;
    }
    m_fAnimationTime -= fElapsTime;
    m_fTimer += fElapsTime;
    if (m_fTimer >= m_fStepTime)
    {
        m_iApplyIndex++;
        m_iApplyIndex = min(m_pTexArray.size()-1, m_iApplyIndex);
        m_fTimer -= m_fStepTime;
    }
    return true;
}
bool    TUiNumber::Render(ID3D11DeviceContext* pd3dContext) 
{
    pd3dContext->PSSetShaderResources(0, 1, &m_pTexArray[m_iApplyIndex]->m_pTextureSRV);
    return true;
};

bool    TUiNumber::Release()
{
    TUiObj::Release();
    return true;
}