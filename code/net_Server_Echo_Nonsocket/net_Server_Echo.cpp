#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
#pragma comment(lib,"ws2_32.lib")
bool InitWinSock()
{
    //_In_ WORD wVersionRequested,
    WSADATA WSAData;
    int ret = WSAStartup(MAKEWORD(2,2),&WSAData);
    if (ret != 0) return false;
    return true;
}
void DelWinSock()
{
    WSACleanup();
}
int main()
{
    InitWinSock();
    // 전화기, 블록형 소켓 -> 넌블록형 소켓으로 전환
    SOCKET g_hListenSock = socket(AF_INET, // IP
           SOCK_STREAM,  //SOCK_DGRAM
            IPPROTO_TCP); //,IPPROTO_TCP,IPPROTO_UDP

    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    //sa.sin_addr.s_addr = inet_addr("192.168.0.12"); //INADDR_ANY;
    inet_pton(AF_INET, "192.168.0.12", &sa.sin_addr.s_addr);
    sa.sin_port = htons(10000);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    // 소켓과  IP+PORT 연결한다. 전화번호 부여 받는다.
    int ret = bind(g_hListenSock,(sockaddr*)&sa,namelen);
    // 전화개통
    ret  = listen(g_hListenSock, SOMAXCONN);

    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(g_hListenSock, FIONBIO, &iNonSocket);


    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);    

    int iMsgCounter = 0;

    std::list<SOCKET> g_sockList;
    SOCKET clientSock;
    while (1)
    {        
        // 수회기 들었다.접속 된 정보 반환.
        // 넌블록형 함수
        // 1)해당 이벤트가 발생하면 반환한다.
        // 2)해당 이벤트가 발생하지 않아도 반환한다.
        // 3)에러가 발생시 반환.
        clientSock = accept(g_hListenSock, (sockaddr*)&addr, &addrlen);
        if (clientSock == INVALID_SOCKET)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }            
        }
        else
        {
            std::cout << "접속 ip=" <<
                inet_ntoa(addr.sin_addr) << "port="
                << ntohs(addr.sin_port) << std::endl;

            g_sockList.emplace_back(clientSock);
        }

        for( auto iter = g_sockList.begin();
             iter != g_sockList.end();
              )
        {
            SOCKET sock = *iter;
            char buf[256] = { 0, };
            // 받고
            int RecvByte = recv(sock,
                buf,
                sizeof(char) * 256,
                0);
            if (RecvByte == 0)
            {
                closesocket(sock);
                std::cout << "정상 접속종료 : " << inet_ntoa(addr.sin_addr) << std::endl;
                iter = g_sockList.erase(iter);
                continue;
            }
            if (RecvByte < 0)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    closesocket(sock);
                    std::cout << "비정상 서버 종료!" << std::endl;
                    iter = g_sockList.erase(iter);                    
                }
                continue;
            }
            if (RecvByte > 0)
            {
                std::cout << "받고" << ret << buf << std::endl;
                // 보내고
                int SendByte = send(sock, buf, strlen(buf), 0);
                if (SendByte < 0)
                {
                    int iError = WSAGetLastError();
                    if (iError != WSAEWOULDBLOCK)
                    {
                        closesocket(sock);
                        std::cout << "비정상 서버 종료!" << std::endl;
                        iter = g_sockList.erase(iter);
                        continue;
                    }
                }
                //std::cout << iMsgCounter++ << "보내고" << ret << buf << std::endl;
            }
        }
   }

    closesocket(clientSock);
    closesocket(g_hListenSock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
