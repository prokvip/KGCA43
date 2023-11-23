#include "TStreamPacket.h"
#include "TUser.h"

bool  TStreamPacket::ParityCheck()
{	
	m_pPacketStart = (UPACKET*)InterlockedExchangePointer(&m_pPacket, nullptr);
	return true;
}

bool   TStreamPacket::GetPacket(TUser& user)
{
	int iPacketCnt = 0;
	if (m_iReadPos < PACKET_HEADER_SIZE)
	{
		return true;
	}
	// 패킷의 시작
	InterlockedExchangePointer(&m_pPacket, (void*)&m_RecvBuffer[m_iStartPos]);	
	
	if (ParityCheck()==false)
	{
		return false;
	}
	// 1개의 패킷 사이즈만큼 받았다면
	if (m_iReadPos >= m_pPacketStart->ph.len)
	{
		do {
			UPACKET    add = { 0, };
			CopyMemory(&add, m_pPacketStart, m_pPacketStart->ph.len);
			user.list.push_back(add);

			InterlockedAdd64(&m_iReadPos, -m_pPacketStart->ph.len);						
			InterlockedAdd64(&m_iStartPos, m_pPacketStart->ph.len);
			InterlockedExchangePointer(&m_pPacket, (void*)&m_RecvBuffer[m_iStartPos]);	
			

			// 잔여량이 패킷 헤더보다 작을 경우
			if (m_iReadPos < PACKET_HEADER_SIZE || m_iReadPos < m_pPacketStart->ph.len)
			{
				break;
			}			
			if (ParityCheck() == false)
			{
				return false;
			}
		} while (m_iReadPos >= m_pPacketStart->ph.len);
	}
	return true;
}
bool   TStreamPacket::Put(char* recvBuffer, int iRecvSize, TUser* pUser)
{		

	// 하지만  보낸 패킷의 크기보다 WSARecv 버퍼가 작으면 쪼개져서 들어온다.
	// 받는 버퍼의 용량이 부족하면		
	if ((m_iWritePos + iRecvSize) >= MAX_RECV_STREAM_SIZE)
	{
		if ( m_iReadPos > 0)
		{
			memmove(m_RecvBuffer, &m_RecvBuffer[m_iStartPos], m_iReadPos);
		}
		//m_iStartPos = 0;
		InterlockedExchange64(&m_iStartPos, 0);
		InterlockedExchange64(&m_iWritePos, m_iReadPos);
		InterlockedExchangePointer(&m_pWritePos, (void*)&m_RecvBuffer[m_iWritePos]);
	}	
	CopyMemory(m_pWritePos, recvBuffer, iRecvSize);
	InterlockedAdd64(&m_iWritePos, iRecvSize);
	InterlockedAdd64(&m_iReadPos, iRecvSize);
	InterlockedExchangePointer(&m_pWritePos, (void*)&m_RecvBuffer[m_iWritePos]);	
	return true;
};

TStreamPacket::TStreamPacket()
{
	Reset();
}

void TStreamPacket::Reset()
{
	//m_RecvBuffer = new char[MAX_RECV_STREAM_SIZE];
	ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_STREAM_SIZE);
	m_iStartPos=0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_pPacket = nullptr;
	m_pPacketStart = nullptr;
	m_pWritePos = nullptr;
	InterlockedExchangePointer(&m_pPacket, (void*)&m_RecvBuffer[m_iStartPos]);
	m_pPacketStart = (UPACKET*)&m_pPacket;
	m_pWritePos = m_pPacket;
}
TStreamPacket::~TStreamPacket()
{
	//delete[] m_RecvBuffer;
}