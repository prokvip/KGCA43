#pragma once

static const short port = 10000;
#include "TThread.h"
#include "TIocpModel.h"
#include "TSessionMgr.h"

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

class TZoneServer : public TServer
{
};

class TUpdateServer : public TServer
{
};