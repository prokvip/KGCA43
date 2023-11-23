#pragma once
#include "TStreamPacket.h"

class TUser
{
public:
    bool                bConneted;
    SOCKET              sock;
    SOCKADDR_IN         addr;
    TStreamPacket       sPacket;
    std::list<UPACKET>  list;
    WSABUF              wsaRecvBuffer;
    WSABUF              wsaSendBuffer;
    char                buffer[4096];
    OVERLAPPED2         ovRecv;
    OVERLAPPED2         ovSend;
public:
    void    Close();
    void    bind(HANDLE iocp);
    void    recv();
    void    Dispatch(DWORD dwTransfer, OVERLAPPED* ov);
public:
    TUser();
    TUser(SOCKET s, SOCKADDR_IN a);
};
