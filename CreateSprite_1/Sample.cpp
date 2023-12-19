#include "Sample.h"
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
bool  Sample::Init()
{
    CreatePixelShader();
    m_pBKSound = TSoundMgr::Get().Load(L"../../data/sound/abel_leaf.asf");
    m_pEffectA = TSoundMgr::Get().Load(L"../../data/sound/Gunshot.mp3");
    m_EffectA = std::make_shared<TEffect>();
    m_EffectA->m_VertexList.emplace_back(TVector3(100, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
    m_EffectA->m_VertexList.emplace_back(TVector3(200.0f, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
    m_EffectA->m_VertexList.emplace_back(TVector3(200.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_EffectA->m_VertexList.emplace_back(TVector3(100.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
    if (!m_EffectA->Create(L"EffectA", L"../../data/effect/alpha/crash.dds", 8, 8, 1.0f))
    {
        return false;
    }

    m_EffectB = std::make_shared<TEffect>();
    m_EffectB->m_VertexList.emplace_back(TVector3(200, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
    m_EffectB->m_VertexList.emplace_back(TVector3(300.0f, 100.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
    m_EffectB->m_VertexList.emplace_back(TVector3(300.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_EffectB->m_VertexList.emplace_back(TVector3(200.0f, 200, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
    if (!m_EffectB->Create(L"EffectB", L"../../data/effect/103.TGA", 4, 4, 1.0f))
    {
        return false;
    }
    return true;
}
bool Sample::Frame()
{
    m_pBKSound->Frame();
    m_EffectA->Frame(m_GameTimer.m_fSecondPerFrame);
    m_EffectB->Frame(m_GameTimer.m_fSecondPerFrame);
    return true;
}
bool  Sample::Render()
{    
   // m_pd3dContext->OMSetBlendState(m_pDualSourceColorBlending, 0, -1);
    m_pd3dContext->OMSetBlendState(m_pAlphaBlendEnable, 0, -1);
    m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
        0,
        nullptr,
        &m_EffectA->m_VertexList.at(0),
        0,
        0);
    m_DefaultPlane.PreRender();
    //m_pd3dContext->PSSetShader(m_pPixelShaderAlphaTest, NULL, 0);
    m_EffectA->Render(m_pd3dContext);
    m_DefaultPlane.PostRender();


    m_pd3dContext->OMSetBlendState(m_pDualSourceColorBlending, 0, -1);
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
    
    if (TInput::Get().m_dwKeyState[VK_END] == KEY_PUSH)
    {
        m_pEffectA->PlayEffect();
    }

    if (TInput::Get().m_dwKeyState[VK_INSERT] == KEY_PUSH)
    {
        m_pBKSound->Play();
    }
    if (TInput::Get().m_dwKeyState[VK_HOME] == KEY_PUSH)
    {
        m_pBKSound->Paused();
    }
    if (TInput::Get().m_dwKeyState[VK_DELETE] == KEY_PUSH)
    {
        m_pBKSound->Stop();
    }
    if (TInput::Get().m_dwKeyState[VK_PRIOR] == KEY_HOLD)
    {
        m_pBKSound->VolumeUp(g_fSecondPerFrame);
    }
    if (TInput::Get().m_dwKeyState[VK_NEXT] == KEY_HOLD)
    {
        m_pBKSound->VolumeDown(g_fSecondPerFrame);
    }

    m_dxWrite.Draw30(100, 300, m_pBKSound->m_szBuffer);
    return true;
}
bool  Sample::Release()
{
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



