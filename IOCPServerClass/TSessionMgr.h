#pragma once
#include "TUser.h"
class TSessionMgr
{
public:
	std::shared_mutex m_sMutex;
	std::list<TUser*>  g_userlist;
public:
	void		Add(TUser* pUser);
	void		DeleteUser();
};