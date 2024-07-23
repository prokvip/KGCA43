#include "TEventSelect.h"
#include "TNetwork.h"
#include "TSession.h"
void     TEventSelect::Accept()
{
	// 이벤트 생성 및 바인딩
	m_EventArray[m_iCurrentEvent] = WSACreateEvent();
	SOCKET clientsock = m_pNet->m_SessionList.back().m_hSock;
	m_pSessionArray[m_iCurrentEvent] = &m_pNet->m_SessionList.back();
	WSAEventSelect(m_pSessionArray[m_iCurrentEvent]->m_hSock, m_EventArray[m_iCurrentEvent],
		FD_READ | FD_WRITE | FD_CLOSE);
	m_iCurrentEvent++;
}
void     TEventSelect::DisConnected(UINT iEvent)
{
	WSACloseEvent(m_EventArray[iEvent]);		
	m_pSessionArray[iEvent]->m_bDisConnected = true;
	m_EventArray[iEvent] = m_EventArray[m_iCurrentEvent - 1];
	m_pSessionArray[iEvent] = m_pSessionArray[m_iCurrentEvent - 1];
	m_iCurrentEvent--;
}
bool     TEventSelect::Run()
{
	// 대기함수
	DWORD dwIndex = WSAWaitForMultipleEvents(m_iCurrentEvent, m_EventArray, 
		FALSE, 1000, FALSE);//WSA_INFINITE
	if (dwIndex == WSA_WAIT_FAILED) return false;
	if (dwIndex == WSA_WAIT_TIMEOUT) return true;
	dwIndex -= WSA_WAIT_EVENT_0;	

	// multi index check
	for (int iEvent = dwIndex; iEvent < m_iCurrentEvent; iEvent++)
	{
		DWORD dwSignal = WSAWaitForMultipleEvents(1, &m_EventArray[iEvent],
			FALSE, 0, FALSE);//WSA_INFINITE
		if (dwSignal == WSA_WAIT_FAILED) continue;
		if (dwSignal == WSA_WAIT_TIMEOUT) continue;

		// 네트워크 이벤트 판단
		WSANETWORKEVENTS   NetworkEvent;
		int iRet = WSAEnumNetworkEvents(
			(iEvent == 0) ? m_pNet->m_hSock : m_pSessionArray[iEvent]->m_hSock,
			m_EventArray[iEvent], &NetworkEvent);
		if (iRet == SOCKET_ERROR)  return false;


		if (NetworkEvent.lNetworkEvents & FD_ACCEPT)
		{
			if (NetworkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				return false;
			}
			if (m_pNet->Accept() == false)
			{
				return false;
			}
			Accept();
		}
		if (NetworkEvent.lNetworkEvents & FD_READ)
		{
			if (NetworkEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				return false;
			}
			if (m_pSessionArray[iEvent]->Recv() == true)
			{
				m_pNet->AddPacket(m_pSessionArray[iEvent]->m_packet);
			}
		}
		if (NetworkEvent.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				return false;
			}

		}
		if (NetworkEvent.lNetworkEvents & FD_CLOSE)
		{
			/*if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				return false;
			}*/
			DisConnected(iEvent);
		}
	}

	if (m_pNet->PacketProcess() == false)
	{
		return false;
	}
	m_pNet->CheckConnected();
	return true;
}
TEventSelect::TEventSelect(TNetwork* net) : TSelectModel(net->m_hSock)
{
	m_pNet = net;
	m_iCurrentEvent = 0;
	m_EventArray[m_iCurrentEvent] = WSACreateEvent();
	m_pSessionArray[m_iCurrentEvent] = nullptr;
	WSAEventSelect(net->m_hSock, m_EventArray[m_iCurrentEvent], 
		FD_ACCEPT | FD_CLOSE);
	m_iCurrentEvent++;
}
TEventSelect::~TEventSelect()
{

}