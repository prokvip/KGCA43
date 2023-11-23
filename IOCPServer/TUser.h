#pragma once
#include "TStreamPacket.h"

class TUser
{
public:
    bool        bConneted;
    SOCKET      sock;
    SOCKADDR_IN addr;
    TStreamPacket sPacket;
    std::list<UPACKET> list;
    WSABUF      wsaRecvBuffer;
    WSABUF      wsaSendBuffer;
    char        buffer[4096];
    OVERLAPPED2  ovRecv;
    OVERLAPPED2  ovSend;
    void Close()
    {
        bConneted = false;
        closesocket(sock);
    }
    void bind(HANDLE iocp)
    {
        bConneted = true;
        ::CreateIoCompletionPort((HANDLE)sock, iocp, (ULONG_PTR)this, 0);
    }
    void recv()
    {
        DWORD dwTransfer;
        DWORD flag = 0;
        DWORD dwReadByte;
        ZeroMemory(&ovRecv, sizeof(OVERLAPPED2));
        ovRecv.flag = OVERLAPPED2::MODE_RECV;
        wsaRecvBuffer.buf = buffer;
        wsaRecvBuffer.len = sizeof(buffer);
        WSARecv(sock, &wsaRecvBuffer, 1, &dwTransfer, &flag, &ovRecv, NULL);
    }
    void  Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
    {
        OVERLAPPED2*  pOV = (OVERLAPPED2*)ov;
        if (pOV->flag == OVERLAPPED2::MODE_RECV)
        {
            if (dwTransfer == 0)
            {
                bConneted = false;
                return;
            }
            sPacket.Put(buffer, dwTransfer);
            sPacket.GetPacket(std::ref(*this));
        }
        if (pOV->flag == OVERLAPPED2::MODE_SEND)
        {            
            int k = 0;
        }
        recv();
        return;
    }
    TUser()
    {
        bConneted = false;
    }
    TUser(SOCKET s, SOCKADDR_IN a) : sock(s), addr(a)
    {
        bConneted = false;
    }
};
