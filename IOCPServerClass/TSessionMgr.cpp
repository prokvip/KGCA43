#include "TSessionMgr.h"

void		TSessionMgr::Add(TUser* pUser)
{
	g_userlist.push_back(pUser);
}