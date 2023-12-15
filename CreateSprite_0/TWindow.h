#pragma once
#include "TStd.h"
extern bool g_bChange;
extern int g_iChangeAnimation;
class TWindow
{
public:
    HWND        m_hWnd;
    HINSTANCE   m_hInstance;
    RECT        m_rtWindow;
    RECT        m_rtClient;
public:
    void SetWindow(HINSTANCE hInstance, int    nCmdShow);
    void Run();
    void ReleaseAll();
public:
    virtual bool GameInit() { return true; };
    virtual bool GameFrame() { return true; };
    virtual bool GameRender() { return true; };
    virtual bool GameRelease() { return true; };
public:
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
    ATOM MyRegisterClass(HINSTANCE hInstance);
};

