//error LNK2019 : WinMain"int __cdecl invoke_main(void)" (? invoke_main@@YAHXZ) 함수에서 참조되는 확인할 수 없는 외부 기호
#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam);

int WINAPI wWinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance, 
					PWSTR pCmdLine, 
					int nCmdShow)
{
	// 1)윈도우클래스를 운영제체에 등록해야 한다.
	// 지정된 구조체를 체워서 등록한다.
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;	// 집주소(또는 전화번호)
	wc.hInstance = hInstance;		// 주민번호
	wc.lpszClassName = L"홍길동2";	// 이름
	RegisterClass(&wc);

	
	// 2)등록된 윈도우클래스를 사용하여 윈도우 생성한다.
	HWND hwnd = CreateWindowEx(
		0, 
		L"홍길동2", 
		L"샘플 윈도우 프로젝트", // Window text
		WS_OVERLAPPEDWINDOW,	 // Window style
		// Size and position
		0, 0,   // position
		800, 600,//Size
		NULL, // Parent window
		NULL, // Menu
		hInstance, // Instance handle
		NULL // Additional application data
	);
	if (hwnd == NULL)
	{
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	
	// Run the message loop.
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
