#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include "TProtocol.h"


/// RECV => 데이터 -> 네트워크카드 -> 운영체제 -> 프로세스(recvbuffer) -> recv()
/// SEND => 데이터 -> send() -> 프로세스(sendbuffer) -> 운영체제 -> 네트워크카드



const short port = 10000;
int g_time = 0;
std::list<UPACKET>  g_userPacketlist;
std::list<SOCKET>  g_userlist;
//int SendPacket(SOCKET sock, const char* msg, int iSize, WORD type)
//{
//    UPACKET sendpacket;
//    sendpacket.ph.len = PACKET_HEADER_SIZE + iSize;
//    sendpacket.ph.type = type;
//    memcpy(sendpacket.msg, msg, iSize);
//    char* sendbuffer = (char*)&sendpacket;
//
//    int iSendByte = 0;
//    int iTotalSendByte = 0;
//    do {
//        iSendByte = send(sock, &sendbuffer[iTotalSendByte],
//            sendpacket.ph.len - iTotalSendByte, 0);
//        iTotalSendByte += iSendByte;
//    } while (iTotalSendByte < sendpacket.ph.len);
//    return sendpacket.ph.len;
//}
int SendPacket(SOCKET sock, UPACKET& packet)
{    
    char* sendbuffer = (char*)&packet;

    int iSendByte = 0;
    int iTotalSendByte = 0;
    do {
        iSendByte = send(sock, &sendbuffer[iTotalSendByte],
            packet.ph.len - iTotalSendByte, 0);
        iTotalSendByte += iSendByte;
    } while (iTotalSendByte < packet.ph.len);
    return packet.ph.len;
}
bool Broadcastting(UPACKET& packet)
{
    for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        )
    {
        SOCKET cSock = *iterSend;

        int iSendByte = SendPacket(cSock, packet);
        if (iSendByte == SOCKET_ERROR)
        {
            closesocket(*iterSend);
            iterSend = g_userlist.erase(iterSend);
            continue;
        }
        iterSend++;
    }
    return true;
}

int RecvPacket(SOCKET sock)
{
    char buffer[256] = { 0, };
    int iRecvByte = 0;
    int iRet = 0;
    while (1)
    {
        iRet = recv(sock, &buffer[iRecvByte],
            PACKET_HEADER_SIZE - iRecvByte, 0);
        // 오류체크
        if (iRet == 0) break;
        if (iRet == SOCKET_ERROR) break;
        iRecvByte += iRet;
        if (iRet != PACKET_HEADER_SIZE) continue;

        // 헤더 해석 가능
        if (iRet == PACKET_HEADER_SIZE)
        {
            UPACKET packet;
            ZeroMemory(&packet, sizeof(packet));
            memcpy(&packet.ph, buffer, PACKET_HEADER_SIZE);

            int iMsgByte = 0;

            do {
                iRet = recv(sock, &packet.msg[iMsgByte],
                    packet.ph.len - PACKET_HEADER_SIZE - iMsgByte, 0);
                
                if (iRet == 0) break;
                // 오류 체크
                if (iRet == SOCKET_ERROR) break;
                iMsgByte += iRet;
            } while (packet.ph.len - PACKET_HEADER_SIZE > iMsgByte);

            //g_userPacketlist.push_back(packet);
            if (iRet != SOCKET_ERROR)
            {
                switch (packet.ph.type)
                {
                case PACKET_CHAT_MSG:
                {
                    // 패킷처리
                    Broadcastting(packet);
                }break;
                }
            }
            break;
        }
    }
    return iRet;
}


int main()
{  
    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);

    //int socktype = 0;
    //int istLen = sizeof(socktype);
    //getsockopt(listensock, SOL_SOCKET, SO_TYPE, (char*)&socktype, &istLen);
    ////if(SOCK_STREAM == socktype) //tcp socket

    int sndbufsize = 0;
    int iLen = sizeof(sndbufsize);
    getsockopt(listensock, SOL_SOCKET, SO_SNDBUF, (char*)&sndbufsize, &iLen);

    int recvbufsize = 0;
    int iRecvLen = sizeof(recvbufsize);
    getsockopt(listensock, SOL_SOCKET, SO_RCVBUF, (char*)&recvbufsize, &iRecvLen);

   /* sndbufsize = 10000;
    recvbufsize = 10000;
    setsockopt(listensock, SOL_SOCKET, SO_SNDBUF, (char*)&sndbufsize, sizeof(sndbufsize));
    setsockopt(listensock, SOL_SOCKET, SO_RCVBUF, (char*)&recvbufsize, sizeof(recvbufsize));
    
    getsockopt(listensock, SOL_SOCKET, SO_SNDBUF, (char*)&sndbufsize, &iLen);
    getsockopt(listensock, SOL_SOCKET, SO_RCVBUF, (char*)&recvbufsize, &iRecvLen);*/


    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(port);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

   // u_long on = TRUE;
    //ioctlsocket(listensock, FIONBIO, &on);

    SOCKADDR_IN clientaddr;
    SOCKET clientsock;

    std::clock_t start = std::clock();
    printf("서버 정상 작동 시작!!\n");

    /// <summary>
    /// select : 매프레임 마다 초기화->세팅->사용(SELECT)
    /// </summary>
    /// <returns></returns>
    FD_SET readSet;
    FD_SET sendSet;
    FD_SET execptSet;
    timeval timeout;
    timeout.tv_sec = 1.0f;
    timeout.tv_usec = 0;

    while (1) 
    {    
        FD_ZERO(&readSet);
        FD_ZERO(&sendSet);
        FD_ZERO(&execptSet);
        FD_SET(listensock, &readSet);

        for( std::list<SOCKET>::iterator iter = g_userlist.begin();
              iter != g_userlist.end();
              iter++)
        {
            FD_SET(*iter, &readSet);
            FD_SET(*iter, &sendSet);
        }
        iRet = select(0, &readSet, &sendSet, NULL, &timeout);
        if (iRet == SOCKET_ERROR) break;
        if (iRet == 0)
        {
            continue;
        }
        //accept
        if (FD_ISSET(listensock, &readSet))
        {
            int addlen = sizeof(clientaddr);
            clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
            if (clientsock == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    break;
                }
            }
            else
            {
                g_userlist.push_back(clientsock);
                printf("클라이언트 접속 ip=%s, Port:%d\n",
                    inet_ntoa(clientaddr.sin_addr),
                    ntohs(clientaddr.sin_port));
            }
        }

        for (std::list<SOCKET>::iterator iter = g_userlist.begin();
            iter != g_userlist.end();
            )
        {
            if (FD_ISSET(*iter, &readSet))
            {
                SOCKET cSock = *iter;
                int iRecvByte = RecvPacket(cSock);
                if (iRecvByte == SOCKET_ERROR)
                {
                    closesocket(*iter);
                    iter = g_userlist.erase(iter); 
                    printf("클라이언트 접속 Socket=%d\n", *iter);                        
                    continue;
                }
                else
                {
                    if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                    {
                        closesocket(*iter);
                        printf("클라이언트 접속 Socket=%d\n", *iter);
                        iter = g_userlist.erase(iter);
                        continue;
                    }                                
                }
            }           
            iter++;
        }       

    }
    closesocket(listensock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
