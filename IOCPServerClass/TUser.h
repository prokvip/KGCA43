#pragma once
#include "TStreamPacket.h"
#include "TObjectPool.h"

struct TOV : public TObjectPool<TOV>
{
    enum { MODE_RECV = 0, MODE_SEND = 1, };

    OVERLAPPED ov;
    int flag;

    TOV(int value) : flag(value)
    {
        ZeroMemory(&ov, sizeof(OVERLAPPED));
    }
    TOV()
    {
        ZeroMemory(&ov, sizeof(OVERLAPPED));
        flag = TOV::MODE_RECV;
    }
};
class TUser
{
public:
    bool                bConneted;
    SOCKET              sock;
    SOCKADDR_IN         addr;
    TStreamPacket       sPacket;
    std::list<UPACKET>  list;   // recv->send
    WSABUF              wsaRecvBuffer;
    WSABUF              wsaSendBuffer;
    char                buffer[4096];
public:
    virtual void    Close();
    virtual void    bind(HANDLE iocp);
    virtual void    recv();
    virtual void    Dispatch(DWORD dwTransfer, OVERLAPPED* ov);
public:
    TUser();
    TUser(SOCKET s, SOCKADDR_IN a);
};
