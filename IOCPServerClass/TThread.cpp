#include "TThread.h"
void		TThread::CreateThread()
{
	if (m_bStarted == true) return;
	m_bStarted = true;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
}
unsigned int WINAPI TThread::HandleRunner(LPVOID param)
{
	TThread* pThread = (TThread*)param;
	while (pThread->m_bStarted)
	{
		if (!pThread->ThreadRun())
		{
			break;
		}
		Sleep(1);
	}
	return 0;
}
TThread::TThread() : m_bStarted(false)
{

}
TThread::~TThread()
{
	CloseHandle((HANDLE)m_hThread);
};