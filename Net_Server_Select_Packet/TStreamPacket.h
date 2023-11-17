#pragma once
#include "TProtocol.h"
#define MAX_RECV_STREAM_SIZE 2048

class TUser;

class TStreamPacket 
{
public:	
	UPACKET*    m_pPacketStart;
	PVOID       m_pWritePos;
	PVOID 		m_pPacket;
	char		m_RecvBuffer[MAX_RECV_STREAM_SIZE];
	// 64비트데이터,   volatile(컴파일러 최적화 방지)
	// const int a = 0;
	// if( 0 ) {} // 최적화
	long long volatile	m_iStartPos;
	long long volatile	m_iWritePos;
	long long volatile	m_iReadPos;
public:

	bool    Put(char* redvBuffer, int iSize, TUser* = NULL);
	bool    GetPacket(TUser* pUser);
	bool	ParityCheck(TUser* pUser);
	void    Reset();
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
