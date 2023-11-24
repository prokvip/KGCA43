#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable> // event

class TLog
{
public:
	static TLog& Get()
	{
		static TLog log;
		return log;
	}
	std::atomic<bool>		m_bExit;
	bool			m_bThreadStarted =false;
	std::string		m_szLogFilePath;
	std::thread		m_hThread;
	std::mutex		m_Mutex;
	std::mutex		m_MutexStarted;
	std::condition_variable   m_CondVar;
	std::condition_variable   m_CondVarStarted;
	std::queue<std::string>  m_szQueue;
	void			Run();
	void			log(std::string msg);
private:
	TLog();
	~TLog();
	TLog(const TLog& log) = delete;
	TLog& operator = (const TLog& log)=delete;
};

