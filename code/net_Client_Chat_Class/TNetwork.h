#pragma once
#include "TNetStd.h"

struct TSession
{
    SOCKET      sock;
    SOCKADDR_IN addr;
    std::string buf;
    UINT        iBeginPos = 0;
    //UINT        RecvByte;
    bool        bDisConnected;
    UPACKET     packet;
    bool        Recv()
    {    
        // 받고       
        ZeroMemory(&packet, sizeof(packet));

        int RecvByte = recv(sock, &buf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
        if (RecvByte == 0)
        {
            closesocket(sock);
            std::cout << "정상 접속종료 : " << inet_ntoa(addr.sin_addr) << std::endl;
            bDisConnected = true;
            return false;
        }
        if (RecvByte < 0)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                closesocket(sock);
                std::cout << "비정상 서버 종료!" << std::endl;
                bDisConnected = true;
            }
            return false;
        }

        iBeginPos += RecvByte;

        if (iBeginPos == PACKET_HEADER_SIZE)
        {
            memcpy(&packet, &buf[0], PACKET_HEADER_SIZE);
            while (packet.ph.len > iBeginPos)
            {
                int dataByte = recv(sock, &buf[0], packet.ph.len - iBeginPos, 0);
                iBeginPos += dataByte;
            }
            if (iBeginPos == packet.ph.len)
            {
                memcpy(&packet.msg, &buf[0], packet.ph.len - PACKET_HEADER_SIZE);
                iBeginPos = 0;
            }
        }
        else
        {         
            return false;
        }        
        return true;
    }
    void        SendPacket(UPACKET& packet)
    {
        if (bDisConnected == false)
        {
            int SendByte = send(sock, (char*)&packet, packet.ph.len, 0);
            if (SendByte < 0)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    closesocket(sock);
                    std::cout << "비정상 서버 종료!" << std::endl;
                    bDisConnected = true;
                }
            }
        }
    }
    TSession(SOCKET sock, SOCKADDR_IN addr)
    {
        //RecvByte = 0;
        this->sock = sock;
        this->addr = addr;
        buf.resize(256);
        bDisConnected = false;
    }
};

class TNetwork
{
public:
    SOCKET              g_hSock;
    bool                m_bRun = true;
protected:    
    std::list<TSession> g_sockList;   
public:
    bool InitWinSock();
    void DelWinSock();
public:
    static UPACKET MakePacket( short len,short type, const char*  msg=nullptr);
    int  SendPacket(SOCKET sock, UPACKET& packet);
    int  SendPacket(UPACKET& packet);
    int  SendPacket(TSession& user, UPACKET& packet);
public:
    virtual void PacketProcess(UPACKET& packet);
    virtual void CheckConnected();
    virtual void AddSession(SOCKET sock, SOCKADDR_IN addr);
    virtual void Broadcast(UPACKET& packet);
    virtual void DisConnected(TSession& session);
    virtual void Create(std::string ip, USHORT port=10000);
    virtual bool Connected(std::string ip, USHORT port = 10000);
    virtual void Run();
    virtual void Release();
public:
    TNetwork();
    virtual ~TNetwork();
};

class TNetworkServer : public TNetwork
{

};

class TNetworkClient : public TNetwork
{
public:
    void   Run() override
    {
        std::string Recvbuf;
        Recvbuf.resize(256);

        UINT  iBeginPos = 0;
        bool  bRun = true;

        while (bRun)
        {
            UPACKET packet;
            ZeroMemory(&packet, sizeof(packet));

            int RecvByte = recv(g_hSock, &Recvbuf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
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
                continue;
            }

            iBeginPos += RecvByte;

            if (iBeginPos == PACKET_HEADER_SIZE)
            {
                memcpy(&packet, &Recvbuf[0], PACKET_HEADER_SIZE);
                while (packet.ph.len > iBeginPos)
                {
                    int dataByte = recv(g_hSock, &Recvbuf[0], packet.ph.len - iBeginPos, 0);
                    if (RecvByte == 0)
                    {
                        std::cout << "서버 종료!" << std::endl;
                        bRun = false;
                        break;
                    }
                    if (dataByte < 0)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            std::cout << "비정상 서버 종료!" << std::endl;
                            bRun = false;
                            break;
                        }
                    }
                    iBeginPos += dataByte;
                }
                if (bRun)
                {
                    if (iBeginPos == packet.ph.len)
                    {
                        memcpy(&packet.msg, &Recvbuf[0], packet.ph.len - PACKET_HEADER_SIZE);
                    }
                    if (packet.ph.type == PACKET_CHAT_MSG)
                    {
                        std::cout << packet.msg << std::endl;
                    }
                    iBeginPos = 0;
                }
            }
        }
    }
};