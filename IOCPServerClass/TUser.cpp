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
    TOV* tov = new TOV(TOV::MODE_RECV);
    wsaRecvBuffer.buf = buffer;
    wsaRecvBuffer.len = sizeof(buffer);
    int iRet = WSARecv(sock, &wsaRecvBuffer, 1, &dwTransfer, &flag,
                                (LPOVERLAPPED)tov, NULL);

    if (iRet == SOCKET_ERROR)
    {        
        DWORD dwError = WSAGetLastError();

        if (dwError != WSA_IO_PENDING)
        {
            E_MSG("WSARecv");
            delete tov;
        }
    }
}
void  TUser::Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
{
    TOV* pOV = (TOV*)ov;
    if (pOV->flag == TOV::MODE_RECV)
    {
        if (dwTransfer == 0)
        {
            bConneted = false;
            return;
        }
        sPacket.Put(buffer, dwTransfer);
        sPacket.GetPacket(std::ref(*this));
    }
    if (pOV->flag == TOV::MODE_SEND)
    {
        int k = 0;
    }
    delete pOV;

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