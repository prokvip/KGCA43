#include "Sample.h"
#include "TTextureMgr.h"
bool Sample::CreateSampleState()
{
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pDefaultSS);
    if (FAILED(hr)) return false;

    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pDefaultSSPoint);
    return true;
}
bool Sample::CreatePixelShader()
{
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pDefaultSS);

    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pDefaultSSPoint);

    ID3DBlob* pBlobByteCode;
    ID3DBlob* ppErrorMsgs = nullptr;
    hr = D3DCompileFromFile(
        L"DefaultShaderVSPS.txt",
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
bool Sample::AlphaBlendState()
{
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    //bsd.AlphaToCoverageEnable = TRUE;
    bsd.RenderTarget[0].BlendEnable = TRUE;
 
    // rgb  
 //FINALCOLOR = DEST COLOR* (1-SRCALPHA) + SRC COLOR * SRC ALPAH
    bsd.RenderTarget[0].SrcBlend= D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend= D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp= D3D11_BLEND_OP_ADD;

    // A
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
    bsd.RenderTarget[0].BlendOpAlpha= D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].RenderTargetWriteMask= D3D11_COLOR_WRITE_ENABLE_ALL;


    HRESULT hr = m_pd3dDevice->CreateBlendState(
        &bsd,
        &m_pAlphaBlendEnable);

    bsd.RenderTarget[0].BlendEnable = FALSE;
    hr = m_pd3dDevice->CreateBlendState(
        &bsd,
        &m_pAlphaBlendDisable);

    //m_pd3dContext->OMSetBlendState(m_pAlphaBlendEnable, 0, -1);
    return true;
}

bool    Sample::Init()
{     
    m_GameTimer.Init();
    TInput::Get().Init();
    TTextureMgr::Get().Set(m_pd3dDevice, m_pd3dContext);
    AlphaBlendState();
    CreateSampleState();
    CreatePixelShader();
    m_DefaultPlane.m_pd3dDevice = m_pd3dDevice;
    m_DefaultPlane.m_pd3dContext = m_pd3dContext;
    m_DefaultPlane.m_rtClient = m_rtClient;

    m_DefaultPlane.m_VertexList.emplace_back(TVector3(0, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
    m_DefaultPlane.m_VertexList.emplace_back(TVector3(800.0f, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
    m_DefaultPlane.m_VertexList.emplace_back(TVector3(800.0f, 600, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
    m_DefaultPlane.m_VertexList.emplace_back(TVector3(0.0f, 600, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3

    m_DefaultPlane.Init();
    if (!m_DefaultPlane.Load(L"../../data/RECT.png"))
    {
        return false;
    }
    m_bk = std::make_shared<TUiObj>();
        m_bk->m_VertexList.emplace_back(TVector3(0.0f, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_bk->m_VertexList.emplace_back(TVector3(m_rtClient.right, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_bk->m_VertexList.emplace_back(TVector3(m_rtClient.right, m_rtClient.bottom, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_bk->m_VertexList.emplace_back(TVector3(0.0f, m_rtClient.bottom, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        m_bk->Load(L"../../data/RECT.png");
    m_uiList.push_back(m_bk);

    m_SkillLayout = std::make_shared<TUiObj>();
        m_SkillLayout->m_VertexList.emplace_back(TVector3(18.0f, 477.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_SkillLayout->m_VertexList.emplace_back(TVector3(460, 477.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_SkillLayout->m_VertexList.emplace_back(TVector3(460, 600, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_SkillLayout->m_VertexList.emplace_back(TVector3(18.0f, 600, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        m_SkillLayout->Load(L"../../data/skill.png");
    m_uiList.push_back(m_SkillLayout);

    m_Skill2 = std::make_shared<TUiObj>();
        m_Skill2->m_VertexList.emplace_back(TVector3(115.0f, 487.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_Skill2->m_VertexList.emplace_back(TVector3(178.0f, 487.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_Skill2->m_VertexList.emplace_back(TVector3(178.0f, 590, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_Skill2->m_VertexList.emplace_back(TVector3(115.0f, 590, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        if (!m_Skill2->Load(L"../../data/s2.png"))
        {
            return false;
        }
    m_uiList.push_back(m_Skill2);

    m_Number = std::make_shared<TUiNumber>();
        m_Number->m_VertexList.emplace_back(TVector3(0, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 0.0f));      // 0
        m_Number->m_VertexList.emplace_back(TVector3(52.0f, 0.0f, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 0.0f));    // 1
        m_Number->m_VertexList.emplace_back(TVector3(52.0f, 68, 0.5f), TVector4(1, 1, 1, 1), TVector2(1.0f, 1.0f));  // 2
        m_Number->m_VertexList.emplace_back(TVector3(0.0f, 68, 0.5f), TVector4(1, 1, 1, 1), TVector2(0.0f, 1.0f));    // 3
        if (!m_Number->Load(L"../../data/0.png"))
        {
            return false;
        }        
    m_uiList.push_back(m_Number);
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
    return true;
}
bool    Sample::Render()
{
    m_pd3dContext->PSSetSamplers(0, 1, &m_pDefaultSS);
    m_pd3dContext->OMSetBlendState(m_pAlphaBlendEnable, 0, -1);
    
    TInput::Get().Render();

    for (auto data : m_uiList)
    {
        m_pd3dContext->UpdateSubresource(m_DefaultPlane.m_pVertexBuffer,
            0,
            nullptr,
            &data->m_VertexList.at(0),
            0,
            0);
        m_DefaultPlane.PreRender();        
            data->Frame(m_GameTimer.m_fSecondPerFrame);
            data->Render(m_pd3dContext);
        m_DefaultPlane.PostRender();
    }


    m_GameTimer.Render();
    return true;
}
bool    Sample::Release() 
{  
    if (m_pDefaultSS)m_pDefaultSS->Release();
    if (m_pDefaultSSPoint)m_pDefaultSSPoint->Release();
    if (m_pAlphaBlendEnable)m_pAlphaBlendEnable->Release();
    if (m_pAlphaBlendDisable)m_pAlphaBlendDisable->Release();
    if (m_pPixelShaderAlphaTest)m_pPixelShaderAlphaTest->Release();

    for (auto data : m_uiList)
    {
        data->Release();
    }
    m_DefaultPlane.Release();

    TInput::Get().Release();

    m_GameTimer.Release();
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



