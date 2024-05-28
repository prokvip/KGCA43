#pragma once
#include <Windows.h>
class TWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool        m_isActive;
public:
	void   CreateRegisterClass(HINSTANCE hInstance);
	bool   CreateWin(UINT xSize = 800, UINT ySize = 600);
	void   WindowRun();
	virtual void   GameInit();
	virtual void   GameRun();
	virtual void   GameRelease();
	TWindow();
};

