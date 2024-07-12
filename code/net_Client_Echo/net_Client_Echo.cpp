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
    while (1)
    {
        Sleep(1000);
        ret  = send(sock, buf,strlen(buf),0);
        std::cout << "보내고" << ret << buf << std::endl;

        ret = recv(sock, buf, sizeof(char) * 256,0);
        std::cout << "받고" << ret << buf << std::endl;
    }
    closesocket(sock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
