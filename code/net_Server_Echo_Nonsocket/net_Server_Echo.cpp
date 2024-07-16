#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
#pragma comment(lib,"ws2_32.lib")
bool InitWinSock()
{
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
    SOCKET g_hListenSock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP); 

    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    //sa.sin_addr.s_addr = inet_addr("192.168.0.12"); //INADDR_ANY;
    inet_pton(AF_INET, "192.168.0.12", &sa.sin_addr.s_addr);
    sa.sin_port = htons(10000);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = bind(g_hListenSock,(sockaddr*)&sa,namelen);
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

        for( auto iter = g_sockList.begin(); iter != g_sockList.end();)
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
                    continue;
                }
                else
                {
                    iter++;
                    continue;
                }
            }
            if (RecvByte > 0)
            {
                std::cout << "받고" << RecvByte << buf << std::endl;
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
            iter++;
        }
   }

    closesocket(clientSock);
    closesocket(g_hListenSock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
