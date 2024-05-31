#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool        m_isActive;
	UINT		m_xWindowSize;
	UINT		m_yWindowSize;
public:
	void   CreateRegisterClass(HINSTANCE hInstance);
	bool   CreateWin(HINSTANCE hInstance, UINT xSize = 800, UINT ySize = 600);
	bool   WindowRun();
	TWindow();
};

