#include "TLog.h"
#include <fstream>
#include <iostream>

// thread1                          
// thread2           log(msg)       threadWrite
// thread3                          

TLog::TLog()
{
	system("md ..\\..\\log");
	time_t now = time(NULL);
	struct tm lt;
	localtime_s(&lt, &now);
	m_szLogFilePath.resize(100);
	strftime(&m_szLogFilePath.at(0), 100,
		"../../log/%Y년%m월%d일%H시%M분%S초.txt", &lt);

	m_hThread = std::thread{ &TLog::Run, this };

	std::unique_lock<std::mutex> lock(m_MutexStarted);
	m_CondVarStarted.wait(lock, 
		[&]() { return m_bThreadStarted == true;}
	);
}
void TLog::log( std::string msg)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_szQueue.push(msg);
	m_CondVar.notify_all();	
}
void TLog::Run()
{
	std::ofstream ofs(m_szLogFilePath);
	if (ofs.fail())
	{
		return;
	}

	std::unique_lock<std::mutex> lock(m_Mutex);
	m_bThreadStarted = true;
	m_CondVarStarted.notify_all();

	while (1)
	{
		m_CondVar.wait(lock);
		lock.unlock();

		while (1)
		{
			lock.lock();
				if (m_szQueue.empty())
				{
					break;
				}
				else
				{
					ofs << m_szQueue.front() << std::endl;
					m_szQueue.pop();
				}
			lock.unlock();
		}
		if (m_bExit)
		{
			break;
		}
	}
}
TLog::~TLog()
{
	m_bExit = true;
	m_CondVar.notify_all();
	m_hThread.join();
}