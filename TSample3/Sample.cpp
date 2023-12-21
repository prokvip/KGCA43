#include "Sample.h"
bool	   Sample::Init()
{
    return true;
}
bool	   Sample::Frame()
{
    return true;
}
bool	   Sample::Render()
{
    return true;
}
bool	   Sample::Release()
{
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
