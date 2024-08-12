#pragma once
#include <queue>

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>
#include <sstream>
class TLog
{
private:
	std::mutex				mMutex;
	std::condition_variable mCondVar;
	std::thread				mThread;
	std::queue<std::string> mQueue;
	std::atomic<bool>		mExit;
public:
	TLog();
	virtual ~TLog();

	TLog(const TLog& s) = delete;
	TLog& operator=(const TLog& s) = delete;
public:
	void log(const std::string& msg);
	void Process();
};

