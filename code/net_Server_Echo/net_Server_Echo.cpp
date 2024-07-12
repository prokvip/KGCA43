#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
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
    // 전화기
    SOCKET sock = socket(AF_INET, // IP
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
    int ret = bind(sock,(sockaddr*)&sa,namelen);
    // 전화개통
    ret  = listen(sock, SOMAXCONN);

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);

    SOCKET clientSock;
    // 수회기 들었다.접속 된 정보 반환.
    clientSock = accept(sock, (sockaddr*)&addr, &addrlen);
    if (ret)
    {
        std::cout << "접속 ip=" <<
            inet_ntoa(addr.sin_addr) << "port="
            << ntohs(addr.sin_port) << std::endl;
    }

    int iMsgCounter = 0;
    while (1)
    {
        char buf[256] = { 0, };        
        // 받고
        ret = recv(clientSock,
              buf,
              sizeof(char)*256,
                0);
        std::cout << "받고" << ret << buf << std::endl;
        // 보내고
        ret = send(clientSock, buf, strlen(buf), 0);
        std::cout << iMsgCounter++ <<"보내고" << ret << buf << std::endl;
    }

    closesocket(clientSock);
    closesocket(sock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
