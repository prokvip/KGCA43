#include "TUiObj.h"
bool    TUiObj::Create(W_STR name, T_STR_VECTOR texFileName)
{
    m_csName = name;
    LoadTexture(texFileName);
    return true;
}
bool    TUiObj::Create(W_STR name, W_STR texFileName)
{
    m_csName = name;
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
bool    TUiObj::Frame(float fElapsTime) {
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