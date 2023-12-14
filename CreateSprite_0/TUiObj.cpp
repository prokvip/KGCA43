#include "TUiObj.h"

bool TUiNumber::Load(std::wstring texFileName)
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