#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <list>
#include <vector>
const short port = 10000;
int g_time = 0;

int     g_iCurrentEvent = 0;
std::vector<HANDLE>  g_hEventArray(WSA_MAXIMUM_WAIT_EVENTS);
std::vector<SOCKET>  g_hSocketArray(WSA_MAXIMUM_WAIT_EVENTS);
std::list<SOCKET>  g_userlist;

int main()
{
    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    //error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(port);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

    // u_long on = TRUE;
     //ioctlsocket(listensock, FIONBIO, &on);

    SOCKADDR_IN clientaddr;
    SOCKET clientsock;

    std::clock_t start = std::clock();
    printf("서버 정상 작동 시작!!\n");

    g_hSocketArray[g_iCurrentEvent] = listensock;
    g_hEventArray[g_iCurrentEvent] = WSACreateEvent();
    WSAEventSelect(listensock, g_hEventArray[g_iCurrentEvent], FD_ACCEPT | FD_CLOSE);
    g_iCurrentEvent++;

    /// <summary>
    /// select : 매프레임 마다 초기화->세팅->사용(SELECT)
    /// </summary>
    /// <returns></returns>
    /// 
    /// typedef struct _WSANETWORKEVENTS {
        //long lNetworkEvents;
        //int iErrorCode[FD_MAX_EVENTS];
    //} WSANETWORKEVENTS, FAR* LPWSANETWORKEVENTS;

    WSANETWORKEVENTS NetworkEvent;
    bool bRunning = true;
    while (bRunning)
    {
         int iIndex = WSAWaitForMultipleEvents(g_iCurrentEvent, &g_hEventArray.at(0),
             FALSE, 0, FALSE);
         if (iIndex == WSA_WAIT_FAILED) break;
         if (iIndex == WSA_WAIT_TIMEOUT) continue;
         iIndex -= WSA_WAIT_EVENT_0;

         int iEvent = iIndex;
         for (int iEvent = iIndex; iEvent < g_iCurrentEvent; iEvent++)
         {
             int iSignal = WSAWaitForMultipleEvents(1, &g_hEventArray[iEvent],
                 TRUE, 0, FALSE);
             if (iSignal == WSA_WAIT_FAILED)  continue;
             if (iSignal == WSA_WAIT_TIMEOUT) continue;

             int iRet = WSAEnumNetworkEvents(
                 g_hSocketArray[iEvent], g_hEventArray[iIndex], &NetworkEvent);
             if (iRet == SOCKET_ERROR) continue;
             if (NetworkEvent.lNetworkEvents & FD_ACCEPT)
             {
                 if (NetworkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
                 {
                     continue;
                 }
                 int addlen = sizeof(clientaddr);
                 clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
                 if (clientsock == SOCKET_ERROR)
                 {
                     int iError = WSAGetLastError();
                     if (iError != WSAEWOULDBLOCK)
                     {
                         bRunning = false;
                         break;
                     }
                 }
                 else
                 {
                     g_hSocketArray[g_iCurrentEvent] = clientsock;
                     g_hEventArray[g_iCurrentEvent] = WSACreateEvent();
                     WSAEventSelect(clientsock,
                         g_hEventArray[g_iCurrentEvent],
                         FD_READ | FD_WRITE | FD_CLOSE);
                     g_iCurrentEvent++;

                     //g_userlist.push_back(clientsock);
                     printf("클라이언트 접속 ip=%s, Port:%d\n",
                         inet_ntoa(clientaddr.sin_addr),
                         ntohs(clientaddr.sin_port));
                 }
             }
             if (NetworkEvent.lNetworkEvents & FD_READ)
             {
                 SOCKET cSock = g_hSocketArray[iIndex];
                 char buf[256] = { 0, };
                 int iRecvByte = recv(cSock, buf, 256, 0);
                 if (iRecvByte == SOCKET_ERROR)
                 {
                     closesocket(g_hSocketArray[iIndex]);
                     WSACloseEvent(g_hEventArray[iIndex]);
                     // 삽입list   [0]->[3]->[1]->[2]    O(1)
                     // vector [0][1][2][3]          O(n)->O(1)
                     // 삭제list   [0]->[3]->[2]     O(1)
                     // vector [0][3][2][null]       O(n)->O(1)
                     //iter = g_userlist.erase(iter);
                     for (int i = iIndex; i < g_iCurrentEvent; i++)
                     {
                         g_hEventArray[iIndex] = g_hEventArray[iIndex + 1];
                         g_hSocketArray[iIndex] = g_hSocketArray[iIndex + 1];
                     }
                     g_iCurrentEvent--;
                     continue;
                 }
                 else
                 {
                     if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                     {
                         closesocket(g_hSocketArray[iIndex]);
                         WSACloseEvent(g_hEventArray[iIndex]);
                         for (int i = iIndex; i < g_iCurrentEvent; i++)
                         {
                             g_hEventArray[iIndex] = g_hEventArray[iIndex + 1];
                             g_hSocketArray[iIndex] = g_hSocketArray[iIndex + 1];
                         }
                         g_iCurrentEvent--;
                         continue;
                     }

                     printf("[받음]%s\n", buf);
                     int iLen = strlen(buf);

                     // [0][1][2][null]|dfdfd
                     // [0][2][3][null]|dfdfd
                     for (int iterSend = 1; iterSend < g_iCurrentEvent; )
                     {
                         SOCKET cSock = g_hSocketArray[iterSend];
                         if (cSock == 0) continue;
                         int iSendByte = send(cSock, buf, iLen, 0);
                         if (iSendByte == SOCKET_ERROR)
                         {
                             closesocket(g_hSocketArray[iIndex]);
                             WSACloseEvent(g_hEventArray[iIndex]);
                             for (int i = iIndex; i < g_iCurrentEvent; i++)
                             {
                                 g_hEventArray[iIndex] = g_hEventArray[iIndex + 1];
                                 g_hSocketArray[iIndex] = g_hSocketArray[iIndex + 1];
                             }
                             g_iCurrentEvent--;
                             continue;
                         }
                         iterSend++;
                     }
                 }
             }
             if (NetworkEvent.lNetworkEvents & FD_WRITE)
             { 
                 if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
                 {
                     continue;
                 }
             }
             if (NetworkEvent.lNetworkEvents & FD_CLOSE)
             {
                 if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
                 {
                     bRunning = false;
                     continue;
                 }
             }
         }
    }
    
    closesocket(listensock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
