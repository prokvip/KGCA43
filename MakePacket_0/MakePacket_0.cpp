#include <iostream>
#include "TProtocol.h"
#include <list>
// 패킷 생성 방법( 네트워크 데이터는 1바이트 정렬 )
// 1) 구조체
// 2) 패킷 생성 클래스 
// 3) 클래스 직렬화, 역직렬화
// 4) std::stringstream( sstream ) 

std::list<UPACKET> sendPacketList;
std::list<UPACKET> recvPacketList;
int main()
{    
	std::string msg = "안녕하세여";
	std::string name = "홍길동";
	int         ID = 999;

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
