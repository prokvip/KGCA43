#pragma once
#include "TUser.h"
class TSessionMgr
{
public:
	std::list<TUser*>  g_userlist;
public:
	void		Add(TUser* pUser);
};