#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
#include "TProtocol.h"
#pragma comment(lib,"ws2_32.lib")
bool InitWinSock()
{
    WSADATA WSAData;
    int ret = WSAStartup(MAKEWORD(2,2),&WSAData);
    if (ret != 0) return false;
    return true;
}
void DelWinSock()
{
    WSACleanup();
}

struct TSession
{
    SOCKET      sock;
    SOCKADDR_IN addr;
    std::string buf;
    UINT        iBeginPos = 0;
    UINT        RecvByte;
    bool        bDisConnected;
    TSession(SOCKET sock, SOCKADDR_IN addr)
    {
        RecvByte = 0;
        this->sock = sock;
        this->addr = addr;
        buf.resize(256);
        bDisConnected = false;
    }
};

std::list<TSession> g_sockList;

void DisConnected(TSession& session)
{
    closesocket(session.sock);
    std::cout << "접속종료 : " << 
        inet_ntoa(session.addr.sin_addr) << std::endl;
}
int main()
{
    InitWinSock();
    SOCKET g_hListenSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, "192.168.0.12", &sa.sin_addr.s_addr);
    sa.sin_port = htons(10000);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = bind(g_hListenSock,(sockaddr*)&sa,namelen);
    ret  = listen(g_hListenSock, SOMAXCONN);
    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(g_hListenSock, FIONBIO, &iNonSocket);

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);    
    int iMsgCounter = 0;
   
    SOCKET clientSock;
    while (1)
    {
        clientSock = accept(g_hListenSock, (sockaddr*)&addr, &addrlen);
        if (clientSock == INVALID_SOCKET)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {
            std::cout << "접속 ip=" << inet_ntoa(addr.sin_addr) << "port=" << ntohs(addr.sin_port) << std::endl;
            g_sockList.emplace_back(clientSock, addr);
        }

        for( auto iter = g_sockList.begin(); iter != g_sockList.end(); iter++)
        {
            TSession& session = (*iter); 
            session.RecvByte = 0;

            SOCKET sock = session.sock;
            char buf[256] = { 0, };
            // 받고

            UPACKET packet;
            ZeroMemory(&packet, sizeof(packet));

            int RecvByte = recv(sock,&session.buf[session.iBeginPos], PACKET_HEADER_SIZE- session.iBeginPos,0);
            if (RecvByte == 0)
            {
                closesocket(sock);
                std::cout << "정상 접속종료 : " << inet_ntoa(addr.sin_addr) << std::endl;
                session.bDisConnected = true;
                continue;
            }
            if (RecvByte < 0)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    closesocket(sock);
                    std::cout << "비정상 서버 종료!" << std::endl;
                    session.bDisConnected = true;
                }
                continue;
            }
            
            session.iBeginPos += RecvByte;

            if (session.iBeginPos == PACKET_HEADER_SIZE)
            {
                memcpy(&packet, &session.buf[0], PACKET_HEADER_SIZE);
                while (packet.ph.len > session.iBeginPos)
                {
                    int dataByte = recv(sock,
                        &session.buf[0], packet.ph.len - session.iBeginPos, 0);
                    session.iBeginPos += dataByte;
                }
                if (session.iBeginPos == packet.ph.len )
                {
                    memcpy(&packet.msg, &session.buf[0], packet.ph.len - PACKET_HEADER_SIZE);
                    session.iBeginPos = 0;
                }
            }

            

            // 정상적인 데이터 수신
           
            /*packet.ph.len = PACKET_HEADER_SIZE + Sendbuf.size();
            packet.ph.type = PACKET_CHAT_MSG;
            memcpy(packet.msg, Sendbuf.c_str(), Sendbuf.size());*/

            session.RecvByte = RecvByte;
            session.buf[RecvByte] = 0;
            if (session.RecvByte > 0)
            {
                if (packet.ph.type == PACKET_CHAT_MSG)
                {
                    std::cout << packet.msg << std::endl;
                }
            }
            // 브로드케스팅
            for (auto iterSend = g_sockList.begin();iterSend != g_sockList.end(); iterSend++)
            {
                TSession& user = (*iterSend);
                if (user.bDisConnected ==false)
                {
                    int SendByte = send(user.sock, (char*)&packet, packet.ph.len, 0);
                    if (SendByte < 0)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            closesocket(user.sock);
                            std::cout << "비정상 서버 종료!" << std::endl;
                            session.bDisConnected = true;                            
                        }
                    }                    
                }              
            }                  
        }
        // 종료
        for (auto iter = g_sockList.begin(); iter != g_sockList.end(); )
        {
            if ((*iter).bDisConnected)
            {
                DisConnected(*iter);
                iter = g_sockList.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

    for (auto iter = g_sockList.begin(); iter != g_sockList.end(); iter++)
    {
        DisConnected(*iter);
    }
    g_sockList.clear();

    closesocket(g_hListenSock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
