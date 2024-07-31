#pragma once
#include "TNetwork.h"
#define MAX_NUM_WORKDER_THREAD 2


class TNetIOCPServer :  public TNetwork
{
public:
	HANDLE			g_hIOCP;
	HANDLE			g_hWorkerThread[MAX_NUM_WORKDER_THREAD];
public:
	void	CreateServer(std::string ip, USHORT port) override;
	void	Run() override;
	void	AddSession(SOCKET sock, SOCKADDR_IN addr) override;
	void	Release() override;
};

