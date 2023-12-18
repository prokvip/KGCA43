#include "Sample.h"
#include "TTextureMgr.h"
bool Sample::CreatePixelShader()
{   
    ID3DBlob* pBlobByteCode;
    ID3DBlob* ppErrorMsgs = nullptr;
    HRESULT hr = D3DCompileFromFile(
        L"effect.txt",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &pBlobByteCode,
        &ppErrorMsgs);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, (char*)ppErrorMsgs->GetBufferPointer(), "Error", MB_OK);
        if (ppErrorMsgs) ppErrorMsgs->Release();
        return false;
    }
    if (ppErrorMsgs) ppErrorMsgs->Release();

    //m_pd3dDevice->CreatePixelShader();

    hr = m_pd3dDevice->CreatePixelShader(
        pBlobByteCode->GetBufferPointer(),
        pBlobByteCode->GetBufferSize(),
        nullptr,
        &m_pPixelShaderAlphaTest);

    if (pBlobByteCode)pBlobByteCode->Release();
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool    Sample::Init()
{     
    CreatePixelShader();
    m_bk = std::make_shared<TUiObj>();
        m_bk->m_VertexList.emplace_back(TVector3(0.0f, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_bk->m_VertexList.emplace_back(TVector3(m_rtClient.right, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_bk->m_VertexList.emplace_back(TVector3(m_rtClient.right, m_rtClient.bottom, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_bk->m_VertexList.emplace_back(TVector3(0.0f, m_rtClient.bottom, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        m_bk->Create(L"BACK GOURND", L"../../data/RECT.png");
    m_uiList.push_back(m_bk);

    m_btnStart = std::make_shared<TUiObj>();
        m_btnStart->m_VertexList.emplace_back(TVector3(350.0f, 275.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_btnStart->m_VertexList.emplace_back(TVector3(350+100, 275, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_btnStart->m_VertexList.emplace_back(TVector3(350+100, 275+50, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_btnStart->m_VertexList.emplace_back(TVector3(350.0f, 275+50, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        T_STR_VECTOR texArrayStart = {
            L"../../data/main_start_nor.png",
            L"../../data/main_start_pus.png",
            L"../../data/main_start_sel.png",
            L"../../data/main_start_dis.png"
        };
        m_btnStart->Create(L"Start Button", texArrayStart);
    m_uiList.push_back(m_btnStart);

    m_Item = std::make_shared<TUiObj>();
        m_Item->m_VertexList.emplace_back(TVector3(115.0f, 487.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_Item->m_VertexList.emplace_back(TVector3(178.0f, 487.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_Item->m_VertexList.emplace_back(TVector3(178.0f, 590, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_Item->m_VertexList.emplace_back(TVector3(115.0f, 590, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        T_STR_VECTOR texArray = {
            L"../../data/main_custom_nor.png",
            L"../../data/main_custom_pus.png",
            L"../../data/main_custom_sel.png",
            L"../../data/main_custom_dis.png"
        };
        if (!m_Item->Create(L"item1", texArray))
        {
            return false;
        }
    m_uiList.push_back(m_Item);

    m_Number = std::make_shared<TUiNumber>();
        m_Number->m_VertexList.emplace_back(TVector3(0, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_Number->m_VertexList.emplace_back(TVector3(52.0f, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_Number->m_VertexList.emplace_back(TVector3(52.0f, 68, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_Number->m_VertexList.emplace_back(TVector3(0.0f, 68, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        if (!m_Number->Create(L"Number"))
        {
            return false;
        }        
    m_uiList.push_back(m_Number);

    m_EffectA = std::make_shared<TEffect>();
    m_EffectA->m_VertexList.emplace_back(TVector3(100, 100.0f, 0.5f), TVector4(1, 0, 0, 1), TVector2(0.0f, 0.0f));      // 0
    m_EffectA->m_VertexList.emplace_back(TVector3(200.0f, 100.0f, 0.5f), TVector4(1, 0, 0, 1), TVector2(1.0f, 0.0f));    // 1
    m_EffectA->m_VertexList.emplace_back(TVector3(200.0f, 200, 0.5f), TVector4(1, 0, 0, 1), TVector2(1.0f, 1.0f));  // 2
    m_EffectA->m_VertexList.emplace_back(TVector3(100.0f, 200, 0.5f), TVector4(1, 0, 0, 1), TVector2(0.0f, 1.0f));    // 3
    if (!m_EffectA->Create(L"EffectA",L"../../data/effect/alpha/critical.dds", 8, 8, 1.0f))
    {
        return false;
    }

    m_EffectB = std::make_shared<TEffect>();
    m_EffectB->m_VertexList.emplace_back(TVector3(200, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
    m_EffectB->m_VertexList.emplace_back(TVector3(300.0f, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
    m_EffectB->m_VertexList.emplace_back(TVector3(300.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_EffectB->m_VertexList.emplace_back(TVector3(200.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
    if (!m_EffectB->Create(L"EffectB", L"../../data/effect/get_item_02.tga", 4, 4, 1.0f))
    {
        return false;
    }
    
    return true;
}
bool    Sample::Frame()
{
    m_GameTimer.Frame();
    TInput::Get().Frame();
    /*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
    if (TInput::Get().m_dwKeyState['W'] == KEY_PUSH)
    {
        ::OutputDebugString(L"W[PUSH].\n");
    }
    if (TInput::Get().m_dwKeyState['W'] == KEY_HOLD)
    {
        ::OutputDebugString(L"W[HOLD].\n");
    }
    if (TInput::Get().m_dwKeyState['W'] == KEY_UP)
    {
        ::OutputDebugString(L"W[UP].\n");
    }
    if (TInput::Get().m_dwKeyState['A'] == KEY_PUSH)
    {
        ::OutputDebugString(L"A[PUSH].\n");
    }
    if (TInput::Get().m_dwKeyState['S'] == KEY_PUSH)
    {
        ::OutputDebugString(L"S[PUSH].\n");
    }
    if (TInput::Get().m_dwKeyState['D'] == KEY_PUSH)
    {
        ::OutputDebugString(L"D[PUSH].\n");
    }
    if (TInput::Get().m_dwKeyState[VK_LBUTTON] == KEY_UP)
    {
        g_bChange = !g_bChange;
    }
    if (TInput::Get().m_dwKeyState[VK_RBUTTON] == KEY_UP)
    {
        g_iChangeAnimation++;
        if (g_iChangeAnimation >= 10) g_iChangeAnimation = 0;
    }

    for (auto data : m_uiList)
    {
        data->Frame(m_GameTimer.m_fSecondPerFrame);
    }

    m_EffectA->Frame(m_GameTimer.m_fSecondPerFrame);
    m_EffectB->Frame(m_GameTimer.m_fSecondPerFrame);
    return true;
}
bool    Sample::Render()
{
    m_pd3dContext->PSSetSamplers(0, 1, &m_pDefaultSS);
    m_pd3dContext->OMSetBlendState(m_pAlphaBlendEnable, 0, -1);    
#pragma region BK
    m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
        0,
        nullptr,
        &m_bk->m_VertexList.at(0),
        0,
        0);
    m_DefaultPlane.PreRender();
    m_bk->Render(m_pd3dContext);
    m_DefaultPlane.PostRender();
#pragma endregion BK

    m_pd3dContext->OMSetBlendState(m_pAlphaBlendEnable, 0, -1);
    for (auto data : m_uiList)
    {
        m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
            0,
            nullptr,
            &data->m_VertexList.at(0),
            0,
            0);
        m_DefaultPlane.PreRender();        
             data->Render(m_pd3dContext);
        m_DefaultPlane.PostRender();    
    }
    
#pragma region EFFECT
   
    m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
        0,
        nullptr,
        &m_EffectA->m_VertexList.at(0),
        0,
        0);
    m_DefaultPlane.PreRender();
        m_pd3dContext->PSSetShader(m_pPixelShaderAlphaTest, NULL, 0);
        m_EffectA->Render(m_pd3dContext);
    m_DefaultPlane.PostRender();

    m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
        0,
        nullptr,
        &m_EffectB->m_VertexList.at(0),
        0,
        0);
    m_DefaultPlane.PreRender();
        m_pd3dContext->PSSetShader(m_pPixelShaderAlphaTest, NULL, 0);
        m_EffectB->Render(m_pd3dContext);
    m_DefaultPlane.PostRender();
#pragma endregion EFFECT  
  
    m_dxWrite.Draw20(0, 30, m_GameTimer.m_outmsg);
    return true;
}
bool    Sample::Release() 
{    
    if (m_pPixelShaderAlphaTest)m_pPixelShaderAlphaTest->Release();
   if(m_bk)m_bk->Release();
   if (m_btnStart)m_btnStart->Release();
   if (m_Item)m_Item->Release();
   if (m_Number)m_Number->Release();
   if (m_EffectA)m_EffectA->Release();
   if (m_EffectB)m_EffectB->Release();   
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



