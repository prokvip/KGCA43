#pragma once
#define MAX_WORKER_THREAD 2
static const short port = 10000;
#include "TIocpModel.h"
#include "TSessionMgr.h"

class TServer
{
	SOCKET          listensock;
	TIocpModel		m_Iocp;
	TSessionMgr		m_SessionMgr
public:
	virtual bool	Init();
	virtual bool	Run();
	virtual bool	Relese();
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