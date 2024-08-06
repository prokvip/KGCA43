#include "TPacket.h"
TPacket::TPacket()
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_pOffset = m_uPacket.msg;
}
TPacket::TPacket(int dwType)
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = dwType;
	m_pOffset = m_uPacket.msg;
}
TPacket::~TPacket()
{
	
}
void TPacket::PutData(const char* data, int iByte)
{
	m_uPacket.ph.len += iByte;
	memcpy(m_pOffset, data, iByte);
	m_pOffset += iByte;
}
void TPacket::GutData(const char* pData, int iByte)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iByte);
	m_pOffset += iByte;
}
TPacket& TPacket::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
TPacket& TPacket::operator << (byte data)
{
	PutData((char*)&data, sizeof(byte));
	return *this;
}
TPacket& TPacket::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
TPacket& TPacket::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
TPacket& TPacket::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
TPacket& TPacket::operator << (double data)
{
	PutData((char*)&data, sizeof(double));
	return *this;
}
TPacket& TPacket::operator << (DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}
TPacket& TPacket::operator << (UINT data)
{
	PutData((char*)&data, sizeof(UINT));
	return *this;
}
TPacket& TPacket::operator << (char* data)
{
	int iSize = strlen(data) + 1;
	PutData(data, iSize);
	return *this;
}
TPacket& TPacket::operator << (std::string data)
{	
	PutData(data.c_str(), data.size()+1);
	return *this;
}
TPacket& TPacket::operator << (std::wstring data)
{
	std::string msg = wm(data);
	PutData(msg.c_str(), msg.size() + 1);
	return *this;
}

TPacket& TPacket::operator >> (int& data)
{
	GutData((char*)&data, sizeof(int));
	return *this;
}
TPacket& TPacket::operator >> (byte& data)
{
	GutData((char*)&data, sizeof(byte));
	return *this;
}
TPacket& TPacket::operator >> (short& data)
{
	GutData((char*)&data, sizeof(short));
	return *this;
}
TPacket& TPacket::operator >> (long& data)
{
	GutData((char*)&data, sizeof(long));
	return *this;
}
TPacket& TPacket::operator >> (float& data)
{
	GutData((char*)&data, sizeof(float));
	return *this;
}
TPacket& TPacket::operator >> (double& data)
{
	return *this;
}
TPacket& TPacket::operator >> (DWORD& data)
{
	GutData((char*)&data, sizeof(DWORD));
	return *this;
}
TPacket& TPacket::operator >> (UINT& data)
{
	GutData((char*)&data, sizeof(UINT));
	return *this;
}
TPacket& TPacket::operator >> (char* data)
{
	int iSize = strlen(m_pOffset) + 1;
	GutData(const_cast<char*>(data), iSize);
	return *this;
}
TPacket& TPacket::operator >> (std::string& data)
{
	int iSize = strlen(m_pOffset) + 1;
	data.resize(iSize);
	GutData(const_cast<char*>(data.c_str()), iSize);
	return *this;
}
TPacket& TPacket::operator >> (std::wstring& data)
{
	std::string ret;
	int iSize = strlen(m_pOffset) + 1;
	ret.resize(iSize);
	GutData(const_cast<char*>(ret.c_str()), iSize);
	data = mw(ret);
	return *this;
}