#pragma once
#include "TNetwork.h"
class TNetClient :   public TNetwork
{

public:
    bool   Connected(std::string ip, USHORT port);
    void   Run() override;
    TNetClient()
    {
        m_hSock = socket(AF_INET, SOCK_STREAM, 0);
    }
};

