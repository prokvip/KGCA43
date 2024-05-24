#include "TWindow.h"
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
bool   TWindow::CreateWin(UINT xSize, UINT ySize)
{
	// 2)��ϵ� ������Ŭ������ ����Ͽ� ������ �����Ѵ�.
	HWND hwnd = CreateWindowEx(
		0,
		L"SAMPLE_WINDOW",
		L"���� ������ ������Ʈ", // Window text
		WS_OVERLAPPEDWINDOW,	 // Window style
		// Size and position
		0, 0,   // position
		xSize, ySize,//Size
		NULL, // Parent window
		NULL, // Menu
		m_hInstance, // Instance handle
		NULL // Additional application data
	);
	if (hwnd == NULL)
	{
		return false;
	}
	m_hWnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	return true;
}
void   TWindow::WindowRun()
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
			// ���� ������ �����Ѵ�. 1������
			GameRun();
		}
	}
}
void   TWindow::GameRun()
{

}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
