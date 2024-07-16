#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <thread>
#include "TProtocol.h"
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

    while (1)
    {
        Sendbuf.clear();
        Sendbuf = UserName;
        //std::cout << "데이터입력(종료:엔터) : ";
        std::getline(std::cin, KeyDatabuf);
        if (KeyDatabuf.empty())
        {
            break;
        }
        Sendbuf += KeyDatabuf;


        UPACKET packet;
        ZeroMemory(&packet, sizeof(packet));
        packet.ph.len = PACKET_HEADER_SIZE + Sendbuf.size();
        packet.ph.type = PACKET_CHAT_MSG;
        memcpy(packet.msg, Sendbuf.c_str(), Sendbuf.size());

        //int SendByte = send(sock, Sendbuf.c_str(), Sendbuf.size(), 0);
        int SendByte = send(sock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
            continue;
        }
        //std::cout << "보내고" << SendByte << Sendbuf.c_str() << std::endl;
    }

    closesocket(sock);
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
    sendThread.detach();

   std::string Recvbuf;
   Recvbuf.resize(256);
    
    while (1)
    {
        //UPACKET packet;
        //ZeroMemory(&packet, sizeof(packet));

        //int RecvByte = recv(sock, &session.buf[session.iBeginPos], PACKET_HEADER_SIZE - session.iBeginPos, 0);
        //if (RecvByte == 0)
        //{
        //    closesocket(sock);
        //    std::cout << "정상 접속종료 : " << inet_ntoa(addr.sin_addr) << std::endl;
        //    session.bDisConnected = true;
        //    continue;
        //}
        //if (RecvByte < 0)
        //{
        //    int iError = WSAGetLastError();
        //    if (iError != WSAEWOULDBLOCK)
        //    {
        //        closesocket(sock);
        //        std::cout << "비정상 서버 종료!" << std::endl;
        //        session.bDisConnected = true;
        //    }
        //    continue;
        //}

        //session.iBeginPos += RecvByte;

        //if (session.iBeginPos == PACKET_HEADER_SIZE)
        //{
        //    memcpy(&packet, &session.buf[0], PACKET_HEADER_SIZE);
        //    while (packet.ph.len > session.iBeginPos)
        //    {
        //        int dataByte = recv(sock,
        //            &session.buf[0], packet.ph.len - session.iBeginPos, 0);
        //        session.iBeginPos += dataByte;
        //    }
        //    if (session.iBeginPos == packet.ph.len)
        //    {
        //        memcpy(&packet.msg, &session.buf[0], packet.ph.len - PACKET_HEADER_SIZE);
        //    }
        //}

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
            std::cout << Recvbuf << std::endl;
        }                
    }

    //sendThread.join();

    closesocket(sock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
