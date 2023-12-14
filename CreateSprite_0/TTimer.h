#pragma once
#include <windows.h>
#include <string>
#pragma comment (lib, "winmm.lib") // timeGetTime
class TTimer
{
public:
    float   m_fGameTimer = 0.0f;
    float   m_fFramePerSecond = 0.0f;
    float   m_fSecondPerFrame = 0.0f;
public:
    UINT  m_iFPS = 0;
    DWORD m_dwTime = 0.0f;
    DWORD m_dwTickStart;
    DWORD m_dwTickEnd;
    std::wstring m_msg;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

