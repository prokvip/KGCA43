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
    bool        bConneted;
    SOCKET      sock;
    SOCKADDR_IN addr;
    TStreamPacket sPacket;
    std::list<UPACKET> list;
    WSABUF      wsaRecvBuffer;
    WSABUF      wsaSendBuffer;
    char        buffer[4096];

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
        
        //ovRecv.flag = TOV::MODE_RECV;
        TOV* tov = new TOV(TOV::MODE_RECV);
        wsaRecvBuffer.buf = buffer;
        wsaRecvBuffer.len = sizeof(buffer);
       int iRet =  WSARecv(sock, &wsaRecvBuffer, 1, 
                    &dwTransfer, &flag, 
                    (LPOVERLAPPED)tov, NULL);
       
       DWORD dwError;
       if (iRet == SOCKET_ERROR)
       {
           //E_MSG("WSARecv");
           dwError = WSAGetLastError();

           if (dwError != WSA_IO_PENDING)
           {
               //delete tov;
               return;
           }
       }
    }
    void  Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
    {
        TOV*  pOV = (TOV*)ov;
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
    TUser()
    {
        bConneted = false;
    }
    TUser(SOCKET s, SOCKADDR_IN a) : sock(s), addr(a)
    {
        bConneted = false;
    }
};
