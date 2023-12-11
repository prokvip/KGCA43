#include "Sample.h"
bool    Sample::Init()
{     
    m_bk.m_pd3dDevice = m_pd3dDevice;
    m_bk.m_pd3dContext = m_pd3dContext;
    m_bk.m_rtClient = m_rtClient;
    // v0       v1
//
// v3       v2
    m_bk.m_VertexList.emplace_back(TVector3(0.0f, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    m_bk.m_VertexList.emplace_back(TVector3(m_rtClient.right, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    m_bk.m_VertexList.emplace_back(TVector3(m_rtClient.right, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_bk.m_VertexList.emplace_back(TVector3(0.0f, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3

    m_bk.Init();
    m_bk.Load(L"bk.png");
   // v0       v1
//
// v3       v2
    m_SkillLayout.m_pd3dDevice = m_pd3dDevice;
    m_SkillLayout.m_pd3dContext = m_pd3dContext;
    m_SkillLayout.m_rtClient = m_rtClient;
 
    m_SkillLayout.m_VertexList.emplace_back(TVector3(18.0f, 477.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    m_SkillLayout.m_VertexList.emplace_back(TVector3(460, 477.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    m_SkillLayout.m_VertexList.emplace_back(TVector3(460, 600, 0.5f), TVector4(0, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_SkillLayout.m_VertexList.emplace_back(TVector3(18.0f, 600, 0.5f), TVector4(0, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3

    m_SkillLayout.Init();
    m_SkillLayout.Load(L"../../data/skill.png");
    // v0       v1
//
// v3       v2
    m_Skill2.m_pd3dDevice = m_pd3dDevice;
    m_Skill2.m_pd3dContext = m_pd3dContext;
    m_Skill2.m_rtClient = m_rtClient;

    m_Skill2.m_VertexList.emplace_back(TVector3(115.0f, 487.0f, 0.5f), TVector4(1, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    m_Skill2.m_VertexList.emplace_back(TVector3(178.0f, 487.0f, 0.5f), TVector4(1, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    m_Skill2.m_VertexList.emplace_back(TVector3(178.0f, 590, 0.5f), TVector4(1, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_Skill2.m_VertexList.emplace_back(TVector3(115.0f, 590, 0.5f), TVector4(1, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3

    m_Skill2.Init();
    if (!m_Skill2.Load(L"../../data/s2.png"))
    {
        return false;
    }
    m_Skill2.LoadTextureChange(L"../../data/s1.png");


    m_Number.m_pd3dDevice = m_pd3dDevice;
    m_Number.m_pd3dContext = m_pd3dContext;
    m_Number.m_rtClient = m_rtClient;

    m_Number.m_VertexList.emplace_back(TVector3(0, 0.0f, 0.5f), TVector4(1, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    m_Number.m_VertexList.emplace_back(TVector3(52.0f, 0.0f, 0.5f), TVector4(1, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    m_Number.m_VertexList.emplace_back(TVector3(52.0f, 68, 0.5f), TVector4(1, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_Number.m_VertexList.emplace_back(TVector3(0.0f, 68, 0.5f), TVector4(1, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3

    m_Number.Init();
    if (!m_Number.Load(L"../../data/0.png"))
    {
        return false;
    }    
    return true;
}
bool    Sample::Render() 
{   
    m_bk.Render();
    m_SkillLayout.Render();

    if(g_bChange)
        m_Skill2.Render();
    else
        m_Skill2.RenderChange();

  
    m_pd3dContext->PSSetShaderResources(0, 1, &m_Number.m_pTextureSRVArray[g_iChangeAnimation]);
    UINT pStrides = sizeof(TVertex);
     UINT pOffsets = 0;
     m_pd3dContext->IASetVertexBuffers(
         0,
         1,
         &m_Number.m_pVertexBuffer,
         &pStrides,
         &pOffsets);
     m_pd3dContext->DrawIndexed(m_Number.m_IndexList.size(), 0, 0);
    //m_pd3dContext->PSSetShaderResources(0, 1, &m_SkillLayout.m_pTextureSRV);
    //UINT pStrides = sizeof(TVertex);
    //UINT pOffsets = 0;
    //m_pd3dContext->IASetVertexBuffers(
    //    0,
    //    1,
    //    &m_SkillLayout.m_pVertexBuffer,
    //    &pStrides,
    //    &pOffsets);
    //m_pd3dContext->DrawIndexed(m_SkillLayout.m_IndexList.size(), 0, 0);


    //m_pd3dContext->IASetVertexBuffers(
    //    0,
    //    1,
    //    &m_Skill2.m_pVertexBuffer,
    //    &pStrides,
    //    &pOffsets);

    //if (g_bChange)
    //{
    //    m_pd3dContext->PSSetShaderResources(0, 1, &m_Skill2.m_pTextureSRV);
    //}
    //else
    //{
    //    m_pd3dContext->PSSetShaderResources(0, 1, &m_Skill2.m_pTextureSRVChange);       
    //}
    //m_pd3dContext->DrawIndexed(m_Skill2.m_IndexList.size(), 0, 0);
    return true;
}
bool    Sample::Release() 
{  
    m_bk.Release();
    m_SkillLayout.Release();
    m_Skill2.Release();
    m_Number.Release();
    return true;
}

//HINSTANCE : 운영체제가 관리하는 윈도우 프로세스ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample win;
    win.SetWindow(hInstance, nCmdShow);
    win.Run();
    return 0;    
}



