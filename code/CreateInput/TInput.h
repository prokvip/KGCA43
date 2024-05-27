#pragma once
#include <windows.h>
#include <tchar.h>
enum KeyState
{
	KEY_FREE = 0,	// ����
	KEY_UP,			// �ٷ� �����ٰ� ���»���
	KEY_PUSH,		// ���»��¿��� ������ ��
	KEY_HOLD,		// ����� ���� ��(������ ������ ���ݵ� ������.)
};

class TInput
{
private:
	DWORD   g_dwKeyState[256];
public:
public:
	void    DebugPrint(int iKey);
	void    Frame();
	DWORD   KeyCheck(DWORD dwKey);
	void    KeyTest();
public:
	TInput()
	{
		ZeroMemory(&g_dwKeyState, sizeof(DWORD) * 256);
	}
};

