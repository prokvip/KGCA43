#include "TWindow.h"

int WINAPI wWinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance, 
					PWSTR pCmdLine, 
					int nCmdShow)
{
	TWindow win;
	win.CreateRegisterClass(hInstance);
	if (win.CreateWin(1024, 768))
	{
		win.WindowRun();
	}	
	return 0;
}
