#include "Sample.h"
bool  Sample::Init()
{
    m_pBKSound = TSoundMgr::Get().Load(L"../../data/sound/abel_leaf.asf");
    m_pEffectA = TSoundMgr::Get().Load(L"../../data/sound/Gunshot.mp3");
    
    return true;
}
bool  Sample::Render()
{
    m_pBKSound->Frame();
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



