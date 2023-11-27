#include "TServer.h"
#include "TLog.h"
void TAccept::Close()
{
    closesocket(listensock);
}
bool TAccept::ThreadRun()
{
    if (m_pServer == nullptr) return false;

    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
    if (clientsock == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            LogErrorA("accept");
            return false;
        }
    }
    else
    {
        TUser* user = new TUser(clientsock, clientaddr);
        user->bind(m_pServer->GetIocpHandle());
        user->recv();
        m_pServer->GetSessionMgr().Add(user);

        LogA("클라이언트 접속 ip=%s, Port:%d\n",
            inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port));            
    }
    return true;
}
bool	TServer::Init()
{   
    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0)
    {
        TLog::Get().log("WSAStartup Failed!");
        return false;
    }
    LogDetailA("WSAStartup");
    m_Accepter.listensock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(10000);

    iRet = ::bind(m_Accepter.listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return false;
    LogDetailA("bind");
    iRet = ::listen(m_Accepter.listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return  false;
    LogDetailA("listen");
    
    m_Accepter.Set(this);
    m_Iocp.Init();
    LogDetailA("m_Iocp.Init()");

    TThread::CreateThread();


    m_fnExecutePacket[PACKET_CHAT_MSG] = std::bind(&TServer::ChatMsg, this, 
                                                    std::placeholders::_1);

    /*m_fnExecutePacket[PACKET_VERIFICATION_REQ] = &TServer::ReqVerification;
    m_fnExecutePacket[PACKET_CHAT_NAME_REQ] = &TServer::ReqChatName;
    m_fnExecutePacket[PACKET_CHAT_NAME_ACK] = &TServer::AckChatName;
    m_fnExecutePacket[PACKET_CHAT_MSG] = &TServer::Msg;
    m_fnExecutePacket[PACKET_USER_POSITION] = &TServer::MoveAction;
    m_fnExecutePacket[PACKET_STOP_POSITION] = &TServer::MoveAction;

    m_fnExecutePacket[PACKET_LOGIN_REQ] = &TServer::Login;
    m_fnExecutePacket[PACKET_LOGOUT_REQ] = &TServer::Logout;
    m_fnExecutePacket[PACKET_CREATE_ACCOUNT_REQ] = &TServer::CreateAccount;
    m_fnExecutePacket[PACKET_DELETE_ACCOUNT_REQ] = &TServer::DeleteAccount;
    m_fnExecutePacket[PACKET_CREATE_CHARACTER_REQ] = &TServer::CreateCharacter;
    m_fnExecutePacket[PACKET_DELETE_CHARACTER_REQ] = &TServer::DeleteCharacter;
    m_fnExecutePacket[PACKET_ZONE_ENTRY_REQ] = &TServer::ReqZoneEntry;
    m_fnExecutePacket[PACKET_ZONE_TARGET_ACK] = &TServer::RecvZoneTargetPoint;

    m_fnExecutePacket[PACKET_HIT_CHARACTER] = &TServer::HitCharacter;
    m_fnExecutePacket[PACKET_HIT_NPC] = &TServer::HitMonster;
    m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = &TServer::AttackCharacter;
    m_fnExecutePacket[PACKET_ATTACK_NPC] = &TServer::AttackMonster;
    m_fnExecutePacket[PACKET_DEAD_CHARACTER] = &TServer::DeadCharacters;
    m_fnExecutePacket[PACKET_DEAD_NPC] = &TServer::DeadMonster;
    m_fnExecutePacket[PACKET_DAMAGE_CHARACTER] = &TServer::DamageCharacter;
    m_fnExecutePacket[PACKET_DAMAGE_NPC] = &TServer::DamageMonster;
    m_fnExecutePacket[PACKET_SPAWN_CHARACTER] = &TServer::SpawnCharacters;
    m_fnExecutePacket[PACKET_SPAWN_NPC] = &TServer::SpawnMonster;
    m_fnExecutePacket[PACKET_SYNC_CHARACTER] = &TServer::SyncCharacters;
    m_fnExecutePacket[PACKET_SYNC_NPC_LIST] = &TServer::SyncMonster;*/

    return true;
}

bool	TServer::Release()
{    
    /*WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkerThread, TRUE, INFINITE);
    CloseHandle(g_hKillEvent);
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }*/
    return true;
}
int TServer::SendPacket(TUser * pUser, UPACKET & packet)
{
    char* sendbuffer = (char*)&packet;

    pUser->wsaSendBuffer.buf = (char*)&packet;
    pUser->wsaSendBuffer.len = packet.ph.len;

    TOV* tov = new TOV(TOV::MODE_SEND);   

    int iSendByte = 0;
    int iTotalSendByte = 0;
    DWORD dwSendByte;
    int iRet = WSASend(pUser->sock, &pUser->wsaSendBuffer, 1,
        &dwSendByte, 0, (LPOVERLAPPED)tov, NULL);
    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            return -1;
        }
    }
    return packet.ph.len;
}

bool TServer::Broadcastting(UPACKET& packet)
{
    for (std::list<TUser*>::iterator iterSend = m_SessionMgr.g_userlist.begin();
        iterSend != m_SessionMgr.g_userlist.end();
        iterSend++)
    {
        TUser* pUser = (*iterSend);
        if (pUser->bConneted == false) continue;
        int iSendByte = SendPacket(pUser, packet);
        if (iSendByte == SOCKET_ERROR)
        {
            pUser->bConneted = false;
            continue;
        }
    }
    return true;
}

bool TServer::ThreadRun()
{    
    // 브로드케스팅
    for (auto& data : m_PacketBroadcasttingPool.list)
    {
        if (!Broadcastting(data.packet))
        {            
        }
    }
    m_PacketBroadcasttingPool.list.clear();

    for (std::list<TUser*>::iterator iterSend = m_SessionMgr.g_userlist.begin();
        iterSend != m_SessionMgr.g_userlist.end();
        iterSend++)
    {
        TUser* pUser = (*iterSend);    

        if (pUser->bConneted == false) continue;        
        for (auto& data : pUser->list)
        {
            if (!SendPacket(pUser, data))
            {
                pUser->bConneted = false;
            }           
        }
        pUser->list.clear();
    }

    //post
    m_SessionMgr.DeleteUser();
    return true;
}


void	TZoneServer::AddPacket(T_Packet& tPacket)
{    
    FunctionIterator iter = m_fnExecutePacket.find(tPacket.packet.ph.type);
    if (iter != m_fnExecutePacket.end())
    {
        CallFunction call = iter->second;
        call(tPacket);
    }
}