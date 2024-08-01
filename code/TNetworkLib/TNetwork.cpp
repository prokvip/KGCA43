#include "TNetwork.h"
void TNetwork::Connected()
{
    m_bConnect = true;
    if (m_bConnect)
    {
        std::cout << "서버 접속 성공 : " << std::endl;
    }
    else
    {
        DisConnected();
        std::cout << "서버 종료 : " << std::endl;
    }
}
void TNetwork::DisConnected(TSession& session)
{
    session.Disconnect();
}
void TNetwork::DisConnected()
{
    closesocket(m_hSock);
    m_bConnect = false;   
}

void    TNetwork::AddPacket(UPACKET& packet)
{
    //임계영역
    EnterCriticalSection(&m_hPacketCS);
        m_PacketPool.emplace_back(packet);
    LeaveCriticalSection(&m_hPacketCS);
}
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

        std::cout << "ERROR(" << iError << "):" << (char*)lpMsgBuffer << std::endl;
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
        SendByte = send(m_hSock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (CheckError())
            {
                closesocket(m_hSock);
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
        SendByte = send(user.m_hSock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (CheckError())
            {
                std::cout << "비정상 서버 종료!" << std::endl;
                user.m_bDisConnected = false;
            }
        }
    }
    return SendByte;
}
void TNetwork::Release()
{
    for (auto iter = m_SessionList.begin(); iter != m_SessionList.end(); iter++)
    {
        DisConnected(*iter);
    }
    m_SessionList.clear();
}
void TNetwork::PacketProcess(UPACKET& packet)
{
    if (packet.ph.type == PACKET_CHAT_MSG)
    {
        packet.msg[packet.ph.len-PACKET_HEADER_SIZE] = 0;
        std::cout << packet.msg << std::endl;
        Broadcast(packet);
    }    
}

void TNetwork::Broadcast(UPACKET& packet)
{
    // 브로드케스팅
    for (auto iterSend = m_SessionList.begin(); iterSend != m_SessionList.end(); iterSend++)
    {
        TSession& user = (*iterSend);
        user.SendPacket(packet);
    }
}
void TNetwork::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
    //임계영역
    EnterCriticalSection(&m_hSessionCS);
    
        std::cout << "접속 ip=" << inet_ntoa(addr.sin_addr) << "port=" << ntohs(addr.sin_port) << std::endl;
        m_SessionList.emplace_back(sock, addr);
    
    LeaveCriticalSection(&m_hSessionCS);
}
bool TNetwork::Accept()
{
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    int iMsgCounter = 0;
    SOCKET clientSock = accept(m_hSock, (sockaddr*)&addr, &addrlen);
    if (clientSock == INVALID_SOCKET)
    {
        if (TNetwork::CheckError())
        {
            return false;
        }
    }
    else
    {
        AddSession(clientSock, addr);
    }
    return true;
}
bool TNetwork::Recv()
{
    for (auto iter = m_SessionList.begin(); iter != m_SessionList.end(); iter++)
    {
        TSession& session = (*iter);
        if (session.Recv() == false)
        {
            continue;
        }
        AddPacket(session.m_packet);
    }
    return true;
}
bool TNetwork::PacketProcess()
{
    for (auto iter = m_PacketPool.begin(); iter != m_PacketPool.end(); iter++)
    {
        PacketProcess(*iter);
    }
    m_PacketPool.clear();
    return true;
}
void TNetwork::Run()
{
    while (m_bRun)
    {
        if (Accept() == false)
        {
            break;
        }
        if (Recv() == false)
        {
            break;
        }
        if (PacketProcess() == false)
        {
            break;
        }
        CheckConnected();
        // 게임로직
    }
}
void TNetwork::CheckConnected()
{
    // 종료
    //임계영역
    EnterCriticalSection(&m_hSessionCS);
    for (auto iter = m_SessionList.begin(); iter != m_SessionList.end(); )
    {
        if ((*iter).m_bDisConnected)
        {
            DisConnected(*iter);
            iter = m_SessionList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    LeaveCriticalSection(&m_hSessionCS);
}
void TNetwork::CreateServer(std::string ip, USHORT port)
{
    m_hSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, ip.c_str(), &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = bind(m_hSock, (sockaddr*)&sa, namelen);
    if (ret < 0)
    {
        CheckError();
    }
    ret = listen(m_hSock, SOMAXCONN);
    if (ret < 0)
    {
        CheckError();
    }
    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(m_hSock, FIONBIO, &iNonSocket);    

    // 객체생성
    InitializeCriticalSection(&m_hSessionCS);

}
bool TNetwork::Connected(std::string ip, USHORT port)
{    
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    //inet_pton(AF_INET, ip.c_str(), &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = connect(m_hSock, (sockaddr*)&sa, namelen);
    if (ret != 0)
    {
        if (CheckError() == true)
        {
            return false;
        }
    }

    // 넌블록형 소켓으로 전환
    u_long iNonSocket = TRUE;
    int iMode = ioctlsocket(m_hSock, FIONBIO, &iNonSocket);
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

TNetwork::TNetwork()
{
    InitWinSock();
    //TObjectPool<tOV>::prepareAllocation();
}
TNetwork::~TNetwork()
{
    // 객체생성
    DeleteCriticalSection(&m_hSessionCS);

    closesocket(m_hSock);
    DelWinSock();
    TObjectPool<tOV>::allFree();
}