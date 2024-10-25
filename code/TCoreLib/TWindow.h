#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool        m_isActive;
	
	RECT		m_rtWindow;
	RECT		m_rtClient;
public:
	void   CreateRegisterClass(HINSTANCE hInstance);
	bool   CreateWin(HINSTANCE hInstance, UINT xSize = 800, UINT ySize = 600);
	bool   WindowRun();
	virtual void Resize(UINT w, UINT h);
	virtual LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
	TWindow();
};

