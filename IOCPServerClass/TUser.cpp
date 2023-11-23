#include "TUser.h"

void TUser::Close()
{
    bConneted = false;
    closesocket(sock);
}
void TUser::bind(HANDLE iocp)
{
    bConneted = true;
    ::CreateIoCompletionPort((HANDLE)sock, iocp, (ULONG_PTR)this, 0);
}
void TUser::recv()
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
void  TUser::Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
{
    OVERLAPPED2* pOV = (OVERLAPPED2*)ov;
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
TUser::TUser()
{
    bConneted = false;
}
TUser::TUser(SOCKET s, SOCKADDR_IN a) : sock(s), addr(a)
{
    bConneted = false;
}