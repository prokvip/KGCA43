#pragma once
#include <windows.h>

enum { KEY_FREE=0, KEY_PUSH, KEY_HOLD, KEY_UP };

class TInput
{
public:
	POINT	m_ptMouse;
	DWORD    m_dwKeyState[256];
	static TInput& Get()
	{
		static TInput mgr;
		return mgr;
	}
public:
	bool	 Init();
	bool	 Frame();
	bool	 Render();
	bool	 Release();
private:
	TInput() {};
public:
	~TInput() {}
};

