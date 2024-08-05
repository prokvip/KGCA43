#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // windows.h 선단에 배치되어야 함.
#include <windows.h>

#include <iostream>
#include <vector>
#include <string>
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

    bool bBroadcast = false;
    if (bBroadcast)
    {
        char usdBroadcast = '1';
        setsockopt(m_hSock, SOL_SOCKET, SO_BROADCAST, &usdBroadcast, sizeof(usdBroadcast));
    }    

    SOCKADDR_IN recv_addr;
    ZeroMemory(&recv_addr, sizeof(SOCKADDR_IN));
    recv_addr.sin_port = htons(9000);
    recv_addr.sin_family = AF_INET;

    int namelen = sizeof(recv_addr);
    int ret = bind(m_hSock, (sockaddr*)&recv_addr, namelen);

    if (ret < 0) { return 0; }

    char msgListenServer[256] = "ListenServer";
    sockaddr_in sender_addr;
    int len = sizeof(sender_addr);


    std::vector<std::string> userIPList;
    userIPList.push_back("192.168.0.92");

    if (bBroadcast)
    {        
        recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
        // 너 호스트야?
        sendto(m_hSock, msgListenServer, strlen(msgListenServer), 0, (sockaddr*)&recv_addr, sizeof(recv_addr));

        char data[256] = { 0, };
        recvfrom(m_hSock, data, 256, 0, (sockaddr*)&sender_addr, &len);
        userIPList.push_back(data);
    }
    else
    {
        recv_addr.sin_addr.s_addr = inet_addr("192.168.0.92");
    }

   
    char msg[256] = { 0, };
    if (bBroadcast)
    {
        char ping[256] = "data";
        sendto(m_hSock, ping, strlen(ping), 0, (sockaddr*)&recv_addr, sizeof(recv_addr));
    }
    else
    {       
        for (int iUser = 0; iUser < userIPList.size(); iUser++)
        {
            recv_addr.sin_addr.s_addr = inet_addr(userIPList[iUser].c_str());
            sendto(m_hSock, msg, strlen(msg), 0, (sockaddr*)&recv_addr, sizeof(recv_addr));
        }
    }
    recvfrom(m_hSock, msg, 256, 0, (sockaddr*)&sender_addr, &len);
   
    closesocket(m_hSock);

    while (1)
    {
        Sleep(1000);
    }
}