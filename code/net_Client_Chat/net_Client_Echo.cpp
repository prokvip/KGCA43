#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib,"ws2_32.lib")
bool InitWinSock()
{
    //_In_ WORD wVersionRequested,
    WSADATA WSAData;
    int ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if (ret != 0) return false;
    return true;
}
void DelWinSock()
{
    WSACleanup();
}
SOCKET sock;
void SenderThread()
{
    std::string UserName = "[홍길동]";
    std::string Sendbuf;
    Sendbuf.reserve(256);
    std::string KeyDatabuf;
    KeyDatabuf.reserve(256);

    //int iSendCounter = 10;
    while (1)
    {
        Sendbuf.clear();
        Sendbuf = UserName;
        std::cout << "데이터입력(종료:엔터) : ";
        std::getline(std::cin, KeyDatabuf);
        if (KeyDatabuf.empty())
        {
            continue;
        }
        Sendbuf += KeyDatabuf;

        int SendByte = send(sock, Sendbuf.c_str(), Sendbuf.size(), 0);
        if (SendByte < 0)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
            continue;
        }
        std::cout << "보내고" << SendByte << Sendbuf.c_str() << std::endl;
    }
}
int main()
{
    InitWinSock();
    sock = socket(AF_INET, // IP
        SOCK_STREAM,  //SOCK_DGRAM
        IPPROTO_TCP); //,IPPROTO_TCP,IPPROTO_UDP

    SOCKADDR_IN sa;    
    //inet_pton , InetPton
    sa.sin_addr.s_addr = inet_addr("192.168.0.12");
    sa.sin_port = htons(10000); //0  ~ 1024
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = connect(sock,  (sockaddr*)&sa,    namelen);
    if (ret != 0) return 1;

    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(sock, FIONBIO, &iNonSocket);

    // 종업원 1명
    std::thread sendThread(SenderThread);
   
   std::string Recvbuf;
   Recvbuf.resize(256);
    
    while (1)
    {
        int RecvByte = recv(sock, &Recvbuf[0],Recvbuf.size(),0);
        if (RecvByte == 0)
        {
            std::cout << "서버 종료!" << std::endl;
            break;
        }
        if (RecvByte < 0)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                std::cout << "비정상 서버 종료!" << std::endl;
                break;
            }
        }
        else
        {
            Recvbuf[RecvByte] = 0;
            std::cout << Recvbuf.c_str() << std::endl;
        }
    }

    sendThread.join();

    closesocket(sock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
