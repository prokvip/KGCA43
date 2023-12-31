#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include "TProtocol.h"
#include "tchar.h" // _tcscpy
#include <list>


static std::string wtm(std::wstring data)
{
    char retData[4096] = { 0 };
    // 변형되는 문자열의 크기가 반환된다.
    int iLength = WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, 0, 0, NULL, NULL);
    int iRet = WideCharToMultiByte(CP_ACP, 0,
        data.c_str(), -1,  //  소스
        retData, iLength, // 대상
        NULL, NULL);
    return retData;
}
static std::wstring mtw(std::string data)
{
    WCHAR retData[4096] = { 0 };
    // 변형되는 문자열의 크기가 반환된다.
    int iLength = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, 0, 0);
    int iRet = MultiByteToWideChar(CP_ACP, 0,
        data.c_str(), -1,  //  소스
        retData, iLength); // 대상
    return retData;
}

static void ConsolePrintW(const wchar_t* fmt, ...)
{
    va_list ap;
    wchar_t buf[MAX_PATH];

    va_start(ap, fmt);
    vswprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleW(handle, buf, wcslen(buf), &dwBytesWriten, 0);
}
static void ConsolePrintA(const char* fmt, ...)
{
    va_list ap;
    char buf[MAX_PATH];

    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleA(handle, buf, strlen(buf), &dwBytesWriten, 0);
}

#define PrintA(fmt,...)         ConsolePrintA( fmt, __VA_ARGS__ )
#define PrintDetailA(fmt,...)   ConsolePrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PrintW(fmt,...)         ConsolePrintW( fmt, __VA_ARGS__ )
#define PrintDetailW(fmt,...)   ConsolePrintW( L"[%s %d] : " fmt, __FILEW__,__LINE__, ##__VA_ARGS__ )


SOCKET      g_hSendSocket;
SOCKADDR_IN g_SendAddress;

SOCKET      g_hRecvSocket;
SOCKADDR_IN g_RecvAddress;
HWND        g_hWnd;

void UDPSocketInit()
{
    // 1) 윈속 초기화
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    g_hSendSocket = socket(AF_INET, SOCK_DGRAM, 0);
    g_hRecvSocket = socket(AF_INET, SOCK_DGRAM, 0);

    BOOL bEnable = TRUE;
    ::setsockopt(g_hSendSocket, SOL_SOCKET,
        SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
    ::setsockopt(g_hRecvSocket, SOL_SOCKET,
        SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));

    g_SendAddress.sin_family = AF_INET;
    g_SendAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    g_SendAddress.sin_port = htons(9000);

    g_RecvAddress.sin_family = AF_INET;
    g_RecvAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    g_RecvAddress.sin_port = htons(9001);
    ::bind(g_hRecvSocket,(SOCKADDR*)&g_RecvAddress, sizeof(g_RecvAddress));
   
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {    
    case WM_DESTROY:
        PostQuitMessage(0);// WM_QUIT
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

//HINSTANCE : 운영체제가 관리하는 윈도우 프로세스ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    int iSize = sizeof(CHAT_MSG);

    if (AllocConsole() == TRUE)
    {
        FILE* console = nullptr;
        if (freopen_s(&console, "conin$", "rt", stdin))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stdout))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stderr))
        {
            return 0;
        }
    }

    UDPSocketInit();
    
    char RecvBuf[2048] = { 0, };

    auto sendthread = []()
    {
        char SendBuf[2048] = "GAME ";
        while (1)
        {
            int iSendRet = sendto(g_hSendSocket, SendBuf, strlen(SendBuf), 0,
                (SOCKADDR*)&g_SendAddress,
                sizeof(g_SendAddress));
            Sleep(1000);
        }
    };
    std::thread t(sendthread);

    //1번 ) 윈도우 레지스터 클래스 등록
    MyRegisterClass(hInstance);
    //2번 ) 윈도우 생성
    InitInstance(hInstance, nCmdShow);
    
    //3번 ) 윈도우 프로시져 작업
    MSG msg;
    // 기본 메시지 루프입니다:
    //WM_QUIT : FALSE
    //while (GetMessage(&msg, nullptr, 0, 0))
    SOCKADDR_IN PeerAddr;
    int addlen;
   
    while(1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {            
            char RecvBuf[2048] = { 0, };
            addlen = sizeof(PeerAddr);
            int recvRet = recvfrom(g_hRecvSocket, RecvBuf, 2048, 0,
                (SOCKADDR*)&PeerAddr, &addlen);
            PrintA(RecvBuf);            
        }
    }
    closesocket(g_hSendSocket);
    closesocket(g_hRecvSocket);
    // )  윈속 해제    
    WSACleanup();
    t.join();
    return (int)msg.wParam;    
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS", 
        L"체팅프로그램", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    g_hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"KGCA_WINDOWS";
    //wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    return RegisterClassExW(&wcex);
}
