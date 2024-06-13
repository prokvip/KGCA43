#pragma once
#include <windows.h>
#include <tchar.h>
enum KeyState
{
	KEY_FREE = 0,	// 평상시
	KEY_UP,			// 바로 눌렀다가 오픈상태
	KEY_PUSH,		// 오픈상태에서 눌렀을 때
	KEY_HOLD,		// 누루고 있을 때(이전도 눌렀고 지금도 눌렀다.)
};

class TInput
{
private:
	DWORD   g_dwKeyState[256];
public:	
	POINT   m_ptMousePos;
public:
	void    DebugPrint(int iKey);
	void    DebugMousePos();
	void    Frame(HWND hWnd);
	DWORD   KeyCheck(DWORD dwKey);
	void    KeyTest();
public:
	TInput()
	{
		ZeroMemory(&g_dwKeyState, sizeof(DWORD) * 256);
	}
};

