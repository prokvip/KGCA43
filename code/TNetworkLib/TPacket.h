#pragma once
#include "TNetStd.h"
// 사용방법

class TPacket
{
	char*			m_pOffset=nullptr;
	//PACKET_HEADER	ph;
	//char			msg[MAX_MSG_SIZE];
public:
	UPACKET			m_uPacket;
	void PutData(const char* data, int iByte);
	void GutData(const char* data, int iByte);
	void reset()
	{
		m_pOffset = m_uPacket.msg;
	};
	TPacket& operator << (int data); 
	TPacket& operator << (byte data);
	TPacket& operator << (short data);
	TPacket& operator << (long data);
	TPacket& operator << (float data);
	TPacket& operator << (double data);
	TPacket& operator << (DWORD data);
	TPacket& operator << (UINT data);
	TPacket& operator << (char* data);
	TPacket& operator << (std::string data);
	TPacket& operator << (std::wstring data);

	TPacket& operator >> (int& data);
	TPacket& operator >> (byte& data);
	TPacket& operator >> (short& data);
	TPacket& operator >> (long& data);
	TPacket& operator >> (float& data);
	TPacket& operator >> (double& data);
	TPacket& operator >> (DWORD& data);
	TPacket& operator >> (UINT& data);
	TPacket& operator >> (char* data);
	TPacket& operator >> (std::string& data);
	TPacket& operator >> (std::wstring& data);
public:
	TPacket();
	TPacket(int dwType);
	~TPacket();
};
