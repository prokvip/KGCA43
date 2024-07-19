#pragma once
#include "TNetStd.h"
class TNetwork;
class TSelectModel
{
public:
	SOCKET   m_hSock;
	TNetwork* m_pNet = nullptr;
	virtual bool     Run() = 0;
public:
	TSelectModel()  = default;
	TSelectModel(SOCKET sock);
	virtual ~TSelectModel();
};


