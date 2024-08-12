#include "TLog.h"
#include <iostream>
#include <vector>

TLog::TLog() : mExit(false)
{
	mThread = std::thread{&TLog::Process , this};
}
TLog::~TLog()
{
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mExit = true;
		mCondVar.notify_all();
	}
	mThread.join();
}
void TLog::log(const std::string & msg)
{
	std::unique_lock<std::mutex> lk(mMutex);
	mQueue.push(msg);
	mCondVar.notify_all();
}
void TLog::Process()
{
	std::ofstream ofs("log.txt");
	if (ofs.fail())
	{
		std::cout << "Failed" << std::endl;
		return;
	}

	std::unique_lock<std::mutex> lk(mMutex);
	while (true)
	{
		if (!mExit)
		{
			mCondVar.wait(lk);
		}
		lk.unlock();
		while (true)
		{
			lk.lock();
			if (mQueue.empty()) break;
			else {
				ofs << mQueue.front() << std::endl;
				mQueue.pop();
			}
			lk.unlock();
		}

		if (mExit)
		{
			break;
		}
	}
}
