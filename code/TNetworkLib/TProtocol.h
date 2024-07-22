#pragma once
#define MAX_MSG_SIZE 1024
struct FVector
{
	float x;
	float y;
};
#pragma pack(push, 1)
typedef struct {
	short       iotype;	// ��Ŷ ����� ����
	short  		len;	// msg ����Ʈ ũ��
	short    	type;	// � ��Ŷ�̴�.	
	short       time;   // ��Ŷ ���� �ð� 
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[MAX_MSG_SIZE];
}UPACKET, * P_UPACKET;

#define PACKET_HEADER_SIZE 8

typedef struct {
	short	user_idx;
	short	posX;
	short	posY;
	short	direction; //0 ~7 8����
}TPACKET_USER_POSITION;

typedef struct {
	FVector vPosition;
	FVector vVelocity;
	FVector vTarget;
	//FRotator vRotation;
	float	fHealth;
	int     iType;
	bool	bAlive;     // ������ ���ɿ���(�ߺ�üũ��)
	bool	bAttack;    // ���� ���ɿ���(�ߺ�üũ��)
	bool	bDamage;	// ������ ���ɿ���(�ߺ�üũ��)
}TPACKET_CHARACTER_INFO;

//------------------------------------------------------------------------------
typedef struct
{
	short		Point;				// �̺й� ����Ʈ.
	short		STR;				// ��.
	short		INT;				// ����.
	short		DEX;				// ��ø.
	short		CHA;				// ī������.
	short		Life;				// ���� �����. (�� ����)
	short		Stamina;			// ���� ������. (�� ����)
	short		Status;				// ���� ���� ��.
	short		Skins[6];			// ���� ��� ����.(0:�Ӹ�, 1:��)
	short		Parts[4];			// ������ ���� ��Ȳ. 
	short		Level;				// ����.
	int			Exp;				// ����ġ.
	int			Skills[16];			// ��ų ���õ�. (0->10,000)

}TSTATUS_VARS;

//------------------------------------------------------------------------------
typedef struct
{
	short		Rank;				// ���.
	short		Medals;				// ȹ���� ���� ��.
	short		Fame;				// �� ��ġ.
	short		Win;				// ��.
	short		Lose;				// ��.

}TSCORE_VARS;

typedef struct
{
	int     iType;
	FVector vPosition;
}TPACKET_TARGETPOINT_INFO;

#pragma pack(pop)

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // ��ȭ�� �Է¿�û	
	PACKET_CHAT_NAME_ACK, // ��ȭ�� �Է� ����
	PACKET_CHAT_MSG, // ���� ä�� �޼���.
	PACKET_LOGIN_REQ, // �α��ο�û
	PACKET_LOGOUT_REQ, // �α׾ƿ���û
	PACKET_LOGIN_ACK, // �α�������
	PACKET_LOGOUT_ACK, // �α׾ƿ�����
	PACKET_LOGOUT_PLAYER, // �α׾ƿ� �÷��̾�

	PACKET_CREATE_ACCOUNT_REQ = 2000, // ����������û
	PACKET_DELETE_ACCOUNT_REQ, // ����������û
	PACKET_CREATE_ACCOUNT_ACK, // ������������
	PACKET_DELETE_ACCOUNT_ACK, // ������������

	PACKET_USER_POSITION = 3000, // �ɸ��� ��ġ �̵� ����
	PACKET_STOP_POSITION,// �ɸ��� ����
	PACKET_LOBBY_USER_INFO, // ���� ��������Ʈ ����


	PACKET_CREATE_CHARACTER_REQ = 4000, // �ɸ��� ���� ��û
	PACKET_DELETE_CHARACTER_REQ, // �ɸ��� ���� ��û
	PACKET_CREATE_CHARACTER_ACK, // �ɸ��� ���� ����
	PACKET_DELETE_CHARACTER_ACK, // �ɸ��� ���� ����

	PACKET_HIT_CHARACTER = 5000, // �ɸ��� �ǰ� ����
	PACKET_HIT_NPC, // NPC �ǰ� ����
	PACKET_ATTACK_CHARACTER, // �ɸ��� ���� ����
	PACKET_ATTACK_NPC, // NPC ���� ����
	PACKET_DEAD_CHARACTER, // �ɸ��� ����
	PACKET_DEAD_NPC, // NPC ����
	PACKET_DAMAGE_CHARACTER, // �ɸ��� ������ ����
	PACKET_DAMAGE_NPC, // NPC ������ ����
	PACKET_SPAWN_CHARACTER, // �ɸ��� ����
	PACKET_SPAWN_NPC, // NPC ����
	PACKET_SPAWN_NPC_LIST, // 
	PACKET_SYNC_CHARACTER, // �ɸ��� ����ȭ
	PACKET_SYNC_NPC_LIST, // NPC ����ȭ	
	PACKET_NPC_TARGETMOVE,  // NPC Ÿ���̵�	


	PACKET_ZONE_ENTRY_REQ = 6000,//������ ���� ��û
	PACKET_ZONE_ENTRY_ACK, // ���� �� ���� ����
	PACKET_ZONE_NEW_PLAYER, // ���� �� ���� �ɸ��� ���� ����
	PACKET_ZONE_PLAYERS_INFO, // ���� ���� ������ �������� �÷��� �ɸ��� ���� ���� 
	PACKET_ZONE_USER_INFO,// ������ ��������Ʈ ����(�÷��̾�+��Ÿ(������ ��))
	PACKET_ZONE_TARGET_REQ, // �� Ÿ�� ��ġ ��û  
	PACKET_ZONE_TARGET_ACK, // �� Ÿ�� ��ġ ����
};