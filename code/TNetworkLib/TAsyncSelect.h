#pragma once
#include "TSelectModel.h"
#define WM_SOCKET   (WM_USER+1)
class TAsyncSelect : public TSelectModel
{
public:
	HWND m_hWnd;
public:
	LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
	bool    Set(HWND hWnd);
	bool	Run();
public:
	TAsyncSelect() = default;
	TAsyncSelect(TNetwork * net);
	virtual ~TAsyncSelect();
};

