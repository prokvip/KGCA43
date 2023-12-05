#pragma once
#include <windows.h>
class TWindow
{
public:
    HWND        m_hWnd;
    HINSTANCE   m_hInstance;
public:
    void SetWindow(HINSTANCE hInstance, int    nCmdShow);
    void Run();
    void ReleaseAll();
public:
    virtual bool GameInit() { return true; };
    virtual bool GameRender() { return true; };
    virtual bool GameRelease() { return true; };
public:
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
    ATOM MyRegisterClass(HINSTANCE hInstance);
};

