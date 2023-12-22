#include "Sample.h"
bool	   Sample::Init()
{
    m_Hero = std::make_shared<THeroObj>();
    m_Hero->m_pd3dDevice = m_pd3dDevice;
    m_Hero->m_pd3dContext = m_pd3dContext;
    m_Hero->m_rtClient = m_rtClient;

    TInitSet info = { L"Hero", L"../../data/bitmap2.bmp",
                    {380.0f, 300.0f },
                    {91.0f,1.0f},
                    {40.0f, 60.0f},300.0f };
    if (!m_Hero->Create(info, L"../../data/bitmap1.bmp"))
    {
        return false;
    }
    return true;
}
bool	   Sample::Frame()
{
    m_Hero->Frame();
    return true;
}
bool	   Sample::Render()
{
    m_Hero->Render();
    return true;
}
bool	   Sample::Release()
{
    m_Hero->Release();
    return true;
}

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
