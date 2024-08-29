#include "TWindow.h"
TWindow* g_pWindow = nullptr;

UINT		g_xClientSize;
UINT		g_yClientSize;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
void   TWindow::CreateRegisterClass(HINSTANCE hInstance)
{
	// 1)윈도우클래스를 운영제체에 등록해야 한다.
	// 지정된 구조체를 체워서 등록한다.
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;	// 집주소(또는 전화번호)
	wc.hInstance = hInstance;		// 주민번호
	wc.lpszClassName = L"SAMPLE_WINDOW";	// 이름
	RegisterClass(&wc);

	m_hInstance = hInstance;
}
bool   TWindow::CreateWin(HINSTANCE hInstance, UINT xSize, UINT ySize)
{
	g_xClientSize = xSize;
	g_yClientSize = ySize;
	CreateRegisterClass(hInstance);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	// 추가 : 클라이언트 크기를 xSize, ySize 조정한다.
	RECT rt = {0,0,xSize, ySize};
	AdjustWindowRect(&rt, dwStyle, FALSE);

	// 2)등록된 윈도우클래스를 사용하여 윈도우 생성한다.
	HWND hwnd = CreateWindowEx(
		0,
		L"SAMPLE_WINDOW",
		L"샘플 윈도우 프로젝트", // Window text
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
	// 메세지큐에 담겨있는 모든 메세지를 수작업으로 프로시져에 전달한다.
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
void TWindow::Resize(UINT w, UINT h)
{

}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam)
{
	switch (uMsg)
	{
	// 프로그램의 활성화(포커스)
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			// 어플리케이션 비선택
			if(g_pWindow!= nullptr)
				g_pWindow->m_isActive = false;
		}
		else
		{
			// 어플리케이션 선택
			if (g_pWindow != nullptr)
				g_pWindow->m_isActive = true;
		}
	}return 0;
	case WM_SIZE:
	{
		UINT w = LOWORD(lParam);
		UINT h = HIWORD(lParam);
		GetClientRect(hwnd, &g_pWindow->m_rtClient);
		GetWindowRect(hwnd, &g_pWindow->m_rtWindow);
		g_xClientSize = w;
		g_yClientSize = h;
		g_pWindow->Resize(w, h);
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
