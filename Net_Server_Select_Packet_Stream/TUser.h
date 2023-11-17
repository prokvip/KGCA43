#pragma once
#include "TStreamPacket.h"

class TUser
{
public:
    SOCKET      sock;
    SOCKADDR_IN addr;
    TStreamPacket sPacket;
    std::list<UPACKET> list;
    TUser(SOCKET s, SOCKADDR_IN a) : sock(s), addr(a)
    {

    }
};
