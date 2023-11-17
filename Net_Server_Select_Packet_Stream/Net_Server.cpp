#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include "TUser.h"


const short port = 10000;
int g_time = 0;

std::list<TUser>  g_userlist;

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
    for (std::list<TUser>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        )
    {
        SOCKET cSock = (*iterSend).sock;

        int iSendByte = SendPacket(cSock, packet);
        if (iSendByte == SOCKET_ERROR)
        {
            closesocket((*iterSend).sock);
            iterSend = g_userlist.erase(iterSend);
            continue;
        }
        iterSend++;
    }
    return true;
}

int RecvPacket(TUser& user)
{
    char buffer[2048] = { 0, };
    int iRecvByte = 0;
    int iRet = 0;
    SOCKET sock = user.sock;

    iRet = recv(sock, buffer, 2048, 0);
    if (iRet == SOCKET_ERROR)
    {
        return -1;
    }
    if (iRet == 0)
    {
        return 1;
    }
    user.sPacket.Put(buffer, iRet);
    user.sPacket.GetPacket(user);

    // 받은 패킷 처리 루틴
    for (auto packet : user.list)
    {
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
    }  

    user.list.clear();
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

        for( std::list<TUser>::iterator iter = g_userlist.begin();
              iter != g_userlist.end();
              iter++)
        {
            FD_SET((*iter).sock, &readSet);
            FD_SET((*iter).sock, &sendSet);
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
                g_userlist.emplace_back(clientsock, clientaddr);

                printf("클라이언트 접속 ip=%s, Port:%d\n",
                    inet_ntoa(clientaddr.sin_addr),
                    ntohs(clientaddr.sin_port));
            }
        }

        for (std::list<TUser>::iterator iter = g_userlist.begin();
            iter != g_userlist.end();
            )
        {
            if (FD_ISSET((*iter).sock, &readSet))
            {
                TUser& user = *iter;
                SOCKET cSock = (*iter).sock;
                int iRecvByte = RecvPacket(user);
                if (iRecvByte == SOCKET_ERROR)
                {
                    closesocket((*iter).sock);
                    iter = g_userlist.erase(iter); 
                    printf("클라이언트 접속 Socket=%d\n", *iter);                        
                    continue;
                }
                else
                {
                    if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                    {
                        closesocket((*iter).sock);
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
