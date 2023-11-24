#pragma once
#include "TNetStd.h"

class TThread
{
public:
	unsigned int m_hThread;
	unsigned int m_iThreadID;
	bool		 m_bStarted;
public:
	void		CreateThread();
	virtual     bool ThreadRun() { return true; }
	static unsigned int WINAPI HandleRunner(LPVOID param);
public:
	TThread();
	virtual ~TThread();
};