#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
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
    UINT        RecvByte;
    TSession(SOCKET sock, SOCKADDR_IN addr)
    {
        RecvByte = 0;
        this->sock = sock;
        this->addr = addr;
        buf.resize(256);
    }
};

std::list<TSession> g_sockList;

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
            std::cout << "접속 ip=" <<
                inet_ntoa(addr.sin_addr) << "port="
                << ntohs(addr.sin_port) << std::endl;

            g_sockList.emplace_back(clientSock, addr);
        }

        for( auto iter = g_sockList.begin(); iter != g_sockList.end();)
        {
            TSession& session = (*iter); 
            session.RecvByte = 0;

            SOCKET sock = session.sock;
            char buf[256] = { 0, };
            // 받고
            int RecvByte = recv(sock,&session.buf[0],session.buf.size(),0);
            if (RecvByte == 0)
            {
                closesocket(sock);
                std::cout << "정상 접속종료 : " << inet_ntoa(addr.sin_addr) << std::endl;
                iter = g_sockList.erase(iter);
                continue;
            }
            if (RecvByte < 0)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    closesocket(sock);
                    std::cout << "비정상 서버 종료!" << std::endl;
                    iter = g_sockList.erase(iter); 
                    continue;
                }
                else
                {
                    iter++;
                    continue;
                }
            }

            // 정상적인 데이터 수신
            session.RecvByte = RecvByte;
            if (session.RecvByte > 0)
            {
                std::cout << session.buf.c_str() << std::endl;
            }

            for (auto iterSend = g_sockList.begin();
                iterSend != g_sockList.end();
                )
            {
                TSession& user = (*iterSend);
                if (session.RecvByte > 0)
                {
                    int SendByte = send(user.sock, session.buf.c_str(), session.RecvByte, 0);
                    if (SendByte < 0)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            closesocket(user.sock);
                            std::cout << "비정상 서버 종료!" << std::endl;
                            iterSend = g_sockList.erase(iterSend);
                            continue;
                        }
                    }                    
                }
                iterSend++;
            }      
            
        }        
   }

    closesocket(clientSock);
    closesocket(g_hListenSock);
    DelWinSock();
    std::cout << "Hello World!\n";
}
