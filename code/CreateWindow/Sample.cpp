//error LNK2019 : WinMain"int __cdecl invoke_main(void)" (? invoke_main@@YAHXZ) �Լ����� �����Ǵ� Ȯ���� �� ���� �ܺ� ��ȣ
#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
	lParam);

int WINAPI wWinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance, 
					PWSTR pCmdLine, 
					int nCmdShow)
{
	// 1)������Ŭ������ ���ü�� ����ؾ� �Ѵ�.
	// ������ ����ü�� ü���� ����Ѵ�.
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;	// ���ּ�(�Ǵ� ��ȭ��ȣ)
	wc.hInstance = hInstance;		// �ֹι�ȣ
	wc.lpszClassName = L"ȫ�浿2";	// �̸�
	RegisterClass(&wc);

	
	// 2)��ϵ� ������Ŭ������ ����Ͽ� ������ �����Ѵ�.
	HWND hwnd = CreateWindowEx(
		0, 
		L"ȫ�浿2", 
		L"���� ������ ������Ʈ", // Window text
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
