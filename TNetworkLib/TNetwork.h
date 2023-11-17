#pragma once
#include "TStdSelect.h"
#include "TEventSelect.h"
#include "TAsyncSelect.h"
class TNetwork : public TNetBase
{
public:
	SOCKET	m_hSock;
	int		m_iPort= 10000;
	TSelectModel* m_pSelectModel = nullptr;
public:
	virtual bool  Init();
	virtual bool  Run();
	virtual bool  Release();
};
class TNetworkServer : public TNetwork
{
public:
	virtual bool  Init();
	virtual bool  Run();
};

class TNetworkClient : public TNetwork
{
public:
	virtual bool  Init();
	virtual bool  Run();
};