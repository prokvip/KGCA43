#pragma once
#include "TSession.h"
#include "TStdSelect.h"
#include "TAsyncSelect.h"
class TNetwork
{
public:
    std::shared_ptr<TSelectModel> m_pSelectIO;
    SOCKET              m_hSock;
    bool                m_bRun = true;
    bool                m_bConnect = false;
public:
    std::list<TSession> m_SessionList;
    std::list<UPACKET>  m_PacketPool;
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
    
    virtual void CreateServer(std::string ip, USHORT port = 10000);
    virtual bool Connected(std::string ip, USHORT port = 10000);
    virtual bool Accept();
    virtual bool Recv();
    virtual bool PacketProcess();
    virtual void AddPacket(UPACKET& packet);
    virtual void Run();
    virtual void Release();

    virtual void Connected();
    virtual void DisConnected();
    virtual void DisConnected(TSession& session);    
     
public:
    TNetwork();
    virtual ~TNetwork();
};
