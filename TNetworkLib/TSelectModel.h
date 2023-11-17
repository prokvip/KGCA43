#pragma once
#include "TNetBase.h"

class TSelectModel : public TNetBase
{
	SOCKET m_hSocck;
public:
	virtual bool Init() = 0;
	virtual bool Release() = 0;
	virtual bool Run() = 0;
public:
	TSelectModel() {}
	TSelectModel(SOCKET sock) : m_hSocck(sock) {}
	virtual ~TSelectModel();
};
