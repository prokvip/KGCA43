#pragma once
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")

class TTimer
{
	// ���� ��� �ð�
	float m_fGameTimeTick;
	// 1�������� ����ð�
	float   m_fSecondPerFrame = 0.0f;
	DWORD   m_dwSecondPerFrame = 0;
	// 1�ʿ� ����� ������ ī����
	DWORD   m_dwFPS;

	DWORD m_dwPreTimeTick;
	DWORD m_dwCurrentTimeTick;

	int		iFPS = 0;
	float	fSecondTime = 0.0f;
public:
	void    DebugTimer();
	void    Init();
	void    Frame();
public:
	TTimer()
	{
		Init();
	}
};
