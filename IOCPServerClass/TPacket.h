#pragma once
#include "TNetStd.h"

class TPacket
{
public:
	UPACKET   m_uPacket;
	char*     m_pOffset = nullptr;
public:
	void Put(const char* pData, int iSize);
	void Get(const char* pData, int iSize);

	TPacket& operator << (int data);
	//TPacket& operator << (BYTE data);
	TPacket& operator << (short data);
	TPacket& operator << (long data);
	TPacket& operator << (DWORD data);
	TPacket& operator << (WORD data);
	TPacket& operator << (const char* data);
	TPacket& operator << (std::string data);

	TPacket& operator >> (int& data);
	//TPacket& operator >> (BYTE& data);
	TPacket& operator >> (short& data);
	TPacket& operator >> (long& data);
	TPacket& operator >> (DWORD& data);
	TPacket& operator >> (WORD& data);
	TPacket& operator >> (const char* data);
	TPacket& operator >> (std::string& data);

	TPacket();
};

// TPacket senddata;
// senddata << "ȫ�浿" << id << msg;

// TPacket recvdata;
// CHAT_MSG chatmsg;
// recvdata >> chatmsg.name << chatmsg.id << chatmsg.msg;
