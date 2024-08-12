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

void SendMsg(int id, TLog& tlog)
{
	for (int i = 0; i < 3; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::stringstream ss;
		ss << "Start " << i << "thread " << id;
		tlog.log(ss.str());
	}
}
int main()
{	
	TLog tlog;	

	int iCnt = 0;
	while (iCnt++ < 10)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		tlog.log("data");
	}
	tlog.log("data1");
	tlog.log("data2");
	tlog.log("data3");
	tlog.log("data4");
	tlog.log("data5");
	tlog.log("data6");



	std::vector<std::thread> thList;
	for (int i = 0; i < 10; i++)
	{
		thList.push_back(
			std::thread{ SendMsg, i, std::ref(tlog) });
	}
	for (auto& th : thList)
	{
		th.join();
	}

    //std::cout << "Hello World!\n";
}
