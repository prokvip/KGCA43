#include "TInput.h"
bool	 TInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}
bool	 TInput::Frame()
{
	for (DWORD dwKey = 0; dwKey < 256; dwKey++)
	{
		SHORT sKey = ::GetAsyncKeyState(dwKey);
		// Áö±Ý ´­·¶´Ù.
		if (sKey & 0x8000) // 1000 0000 0000 0000
		{
			if( m_dwKeyState[dwKey] == KEY_FREE || 
				m_dwKeyState[dwKey] == KEY_UP)
				m_dwKeyState[dwKey] = KEY_PUSH;
			else
				m_dwKeyState[dwKey] = KEY_HOLD;
		}
		else
		{
			if (m_dwKeyState[dwKey] == KEY_PUSH ||
				m_dwKeyState[dwKey] == KEY_HOLD)
				m_dwKeyState[dwKey] = KEY_UP;
			else
				m_dwKeyState[dwKey] = KEY_FREE;
		}
	}
	return true;
}
bool	 TInput::Render()
{
	return true;
}
bool	 TInput::Release()
{
	return true;
}
