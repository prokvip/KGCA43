#pragma once
#include "TNetStd.h"
#include "TUser.h"

struct T_Packet
{
	UPACKET packet;
	TUser*  m_pUser;
};
// std::mutex -> std::lock
// std::shared_mutex(read, write)
// read, write ->locking

class TPacketPool
{	
public:
	std::shared_mutex m_sMutex;
	std::list<T_Packet*>  list;
	void    Add(T_Packet& tPacket);
	void    Process(T_Packet& tPacket);
};