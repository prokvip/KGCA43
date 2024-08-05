#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // windows.h 선단에 배치되어야 함.
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

SOCKET m_hSock;
int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    m_hSock = socket(AF_INET, SOCK_DGRAM, 0);
  
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(9000);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = bind(m_hSock, (sockaddr*)&sa, namelen);

    if (ret < 0)   {        return 0;    }
    char msg[256] = { 0, };
    sockaddr_in sender_addr;
    int len = sizeof(sender_addr);
    while (1)
    {
        recvfrom(m_hSock, msg, 256, 0, (sockaddr*)&sender_addr, &len);
        printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
            	inet_ntoa(sender_addr.sin_addr),
            	ntohs(sender_addr.sin_port));
        sendto(m_hSock, msg, strlen(msg), 0, (sockaddr*)&sender_addr, sizeof(sender_addr));
    }
    closesocket(m_hSock);
}