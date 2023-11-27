#include "TPacketPool.h"

void    TPacketPool::Add(T_Packet& tPacket)
{
	// write 전용
	{
		std::lock_guard<std::shared_mutex> lock(m_sMutex);
		// 임계구역
		list.push_back(tPacket);
	}
	//m_sMutex.lock();
	//	// 임계구역
	//	list.push_back(tPacket);
	//m_sMutex.unlock();
}

void    TPacketPool::Process(T_Packet& tPacket)
{
	// read 전용
	{
		std::shared_lock<std::shared_mutex> lock(m_sMutex);
		// 공유
		for (auto& packet : list)
		{
		}
	}
	//m_sMutex.lock_shared();
	//	// 공유
	//	for (auto& packet : list)
	//	{

	//	}
	//m_sMutex.unlock_shared();
}