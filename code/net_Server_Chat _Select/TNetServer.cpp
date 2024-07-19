#include "TNetServer.h"
void   TNetServer::Run()
{
	_ASSERT(m_pSelectIO);
    while (m_bRun)
    {
        if (!m_pSelectIO->Run())
        {
            break;
        }
    }
}