#include <iostream>
#include "TProtocol.h"
#include <list>
#include "TPacket.h"
// 패킷 생성 방법( 네트워크 데이터는 1바이트 정렬 )
// 1) 구조체
// 2) 패킷 생성 클래스 
// 3) 클래스 직렬화, 역직렬화
// 4) std::stringstream( sstream ) 

std::list<UPACKET> sendPacketList;
std::list<UPACKET> recvPacketList;

//int SendPacket(int sock, const char* msg, int iSize,  WORD type)
//{
//	UPACKET sendpacket;
//	sendpacket.ph.len = PACKET_HEADER_SIZE + iSize;
//	sendpacket.ph.type = type;
//	memcpy(sendpacket.msg, msg, iSize);
//	char* sendbuffer = (char*)&sendpacket;
//
//	int iSendByte = 0;
//	int iTotalSendByte = 0;
//	do {
//		iSendByte = send(sock, &sendbuffer[iTotalSendByte],
//			                   sendpacket.ph.len- iTotalSendByte, 0);
//		iTotalSendByte += iSendByte;
//	} while (iTotalSendByte< sendpacket.ph.len);
//	return sendpacket.ph.len;
//}

int main()
{    
	//CHAT_MSG cMsg[100];
	std::string msg = "안녕하세여";
	std::string name = "홍길동";
	int         ID = 999;

	TPacket senddata;
	//senddata << cMsg;
	senddata << name << ID << "안녕하세여";

	TPacket recvdata= senddata;
	recvdata.m_pOffset = recvdata.m_uPacket.msg;
	CHAT_MSG chatmsg1;
	recvdata >> chatmsg1.szName >> chatmsg1.iID >> chatmsg1.szMsg;




	CHAT_MSG chatmsg;
	ZeroMemory(&chatmsg, sizeof(chatmsg));
	memcpy(chatmsg.szName, name.c_str(), name.size());
	chatmsg.iID = ID;
	memcpy(chatmsg.szMsg, msg.c_str(), msg.size());

	
	UPACKET sendpacket;
	sendpacket.ph.len = PACKET_HEADER_SIZE + sizeof(chatmsg);
	sendpacket.ph.type = PACKET_CHAT_MSG;	
	memcpy(sendpacket.msg, &chatmsg, sizeof(chatmsg));		
	char * sendbuffer = (char*)&sendpacket;
	sendPacketList.push_back(sendpacket);
	for (auto packet : sendPacketList)
	{
		// send(sendbuffer, sendpacket.ph.len);
	}

	//int sock = { 0, };
	//SendPacket(sock, (char*)&chatmsg, sizeof(chatmsg), PACKET_CHAT_MSG);



	// recv
	UPACKET* recvData = (UPACKET*)sendbuffer;
	recvPacketList.push_back(*recvData);

	for (auto packet : recvPacketList)
	{
		UPACKET recvpacket;
		recvpacket.ph = recvData->ph;
		switch (recvpacket.ph.type)
		{
		case PACKET_CHAT_MSG:
		{
			CHAT_MSG* chatmsgRecv = (CHAT_MSG*)recvData->msg;
			std::cout << chatmsgRecv->iID << " " 
					  <<chatmsgRecv->szName << " " 
				      << chatmsgRecv->szMsg;
		}break;
		}
	}	
}
