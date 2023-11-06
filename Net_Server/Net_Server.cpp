#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
const short port = 10000;
// (1)socket
int main()
{
    // 1) 윈속 초기화
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //                           IP                    TCP
    //SOCKET sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    //error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr("192.168.0.12");
    sa.sin_port = htons(port);

    int ret = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
    ret = listen(sock, SOMAXCONN);   
    SOCKADDR_IN clientaddr;
    SOCKET clientsock;
    printf("서버 정상 작동 시작!!\n");
    while (1) {       
        int addlen = sizeof(clientaddr);
        clientsock = accept(sock, (SOCKADDR*)&clientaddr, &addlen);
        printf("클라이언트 접속 ip=%s, Port:%d\n",
            inet_ntoa(clientaddr.sin_addr), 
            ntohs(clientaddr.sin_port));      
        while (1)
        {
            char buf[256] = { 0, };
            int iRecvByte=recv(clientsock, buf, 256, 0);
            if (iRecvByte == 0) break; // 정상종료
            if (clientsock == SOCKET_ERROR) break;// 비정상 종료
            printf("[받음]%s\n",buf);
        }
        closesocket(clientsock);
        printf("클라이언트 접속해제 ip=%s, Port:%d\n",
            inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port));
    }
    closesocket(sock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
