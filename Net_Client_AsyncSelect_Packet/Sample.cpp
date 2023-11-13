#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Net_Client.cpp"
#include "TProtocol.h"
#include "tchar.h" // _tcscpy
#include <list>
SOCKET sock;
HWND g_hWnd;
bool g_bConnect = false;
HWND g_hEdit;
HWND g_hButton;
HWND g_hList;

UPACKET  g_RecvPacket;
UPACKET  g_SendPacket;
CHAT_MSG g_chatMsg;

int g_iRecvByte = 0;
int g_iSendByte = 0;
std::list<std::string> g_szMsgList;

void   ResetRecvPacket()
{
    ZeroMemory(&g_RecvPacket, sizeof(g_RecvPacket));
    g_iRecvByte = 0;
}
void   ResetSendPacket()
{
    ZeroMemory(&g_SendPacket, sizeof(g_SendPacket));
    g_iSendByte = 0;
}
bool   MakePacket(const char* msg, int iSize, WORD type) 
{
    g_SendPacket.ph.len = PACKET_HEADER_SIZE + iSize;
    g_SendPacket.ph.type = type;
    memcpy(g_SendPacket.msg, msg, iSize);
    return true; 
}
bool   MakeChatMessage(const char* msg, int iSize)
{
    ZeroMemory(&g_chatMsg, sizeof(g_chatMsg));
    g_chatMsg.iID = 9;
    char szName[] = "홍길동";
    strcpy(g_chatMsg.szName, szName);
    memcpy(g_chatMsg.szMsg, msg, iSize);    
    return true;
}

int SendPacket(SOCKET sock)
{	
	int iSendByte = 0;
	int iTotalSendByte = 0;
    char* sendbuffer = (char*)&g_SendPacket;
	do {
		iSendByte = send(sock, &sendbuffer[iTotalSendByte],
			                   g_SendPacket.ph.len- iTotalSendByte, 0);
        if (iSendByte == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                return -1;
            }
        }
        else
        {
            if (iSendByte == 0)
            {
                return -1;
            }
            iTotalSendByte += iSendByte;
        }
	} while (iTotalSendByte< g_SendPacket.ph.len);

    ResetSendPacket();
	return iSendByte;
}

int RecvHeader(SOCKET sock, UPACKET& packet)
{
    char* buffer = (char*)&packet;
    int iRet = 0;
    iRet = recv(sock, &buffer[g_iRecvByte],
        PACKET_HEADER_SIZE - g_iRecvByte, 0);
    // 오류체크
    if (iRet == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            closesocket(sock);
            return iRet;
        }
    }
    else
    {
        g_iRecvByte += iRet;
    }
    return g_iRecvByte;
}
int RecvPacket(SOCKET sock, UPACKET& packet)
{
    if (g_iRecvByte < PACKET_HEADER_SIZE)
    {
        RecvHeader(sock, packet);
    }
    // RecvHader(sock, packet)
    char* buffer = (char*)&packet;      
    // 헤더 해석 가능
    if (g_iRecvByte == PACKET_HEADER_SIZE)
    {
        int iMsgByte = 0;
        do {
            int iRet = recv(sock, &packet.msg[iMsgByte],
                packet.ph.len - PACKET_HEADER_SIZE - iMsgByte, 0);
            // 오류 체크
            if (iRet == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    return iRet;
                }
            }
            else
            {
                if (iRet == 0)
                {
                    closesocket(sock);
                    break;
                }
                iMsgByte += iRet;
            }
        } while (packet.ph.len - PACKET_HEADER_SIZE > iMsgByte);
    }    
    return g_iRecvByte;
}
static int Recvwork()
{    
    int iRecvByte = RecvPacket(sock, g_RecvPacket);
    if( iRecvByte > 0)
    {
        switch (g_RecvPacket.ph.type)
        {
            case PACKET_CHAT_MSG:
            {
                CHAT_MSG* pCharMsg = (CHAT_MSG*)g_RecvPacket.msg;
                std::string msg = pCharMsg->szName;
                msg += "->";
                msg += pCharMsg->szMsg;
                OutputDebugStringA(msg.c_str());
                g_szMsgList.push_back(msg.c_str());
                if (g_szMsgList.size() > 20)
                {
                    g_szMsgList.pop_front();
                }
                SendMessageA(g_hList, LB_RESETCONTENT, 0, 0);
                for (auto msg : g_szMsgList)
                {
                    SendMessageA(g_hList, LB_ADDSTRING, 0, (LPARAM)msg.c_str());
                }
            }break;
        }

        ResetRecvPacket();
    }
    return 1;
}
static int sendwork()
{
    int iSize = sizeof(g_chatMsg);// iLen + 13 + 4;
    char* szCharBuffer = (char*)&g_chatMsg;
    int iSendByte = SendPacket(sock);    
    printf("%d바이트를 전송했습니다", iSendByte);
    return iSendByte;

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 200://button
        {
            char buffer[MAX_PATH] = { 0, };
            SendMessageA(g_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
            MakeChatMessage(buffer, strlen(buffer));
            MakePacket((char*)&g_chatMsg, sizeof(g_chatMsg), PACKET_CHAT_MSG);
            sendwork();
        }break;
        }
    }break;
    case NETWORK_MSG:
    {
        if (WSAGETSELECTERROR(lParam) != 0)
        {
            PostQuitMessage(0);
        }
        WORD dwSelect = WSAGETSELECTEVENT(lParam);
        switch (dwSelect)
        {
        case FD_CONNECT:
        {
            g_bConnect = true;
        }break;
        case FD_CLOSE: 
        {
            g_bConnect = false;
        }break;
        case FD_READ: 
        {
            int iRet =Recvwork();
            // 넌블록킹 소켓.
            if (iRet == -1)
            {
                g_bConnect = false;
                closesocket(sock);
            }
            if (iRet == 0)
            {
                PostMessage(g_hWnd, NETWORK_MSG, sock, FD_READ);
            }
        };
        case FD_WRITE: 
        {
            //sendwork();
        }break;
        }
    }break;
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        int k = 0;
    }break;
    case WM_CREATE: break;
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

    //1번 ) 윈도우 레지스터 클래스 등록
    MyRegisterClass(hInstance);
    //2번 ) 윈도우 생성
    InitInstance(hInstance, nCmdShow);
    /// <summary>
    ///  채팅 UI
    /// </summary>
    DWORD style = WS_CHILD | WS_VISIBLE;
    g_hList = CreateWindow(L"listbox", NULL, style, 10, 10, 400, 500,
        g_hWnd, (HMENU)100, hInstance, NULL);
    g_hButton = CreateWindow(L"button", L"전송", style, 640, 10, 100, 100,
        g_hWnd, (HMENU)200, hInstance, NULL);
    style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
    g_hEdit = CreateWindow(L"edit", NULL, style, 430, 10, 200, 100,
        g_hWnd, (HMENU)300, hInstance, NULL);
    


    // 네트워크 초기화
    mainNetwork();
    SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)L"체팅시작!");

    //3번 ) 윈도우 프로시져 작업
    MSG msg;
    // 기본 메시지 루프입니다:
    //WM_QUIT : FALSE
    //while (GetMessage(&msg, nullptr, 0, 0))
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
            // PreFrame() // recv
            // 게임로직
            // PostFrame() // send
            
        }
    }
    closesocket(sock);
    // )  윈속 해제    
    WSACleanup();

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
