#pragma once
#include "TNetStd.h"
class TNetwork;
class TSession
{
public:
    SOCKET      sock;
    SOCKADDR_IN addr;
    std::string buf;
    UINT        iBeginPos = 0;
    //UINT        RecvByte;
    bool        bDisConnected;
    UPACKET     packet;
    bool        Recv();
    void        SendPacket(UPACKET& packet);
    TSession(SOCKET sock, SOCKADDR_IN addr);
};

