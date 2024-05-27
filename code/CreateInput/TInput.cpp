#include "TInput.h"
/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
	void    TInput::DebugPrint(int iKey)
	{
#ifdef _DEBUG
		static int iPushCounter = 0;
		TCHAR msgKey[MAX_PATH] = { 0, };
		_stprintf_s(msgKey, L"KEY=%d(%d)\n", iKey, iPushCounter++);
		OutputDebugString(msgKey);
#endif
	}
	void    TInput::DebugMousePos()
	{
#ifdef _DEBUG
		TCHAR msgKey[MAX_PATH] = { 0, };
		_stprintf_s(msgKey, L"Mouse X=%d, Y=%d\n", 
			m_ptMousePos.x, m_ptMousePos.y);
		OutputDebugString(msgKey);
#endif
	}

void    TInput::Frame(HWND hWnd)
{
	// ȭ����ǥ�踦 ��ȯ�Ѵ�.
	GetCursorPos(&m_ptMousePos);
	// Ŭ���̾�Ʈ ��ǥ��� ��ȯ�Ѵ�.
	ScreenToClient(hWnd, &m_ptMousePos);

	for (int iKey = 0; iKey < 255; iKey++)
	{
		//			GetKeyState(iKey);
					// iKey�� �ش��ϴ� Ű�� ���¸� ��ȯ�ް�
		SHORT sKey = GetAsyncKeyState(iKey);
		// 8     0     0    0
		// 1000  0000  0000 0000
		//         & 
		// 1000  0000  0000 0000
		if (sKey & 0x8000) // ������.
		{
			// ���������ӿ��� 
			if (g_dwKeyState[iKey] == KEY_FREE ||
				g_dwKeyState[iKey] == KEY_UP)
			{
				g_dwKeyState[iKey] = KEY_PUSH;
			}
			else
			{
				g_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else // ���� �����ӿ����� �������� �ʾҴ�.
		{
			// ���������ӿ��� 
			if (g_dwKeyState[iKey] == KEY_PUSH ||
				g_dwKeyState[iKey] == KEY_HOLD)
			{
				g_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				g_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}

}
void    TInput::KeyTest()
{
	/*
	 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	 * 0x3A - 0x40 : unassigned
	 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	 */
	if (g_dwKeyState[VK_HOME] == KEY_PUSH)
	{
		DebugPrint(KEY_PUSH);
	}
	if (g_dwKeyState[VK_HOME] == KEY_HOLD)
	{
		DebugPrint(KEY_HOLD);
	}
	if (g_dwKeyState[VK_HOME] == KEY_UP)
	{
		DebugPrint(KEY_UP);
	}

	//if (g_dwKeyState[VK_LBUTTON] == KEY_PUSH)
	//{
	//	DebugPrint(VK_LBUTTON);
	//}
	//if (g_dwKeyState[VK_RBUTTON] == KEY_PUSH)
	//{
	//	DebugPrint(VK_RBUTTON);
	//}
	//if (g_dwKeyState[VK_MBUTTON] == KEY_PUSH)
	//{
	//	DebugPrint(VK_MBUTTON);
	//}
	//if (g_dwKeyState['W'] == KEY_PUSH)
	//{
	//	DebugPrint('W');
	//}
	//if (g_dwKeyState['S'] == KEY_PUSH)
	//{
	//	DebugPrint('S');
	//}
	//if (g_dwKeyState['A'] == KEY_PUSH)
	//{
	//	DebugPrint('A');
	//}
	//if (g_dwKeyState['D'] == KEY_PUSH)
	//{
	//	DebugPrint('D');
	//}	
}
DWORD   TInput::KeyCheck(DWORD dwKey)
{
	return g_dwKeyState[dwKey];
}