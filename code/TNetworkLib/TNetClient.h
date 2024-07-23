#pragma once
#include "TNetwork.h"
class TNetClient :   public TNetwork
{
    std::string Recvbuf;   
    UINT  iBeginPos = 0;
public:
    void   Run() override;
    bool   Recv() override;

    TNetClient()
    {
        m_hSock = socket(AF_INET, SOCK_STREAM, 0);
        Recvbuf.resize(256);
    }
};

