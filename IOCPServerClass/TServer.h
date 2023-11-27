#pragma once

static const short port = 10000;
#include "TThread.h"
#include "TIocpModel.h"
#include "TSessionMgr.h"
#include "TPacketPool.h"
class TServer;
class TAccept : public TThread
{
	TServer*		m_pServer;
public:
	SOCKET          listensock;
public:
	void		 Set(TServer* pServer)
	{
		m_pServer = pServer;
		CreateThread();
	}
	void	Close();
	virtual bool ThreadRun() override;
	TAccept() { 
		
	}
};

class TServer : public TThread
{	
	TIocpModel		m_Iocp;
	TSessionMgr		m_SessionMgr;
	TAccept			m_Accepter;
public:
	TPacketPool		m_PacketPool;
	TPacketPool		m_PacketBroadcasttingPool;
	typedef std::function<void(T_Packet& t)>  CallFunction;
	typedef std::map<int, CallFunction>::iterator  FunctionIterator;
	std::map<int, CallFunction>  m_fnExecutePacket;

	virtual void	AddPacket(T_Packet& tPacket) {};
	virtual void	ChatMsg(T_Packet& t);
	virtual void	tPACKET_MOVE_CHARACTER(T_Packet& t);
	virtual void	tPACKET_CREATE_CHARACTER(T_Packet& t);

	TIocpModel& GetIocp()
	{
		return m_Iocp;
	}
	HANDLE& GetIocpHandle()
	{
		return m_Iocp.g_hIOCP;
	}
	TSessionMgr& GetSessionMgr()
	{
		return m_SessionMgr;
	}
	TAccept& GetAccepter()
	{
		return m_Accepter;
	}
public:
	virtual bool	Init();
	virtual bool	ThreadRun();
	virtual bool	Release();
	virtual int		SendPacket(TUser* pUser, UPACKET& packet);
	virtual bool	Broadcastting(UPACKET& packet);
};

class TListenServer : public TServer
{
};

//class TZoneUser : public TUser
//{
//public:
//	void			recv2();
//	virtual void    Close();
//	virtual void    bind(HANDLE iocp);
//	virtual void    recv();
//	virtual void    Dispatch(DWORD dwTransfer, OVERLAPPED* ov);
//};
class TZoneServer : public TSingleton<TZoneServer>, public TServer
{
	friend class TSingleton<TZoneServer>;
public:
	virtual void	AddPacket(T_Packet& tPacket) override;
};

class TUpdateServer :  public TServer
{
};

#define I_Server TZoneServer::GetInstance()
