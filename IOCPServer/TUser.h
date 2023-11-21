#pragma once
#include "TStreamPacket.h"

class TUser
{
public:
    SOCKET      sock;
    SOCKADDR_IN addr;
    TStreamPacket sPacket;
    std::list<UPACKET> list;
    WSABUF      wsaBuffer;
    char        buffer[1024];
    OVERLAPPED2  ov;
    void bind(HANDLE iocp)
    {
        ::CreateIoCompletionPort((HANDLE)sock, iocp, (ULONG_PTR)this, 0);
    }
    void recv()
    {
        DWORD dwTransfer;
        DWORD flag = 0;
        DWORD dwReadByte;
        ov.flag = OVERLAPPED2::MODE_RECV;
        wsaBuffer.buf = buffer;
        wsaBuffer.len = sizeof(buffer);
        WSARecv(sock, &wsaBuffer, 1, &dwTransfer, &flag, &ov, NULL);
    }
    void  Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
    {
        OVERLAPPED2*  pOV = (OVERLAPPED2*)ov;
        if (pOV->flag == OVERLAPPED2::MODE_RECV)
        {

        }
        if (pOV->flag == OVERLAPPED2::MODE_SEND)
        {

        }
        return;
    }
    TUser()
    {

    }
    TUser(SOCKET s, SOCKADDR_IN a) : sock(s), addr(a)
    {

    }
};
