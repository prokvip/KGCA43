#include <windows.h> // createthread, critical_section, CreateMutex, CreateEvent
#include <iostream>
#include <thread> // modern c++ ( 11이상)
#include <mutex> // modern c++ ( 11이상)
#include <chrono> // modern c++ ( 11이상) std::chrono::milliseconds(1)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RaceCrit");
std::mutex g_hMutex; // 상호배제
std::mutex g_hEventMutex;
int X;
CRITICAL_SECTION critA;
CRITICAL_SECTION critB;
std::condition_variable g_hEvent;

bool  g_bDataReady = false;

void  ThreadFunc1(int iMax)
{
	//WaitForSingleObject(g_hEvent, INFINITE);	
	{
		std::unique_lock<std::mutex> hLock(g_hEventMutex);
		g_hEvent.wait(hLock, [] { return g_bDataReady; });
		//while (!g_bDataReady)
		//{
		//	g_hEvent.wait(hLock);// , [] { return g_bDataReady; });
		//}
		HDC hdc;
		hdc = GetDC(hWndMain);
		for (int i = 0; i < iMax; i++)
		{
			std::lock_guard<std::mutex> lc(g_hMutex);
			//g_hMutex.lock();	
			X = 100; // 변수가 공유 : datarace, 
			//Sleep(1);// 스위칭
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			TextOutA(hdc, X, 100, "강아지", 6);
			//g_hMutex.unlock();
		}
		ReleaseDC(hWndMain, hdc);
	}
	return ;
}
void  ThreadFunc2(int iMax)
{
	std::unique_lock<std::mutex> hLock(g_hEventMutex);
	//g_hEvent.wait(hLock);
	g_hEvent.wait(hLock, [] { return g_bDataReady; });

	HDC hdc;
	hdc = GetDC(hWndMain);
	for (int i = 0; i < iMax; i++) 
	{		
		{
			std::lock_guard<std::mutex> lc(g_hMutex);
			X = 200;
			//Sleep(1);// 스위칭(스레드컨텍스트, 스레드 스위칭)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			TextOutA(hdc, X, 200, "고양이", 6);
		}
	}
	ReleaseDC(hWndMain, hdc);
	return ;
}

std::thread t1(ThreadFunc1, 10);
std::thread t2(ThreadFunc1, 10);
std::thread t3(ThreadFunc1, 10);
std::thread t4(ThreadFunc2, 10);


class Tcs
{
public:
	CRITICAL_SECTION cs;
	void  Run() {}
	Tcs() { 
		InitializeCriticalSection(&cs); }
	virtual ~Tcs() {
		DeleteCriticalSection(&cs); 
	}
};
class TLock_guard
{
	Tcs* m_pTcs = nullptr;
public:
	TLock_guard(Tcs* tcs): m_pTcs(tcs)
	{
		EnterCriticalSection(&tcs->cs);
	}
	~TLock_guard()
	{
		LeaveCriticalSection(&m_pTcs->cs);
	}
};
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hEvent.notify_all();
	{
		Tcs tcs;
		{
			TLock_guard sync(&tcs);
			std::cout << "aaaa";
		}
		{
			TLock_guard sync(&tcs);
			std::cout << "bbbb";
		}
		{
			TLock_guard sync(&tcs);
			std::cout << "cccc";
		}
	}

	HANDLE hMutex = CreateMutex(NULL, FALSE, L"KGCA");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, L"이미 클라이언트가 실행 중입니다.!", L"중복실행", MB_OK);
		CloseHandle(hMutex);
		return 1;
	}
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	// JOIN
	//WaitForSingleObject(g_hThread, INFINITE);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return (int)Message.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		//SetEvent(g_hEvent);	
		g_bDataReady = true;
		g_hEvent.notify_all();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

