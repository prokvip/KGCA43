#include "TServer.h"

void TServer::ChatMsg(T_Packet& t)
{
	m_PacketBroadcasttingPool.Add(t);
}
void TServer::tPACKET_MOVE_CHARACTER(T_Packet& t)
{
    // 서버 작업한 후
    T_Packet SendPacket;
    SendPacket.m_pUser = t.m_pUser;
    m_PacketBroadcasttingPool.Add(SendPacket);
}
void TServer::tPACKET_CREATE_CHARACTER(T_Packet& t)
{
    // 해당 패킷의 처리의 결과를 해당 유저에게 전송.
    // MAKE Packet 
    UPACKET SendPacket;
    t.m_pUser->list.push_back(SendPacket);
}