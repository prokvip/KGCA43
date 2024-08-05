#include <winsock2.h> // windows.h 선단에 배치되어야 함.
#include <windows.h>

#include <iostream>
#pragma comment(lib, "ws2_32.lib")

SOCKET m_hSock;
void Create(USHORT port)
{
    m_hSock = socket(AF_INET, SOCK_DGRAM, 0);
    //// 넌블록형 소켓으로 전환
    //u_long iNonSocket = TRUE;
    //int iMode = ioctlsocket(m_hSock, FIONBIO, &iNonSocket);
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    Create(9000);

    char broadcast = '1';
    setsockopt(m_hSock, SOL_SOCKET, SO_BROADCAST, &broadcast, 
        sizeof(broadcast));

    SOCKADDR_IN recv_addr;
    ZeroMemory(&recv_addr, sizeof(SOCKADDR_IN));
    recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
    recv_addr.sin_port = htons(9000);
    recv_addr.sin_family = AF_INET;

    char msg[256] = "kgca";
    sockaddr_in sender_addr;
    int len = sizeof(sender_addr);
    
    sendto(m_hSock, msg, strlen(msg), 0, (sockaddr*)&recv_addr, sizeof(recv_addr));
    recvfrom(m_hSock, msg, 256, 0, (sockaddr*)&sender_addr, &len);
   
    closesocket(m_hSock);

    while (1)
    {
        Sleep(1000);
    }
}