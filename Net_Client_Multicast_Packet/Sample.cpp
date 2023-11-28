#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include "TProtocol.h"
#include "tchar.h" // _tcscpy
#include <list>

int   g_Mode = MCAST_EXCLUDE;// MCAST_INCLUDE;// MCAST_EXCLUDE; // MCAST_INCLUDE

static void LogErrorA(const char* fmt, ...)
{
    void* lpMsgBuf;
    DWORD dwError = WSAGetLastError();
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    LocalFree(lpMsgBuf);

    va_list ap;
    char buf[MAX_PATH];

    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    std::string msg = buf;
    msg += "\n";
    msg += "ERRCode[";
    msg += std::to_string(dwError);
    msg += "]:";
    msg += (char*)lpMsgBuf;
    //TLog::Get().log(msg);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleA(handle, msg.c_str(), msg.size(), &dwBytesWriten, 0);
}
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
#define LogErrorA(fmt,...)		LogErrorA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )


SOCKET      g_hSendSocket;
SOCKADDR_IN g_SendAddress;

SOCKET      g_hRecvSocket;
SOCKADDR_IN g_RecvAddress;
HWND        g_hWnd;

SOCKADDR_IN g_hMulticastAddr;

int SetMulticast(SOCKET s,  int opt, int val, int af= IPPROTO_IP)
{
    int rc = ::setsockopt(g_hRecvSocket, af,
        opt, (char*)&val, sizeof(val));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("IP_MULTICAST_LOOP");
    }
    return rc;
}
void UDPSocketInit()
{
    // 1) 윈속 초기화
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    g_hSendSocket = socket(AF_INET, SOCK_DGRAM, 0);
    g_hRecvSocket = socket(AF_INET, SOCK_DGRAM, 0);

    int loopval = 1;
    SetMulticast(g_hRecvSocket, IP_MULTICAST_LOOP, loopval);

    BOOL optval2 = TRUE;
    // ip & port 재사용
    setsockopt(g_hRecvSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval2, sizeof(optval2));
    // 전송 데이터 사본 수신.
    bool loop = true;
    setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loop, sizeof(loop));
    int ttl = 3;
    setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));
    // 멀티케스트 데이터를 전송할 로컬 인터페이스를 설정한다.
    //ULONG addr = inet_addr("192.168.0.12");
    //setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&addr, sizeof(addr));

    g_RecvAddress.sin_family = AF_INET;
    g_RecvAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    g_RecvAddress.sin_port = htons(9000);
    int rc = ::bind(g_hRecvSocket,(SOCKADDR*)&g_RecvAddress, sizeof(g_RecvAddress));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("bind");
    }
    else
    {
        //LogErrorA("bind");
    }
    
    g_hMulticastAddr.sin_family = AF_INET;
    g_hMulticastAddr.sin_addr.s_addr = inet_addr("235.7.8.9");
    g_hMulticastAddr.sin_port = htons(9000);

    struct ip_mreq mreq;
    struct ip_mreq_source mreqsrc;
    
    int g_iSouceCnt = 1;
    if (g_Mode == MCAST_INCLUDE)
    {
        for (int i = 0; i < g_iSouceCnt; i++)
        {
            //IP_ADD_SOURCE_MEMBERSHIP 가입하고 특정 주소만 수신 등록.
            //IP_DRAP_SOURCE_MEMBERSHIP 수신 등록된 리스트 제거.
            mreqsrc.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
            mreqsrc.imr_interface.s_addr = inet_addr("192.168.0.12");
            rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
                IP_ADD_SOURCE_MEMBERSHIP, (char*)&mreqsrc, sizeof(mreqsrc));
            if (rc == SOCKET_ERROR)
            {
                LogErrorA("bind");
            }
        }
    }
    else if( g_Mode == MCAST_EXCLUDE)
    {
        //IP_ADD_MEMBERSHIP  가입한다.
        //IP_DROP_MEMBERSHIP 탈퇴한다.
        mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
        mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
        rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
            IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
        if (rc == SOCKET_ERROR)
        {
            LogErrorA("bind");
        }
        for (int i = 0; i < g_iSouceCnt; i++)
        {
            //IP_BLOCK_SOURCE    : 가입하고 특정 주소로부터 전송된 데이터 제외하고 수신.
            //IP_UNBLOCK_SOURCE  : 수신할 주소의 리스트에서 특정 주소를 추가한다.
            mreqsrc.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
            mreqsrc.imr_interface.s_addr = inet_addr("192.168.0.12");
            mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.54");
            rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
                IP_BLOCK_SOURCE, (char*)&mreqsrc, sizeof(mreqsrc));
            if (rc == SOCKET_ERROR)
            {
                LogErrorA("bind");
            }
        }
    }   
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
        char SendBuf[2048] = "KGCA";

        while (1)
        {
            int iSendRet = sendto(g_hSendSocket, SendBuf, strlen(SendBuf), 0,
                (SOCKADDR*)&g_hMulticastAddr,
                sizeof(g_hMulticastAddr));
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

    //t.join();
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
