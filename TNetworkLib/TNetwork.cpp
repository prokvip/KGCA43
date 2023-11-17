#include "TNetwork.h"
bool  TNetwork::Init()
{
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    return true;
}
bool  TNetwork::Run()
{
	m_pSelectModel->Run();
	return true;
}
bool TNetwork::Release()
{
    closesocket(m_hSock);
    // )  윈속 해제    
    WSACleanup();
    return true;
}

bool TNetworkServer::Init()
{
    TNetwork::Init();

    // 1) 윈속 초기화    
    m_hSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(m_iPort);

    int iRet = bind(m_hSock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    iRet = listen(m_hSock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

    // u_long on = TRUE;
     //ioctlsocket(m_hSock, FIONBIO, &on);
	return true;
}

bool TNetworkServer::Run()
{
    return TNetwork::Run();
}

