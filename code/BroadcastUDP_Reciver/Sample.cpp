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
    char broadcast = '1';
    setsockopt(m_hSock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    
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
    recvfrom(m_hSock, msg, 256, 0, (sockaddr*)&sender_addr, &len);
    sendto(m_hSock, msg, strlen(msg), 0, (sockaddr*)&sender_addr, sizeof(sender_addr));
    closesocket(m_hSock);
}