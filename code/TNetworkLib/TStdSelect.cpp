#include "TStdSelect.h"
#include "TNetwork.h"

bool TStdSelect::Run()
{
	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_ZERO(&m_eSet);
	FD_SET(m_hSock, &m_rSet);
	FD_SET(m_hSock, &m_wSet);
	FD_SET(m_hSock, &m_eSet);

	for (auto& user : m_pNet->m_SessionList)
	{
		FD_SET(user.m_hSock, &m_rSet);
		FD_SET(user.m_hSock, &m_wSet);
		FD_SET(user.m_hSock, &m_eSet);
	}

	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 100;
	int iRet = select(0, &m_rSet, &m_wSet, &m_eSet, &time);
	if (iRet == SOCKET_ERROR)
	{
		TNetwork::CheckError();
		return false;
	}
	// timeout
	if (iRet == 0)
	{
		return true;
	}
	if (FD_ISSET(m_hSock, &m_rSet))
	{
		if (m_pNet->Accept() == false)
		{
			return false;
		}
	}
	if (FD_ISSET(m_hSock, &m_wSet))
	{
		return true;
	}
	for (auto& session : m_pNet->m_SessionList)
	{
		if (FD_ISSET(session.m_hSock, &m_eSet))
		{
			session.m_bDisConnected = true;
			continue;
		}

		if (FD_ISSET(session.m_hSock, &m_rSet))
		{
			if (session.Recv() == true)
			{
				m_pNet->AddPacket(session.m_packet);
			}			
		}
		if (FD_ISSET(session.m_hSock, &m_wSet)) // 이제부터 보낼수 있다.
		{			
		}		
	}
	if (m_pNet->PacketProcess() == false)
	{
		return false;
	}
	m_pNet->CheckConnected();
	return true;
}
TStdSelect::TStdSelect(TNetwork* net) : TSelectModel(net->m_hSock)
{
	m_pNet = net;
}
TStdSelect::~TStdSelect()
{

}