#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#pragma pack(push, 1)
// 패킷 구성 방법
typedef struct
{
	WORD  len;
	WORD  type;	
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER  ph;
	char           msg[2048];
}UPACKET;

typedef struct
{
	char   szName[13];  //1+1	
	int    iID;        // 4
}CHAT_HEADER;
typedef struct
{
	CHAT_HEADER header;
	char   szMsg[1024];   // 4+4
}CHAT_MSG;
typedef struct
{
	byte   szName[13];
	int    iID;
	int    iData[10];
}ITEM_CREATE;

#pragma pack(pop)

// 패킷 타입
#define PACKET_CHAT_MSG			1000
#define PACKET_GAME_START		2000
#define PACKET_GAME_END			3000
#define PACKET_CREATE_CHARACTER 4000
#define PACKET_MOVE_CHARACTER   5000

/*
* UPACKET sendpacket;
* sendpacket.len = PACKET_HEADER_SIZE + "안녕하세여";
* sendpacket.type = PACKET_CHAT_MSG;
* sendpacket.msg = "안녕하세여";
* char * sendbuffer = (char*)sendpacket;
* 
* UPACKET recvpacket;
* memcpy(recvpacket, sendbuffer, sizeof(sendbuffer));
  CHAT_MSG msg;
  memcpy(msg, recvpacket.msg, sizeof(CHAT_MSG));
*/