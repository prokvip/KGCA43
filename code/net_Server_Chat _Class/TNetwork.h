#pragma once
#include "TSession.h"
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
    static bool    CheckError();
    static UPACKET MakePacket(short len, short type, const char* msg = nullptr);
    int  SendPacket(SOCKET sock, UPACKET& packet);
    int  SendPacket(UPACKET& packet);
    int  SendPacket(TSession& user, UPACKET& packet);
public:
    virtual void PacketProcess(UPACKET& packet);
    virtual void CheckConnected();
    virtual void AddSession(SOCKET sock, SOCKADDR_IN addr);
    virtual void Broadcast(UPACKET& packet);
    virtual void DisConnected(TSession& session);
    virtual void Create(std::string ip, USHORT port = 10000);
    virtual bool Connected(std::string ip, USHORT port = 10000);
    virtual void Run();
    virtual void Release();
     
public:
    TNetwork();
    virtual ~TNetwork();
};
