#include "TNetwork.h"
bool    TNetwork::CheckError()
{
    int iError = WSAGetLastError();
    if (iError != WSAEWOULDBLOCK)
    {
        LPVOID lpMsgBuffer;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM, NULL,
            iError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuffer,
            0, NULL);
        std::cout << "ERROR(" << iError << "):" << lpMsgBuffer << std::endl;
        LocalFree(lpMsgBuffer);
        //exit(-1);
        return true;
    }
    return false;
}
UPACKET TNetwork::MakePacket(
    short  		sDataSize,
    short    	type,
    const char* msg)
{
    UPACKET packet;
    ZeroMemory(&packet, sizeof(packet));
    packet.ph.len = PACKET_HEADER_SIZE + sDataSize;
    packet.ph.type = type;
    memcpy(packet.msg, msg, sDataSize);
    return std::move(packet);
}
int  TNetwork::SendPacket(SOCKET sock, UPACKET& packet)
{
    int SendByte = 0;
    if (m_bRun)
    {
        SendByte = send(sock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (CheckError())
            {
                closesocket(sock);
                std::cout << "비정상 서버 종료!" << std::endl;
            }
        }

    }
    return SendByte;
}
int  TNetwork::SendPacket(UPACKET& packet)
{
    int SendByte = 0;
    if (m_bRun)
    {
        SendByte = send(g_hSock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (CheckError())
            {
                closesocket(g_hSock);
                std::cout << "비정상 서버 종료!" << std::endl;
            }
        }
    }
    return SendByte;
}
int  TNetwork::SendPacket(TSession& user, UPACKET& packet)
{
    int SendByte = 0;
    if (m_bRun)
    {
        SendByte = send(user.sock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (CheckError())
            {
                closesocket(user.sock);
                std::cout << "비정상 서버 종료!" << std::endl;
                user.bDisConnected = false;
            }
        }
    }
    return SendByte;
}
void TNetwork::Release()
{
    for (auto iter = g_sockList.begin(); iter != g_sockList.end(); iter++)
    {
        DisConnected(*iter);
    }
    g_sockList.clear();
}
void TNetwork::PacketProcess(UPACKET& packet)
{
    if (packet.ph.type == PACKET_CHAT_MSG)
    {
        std::cout << packet.msg << std::endl;
        Broadcast(packet);
    }
}

void TNetwork::Broadcast(UPACKET& packet)
{
    // 브로드케스팅
    for (auto iterSend = g_sockList.begin(); iterSend != g_sockList.end(); iterSend++)
    {
        TSession& user = (*iterSend);
        user.SendPacket(packet);
    }
}
void TNetwork::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
    std::cout << "접속 ip=" << inet_ntoa(addr.sin_addr) << "port=" << ntohs(addr.sin_port) << std::endl;
    g_sockList.emplace_back(sock, addr);
}
void TNetwork::Run()
{
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    int iMsgCounter = 0;
    SOCKET clientSock;

    while (m_bRun)
    {
        clientSock = accept(g_hSock, (sockaddr*)&addr, &addrlen);
        if (clientSock == INVALID_SOCKET)
        {
            if (CheckError())
            {
                break;
            }
        }
        else
        {
            AddSession(clientSock, addr);
        }

        for (auto iter = g_sockList.begin(); iter != g_sockList.end(); iter++)
        {
            TSession& session = (*iter);
            if (session.Recv() == false)
            {
                continue;
            }
            // 정상적인 데이터 수신            
            PacketProcess(session.packet);
        }
        CheckConnected();
    }
}
void TNetwork::CheckConnected()
{
    // 종료
    for (auto iter = g_sockList.begin(); iter != g_sockList.end(); )
    {
        if ((*iter).bDisConnected)
        {
            DisConnected(*iter);
            iter = g_sockList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}
void TNetwork::Create(std::string ip, USHORT port)
{
    g_hSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, ip.c_str(), &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = bind(g_hSock, (sockaddr*)&sa, namelen);
    ret = listen(g_hSock, SOMAXCONN);
    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(g_hSock, FIONBIO, &iNonSocket);
}
bool TNetwork::Connected(std::string ip, USHORT port)
{
    g_hSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    //inet_pton(AF_INET, ip.c_str(), &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = connect(g_hSock, (sockaddr*)&sa, namelen);
    if (ret != 0) return false;

    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(g_hSock, FIONBIO, &iNonSocket);
    return true;
}
bool TNetwork::InitWinSock()
{
    WSADATA WSAData;
    int ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if (ret != 0) return false;
    return true;
}
void TNetwork::DelWinSock()
{
    WSACleanup();
}
void TNetwork::DisConnected(TSession& session)
{
    closesocket(session.sock);
    std::cout << "접속종료 : " <<
        inet_ntoa(session.addr.sin_addr) << std::endl;
}
TNetwork::TNetwork()
{
    InitWinSock();
}
TNetwork::~TNetwork()
{
    closesocket(g_hSock);
    DelWinSock();
}