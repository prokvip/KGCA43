#include "TSessionMgr.h"

void		TSessionMgr::Add(TUser* pUser)
{
	{
		std::lock_guard<std::shared_mutex> lock(m_sMutex);
		// 임계구역
		g_userlist.push_back(pUser);
	}	
}
void		TSessionMgr::DeleteUser()
{
	{
		std::lock_guard<std::shared_mutex> lock(m_sMutex);
        for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
            iterSend != g_userlist.end();
            )
        {
            TUser* pUser = (*iterSend);
            if (pUser->bConneted == false)
            {
                pUser->Close();
                iterSend = g_userlist.erase(iterSend);
            }
            else
            {
                iterSend++;
            }
        }
	}
}