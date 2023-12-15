#include "TDevice.h"
bool    TDevice::GameInit()
{
    D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;
    UINT Flags = 0;
    D3D_FEATURE_LEVEL  pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    DXGI_SWAP_CHAIN_DESC sd;
    //DXGI_MODE_DESC BufferDesc;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    //DXGI_SAMPLE_DESC SampleDesc;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = m_hWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        DriverType,
        nullptr,
        Flags,
        pFeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pd3dDevice,
        nullptr,
        &m_pd3dContext);
    if (FAILED(hr))
    {
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    if (pBackBuffer) pBackBuffer->Release();

    m_pd3dContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);


    D3D11_VIEWPORT vp;
    vp.Width    = sd.BufferDesc.Width;
    vp.Height   = sd.BufferDesc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pd3dContext->RSSetViewports(1, &vp);

    Init();
    return true;
}
bool    TDevice::GameFrame()
{
    Frame();
    return true;
}
bool    TDevice::GameRender()
{
    float clearColor[] = { 0.0f,0.0f,0.0f,1 };
    m_pd3dContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

    Render();

    m_pSwapChain->Present(0, 0);
    return true;
}
bool    TDevice::GameRelease()
{
    Release();

    if (m_pd3dDevice != nullptr) m_pd3dDevice->Release();
    if (m_pd3dContext) m_pd3dContext->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    m_pd3dDevice = nullptr;
    m_pd3dContext = nullptr;
    m_pRenderTargetView = nullptr;
    m_pSwapChain = nullptr;
    return true;
}
