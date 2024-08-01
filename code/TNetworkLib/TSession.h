#pragma once
#include "TNetStd.h"
#define MAX_NUM_RECV_BUFFER 1024
#define MAX_NUM_PACKET_BUFFER 10000


class TNetwork;
class TSession
{
public:
    SOCKET      m_hSock;
    SOCKADDR_IN m_addr;
    std::string m_buf;
    UINT        m_iBeginPos = 0;
    //UINT        m_RecvByte;
    bool        m_bDisConnected;
    UPACKET     m_packet;
public:
    //tOV           m_ovRecv;
    //tOV           m_ovSend;
    char          m_szDataBuffer[MAX_NUM_RECV_BUFFER] = { 0, };
    WSABUF        m_wsaDataBuffer;
public:

    // m_szPacketBuffer[0,20]->m_szPacketBuffer[20, 30]->m_szPacketBuffer[50, ?]
    byte          m_szPacketBuffer[MAX_NUM_PACKET_BUFFER];
    DWORD         m_dwWritePos = 0;
    DWORD         m_dwReadPos = 0;
    DWORD         m_dwStartPacketPos = 0;
    DWORD         m_dwEndPacketPos = 0;
    std::vector<std::string>   m_dataList;
public:
    bool        Recv();
    int         SendPacket(UPACKET& packet);
    void        Disconnect();
public:
    bool        AsyncRecv();
    bool        Dispatch(DWORD dwTransfer, tOV* ov);
    bool        Put(DWORD dwSize);
public:
    TNetwork*  m_pNet = nullptr;
    TSession(SOCKET sock, SOCKADDR_IN addr, TNetwork* pNet=nullptr);
};

