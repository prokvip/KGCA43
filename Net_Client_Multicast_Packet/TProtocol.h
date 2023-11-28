#pragma once
#include <winsock2.h>
#include <windows.h>

#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 2048
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)

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
	char           msg[PACKET_MSG_BUFFER_SIZE];
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

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8방향
}TPACKET_USER_POSITION;

typedef struct {
	int m_iID;
	int m_iType;
	char m_name[20];
}TPACKET_CREATE_CHARACTER_INFO;

#pragma pack(pop)

// 패킷 타입
#define PACKET_CHAT_MSG			1000
#define PACKET_GAME_START		2000
#define PACKET_GAME_END			3000
#define PACKET_CREATE_CHARACTER 4000
#define PACKET_MOVE_CHARACTER   5000

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // 대화명 입력요청	
	PACKET_CHAT_NAME_ACK, // 대화명 입력 응답
	tPACKET_CHAT_MSG, // 보통 채팅 메세지.
	PACKET_LOGIN_REQ, // 로그인요청
	PACKET_LOGOUT_REQ, // 로그아웃요청
	PACKET_LOGIN_ACK, // 로그인응답
	PACKET_LOGOUT_ACK, // 로그아웃응답
	PACKET_LOGOUT_PLAYER, // 로그아웃 플레이어

	PACKET_CREATE_ACCOUNT_REQ = 2000, // 계정생성요청
	PACKET_DELETE_ACCOUNT_REQ, // 계정삭제요청
	PACKET_CREATE_ACCOUNT_ACK, // 계정생성응답
	PACKET_DELETE_ACCOUNT_ACK, // 계정삭제응답

	PACKET_USER_POSITION = 3000, // 케릭터 위치 이동 시작
	PACKET_STOP_POSITION,// 케릭터 정지
	PACKET_LOBBY_USER_INFO, // 대기방 유저리스트 정보


	PACKET_CREATE_CHARACTER_REQ = 4000, // 케릭터 생성 요청
	PACKET_DELETE_CHARACTER_REQ, // 케릭터 삭제 요청
	PACKET_CREATE_CHARACTER_ACK, // 케릭터 생성 응답
	PACKET_DELETE_CHARACTER_ACK, // 케릭터 삭제 응답

	PACKET_HIT_CHARACTER = 5000, // 케릭터 피격 받음
	PACKET_HIT_NPC, // NPC 피격 받음
	PACKET_ATTACK_CHARACTER, // 케릭터 공격 시작
	PACKET_ATTACK_NPC, // NPC 공격 시작
	PACKET_DEAD_CHARACTER, // 케릭터 죽음
	PACKET_DEAD_NPC, // NPC 죽음
	PACKET_DAMAGE_CHARACTER, // 케릭터 데미지 받음
	PACKET_DAMAGE_NPC, // NPC 데미지 받음
	PACKET_SPAWN_CHARACTER, // 케릭터 스폰
	PACKET_SPAWN_NPC, // NPC 스폰
	PACKET_SPAWN_NPC_LIST, // 
	PACKET_SYNC_CHARACTER, // 케릭터 동기화
	PACKET_SYNC_NPC_LIST, // NPC 동기화	
	PACKET_NPC_TARGETMOVE,  // NPC 타켓이동	


	PACKET_ZONE_ENTRY_REQ = 6000,//게임존 입장 요청
	PACKET_ZONE_ENTRY_ACK, // 게임 존 입장 응답
	PACKET_ZONE_NEW_PLAYER, // 게임 존 입장 케릭터 정보 전송
	PACKET_ZONE_PLAYERS_INFO, // 게임 존에 입장한 유저들의 플레이 케릭터 정보 전송 
	PACKET_ZONE_USER_INFO,// 게임존 유저리스트 정보(플레이어+기타(옵저버 등))
	PACKET_ZONE_TARGET_REQ, // 존 타켓 위치 요청  
	PACKET_ZONE_TARGET_ACK, // 존 타켓 위치 응답
};
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