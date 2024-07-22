#include "TWindow.h"
TWindow* g_pWindow = nullptr;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam);
void   TWindow::CreateRegisterClass(HINSTANCE hInstance)
{
	// 1)������Ŭ������ ���ü�� ����ؾ� �Ѵ�.
	// ������ ����ü�� ü���� ����Ѵ�.
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;	// ���ּ�(�Ǵ� ��ȭ��ȣ)
	wc.hInstance = hInstance;		// �ֹι�ȣ
	wc.lpszClassName = L"SAMPLE_WINDOW";	// �̸�
	RegisterClass(&wc);

	m_hInstance = hInstance;
}
bool   TWindow::CreateWin(HINSTANCE hInstance, UINT xSize, UINT ySize)
{
	CreateRegisterClass(hInstance);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	// �߰� : Ŭ���̾�Ʈ ũ�⸦ xSize, ySize �����Ѵ�.
	RECT rt = {0,0,xSize, ySize};
	AdjustWindowRect(&rt, dwStyle, FALSE);

	// 2)��ϵ� ������Ŭ������ ����Ͽ� ������ �����Ѵ�.
	HWND hwnd = CreateWindowEx(
		0,
		L"SAMPLE_WINDOW",
		L"���� ������ ������Ʈ", // Window text
		dwStyle,	 // Window style
		// Size and position
		0, 0,   // position
		rt.right- rt.left, rt.bottom-rt.top,//Size
		NULL, // Parent window
		NULL, // Menu
		m_hInstance, // Instance handle
		NULL // Additional application data
	);
	if (hwnd == NULL)
	{
		return false;
	}

	GetWindowRect(hwnd, &m_rtWindow);
	GetClientRect(hwnd, &m_rtClient);

	m_hWnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	return true;
}
bool   TWindow::WindowRun()
{
	// �޼���ť�� ����ִ� ��� �޼����� ���۾����� ���ν����� �����Ѵ�.
	MSG msg = { };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return true;
		}
	}
	return false;
}

TWindow::TWindow()
{
	g_pWindow = this;
}
LRESULT TWindow::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam)
{
	return  0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam)
{
	g_pWindow->MsgProc(hwnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
		// ���α׷��� Ȱ��ȭ(��Ŀ��)
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			// ���ø����̼� ����
			if(g_pWindow!= nullptr)
				g_pWindow->m_isActive = false;
		}
		else
		{
			// ���ø����̼� ����
			if (g_pWindow != nullptr)
				g_pWindow->m_isActive = true;
		}
	}return 0;
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
