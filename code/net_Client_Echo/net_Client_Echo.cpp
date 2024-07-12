#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
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
int main()
{
    InitWinSock();
    SOCKET sock = socket(AF_INET, // IP
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

    char buf[256] = "안녕하세여~";
    
    int iSendCounter = 10;
    while (iSendCounter-- > 0)
    {
        Sleep(1000);
        int SendByte  = send(sock, buf,strlen(buf),0);
        std::cout << "보내고" << SendByte << buf << std::endl;

        int RecvByte = recv(sock, buf, sizeof(char) * 256,0);
        if (RecvByte == 0)
        {
            std::cout << "서버 종료!" << std::endl;
            break;
        }
        if (RecvByte < 0)
        {
            std::cout << "비정상 서버 종료!" << std::endl;
            break;
        }
        std::cout << "받고" << RecvByte << buf << std::endl;
    }
    closesocket(sock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
