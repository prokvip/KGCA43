#pragma once
#include "TSession.h"
#include "TStdSelect.h"
#include "TAsyncSelect.h"
#include "TEventSelect.h"
struct tPacket
{
    UINT     userid;
    UPACKET  packet;
};
class TNetwork
{
public:
    std::shared_ptr<TSelectModel> m_pSelectIO;
    SOCKET              m_hSock;
    bool                m_bRun = true;
    bool                m_bConnect = false;
    HANDLE              m_hKillEvent;
    HANDLE              m_hRunning;
    CRITICAL_SECTION    m_hSessionCS;
    CRITICAL_SECTION    m_hPacketCS;
public:
    std::list<TSession> m_SessionList;
    std::list<UPACKET>  m_PacketPool;

   /* TSession             m_TotalSession[2000];
    std::map<UINT, UINT> m_SessionList2;
    std::list<tPacket>   m_PacketPool2;*/
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
    virtual void AddPacket(PACKET_HEADER ph, byte* data);
    virtual void Run();
    virtual void Release();

    virtual void Connected();
    virtual void DisConnected();
    virtual void DisConnected(TSession& session);    
     

public:
    TNetwork();
    virtual ~TNetwork();
};
