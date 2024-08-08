#include <windows.h>
#include <iostream>
#include <thread> // os따라서 달리 구현된다.
#include <mutex> 
#include <atomic>
std::mutex mtx;
int iSum = 0;
void Counter(int iStart, int iEnd)
{    
    while (iStart++ < iEnd)
    {        
        std::this_thread::sleep_for(std::chrono::seconds(1));
       std::cout << "ID:"  << std::this_thread::get_id() << std::endl;
    }    
}
class tCounter
{
public:
    static void prcv(int iStart, int iEnd)
    {
        while (iStart++ < iEnd)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "ID:" << std::this_thread::get_id() <<
                std::endl;
        }
    }
    void loop(int iStart, int iEnd)
    {
        while (iStart++ < iEnd)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "ID:" << std::this_thread::get_id() <<
                std::endl;
        }
    }
};
class TThread
{
public:
    std::thread             m_thread;
    std::atomic<bool>       m_bStop =false; // interlock
    virtual void run(int iStart, int iEnd) = 0;
    void start()
    {
        m_thread = std::thread(&TThread::run, this, 0, 10);
    }
    void stop()
    {
        m_bStop = true;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    void suspend()
    {
        // win api
        // HANDLE hThread = CreateThread()
        SuspendThread(m_thread.native_handle());
    }
    void resume()
    {
        // win api
        ResumeThread(m_thread.native_handle());
    }
};
class TThreadJob : public TThread
{
public:
    void run(int iStart, int iEnd)
    {
        while (iStart++ < iEnd)
        {
            if (m_bStop) break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "ID:" << std::this_thread::get_id() <<
                std::endl;
        }
    };
};
int main()
{
    std::cout.sync_with_stdio(true);

    TThreadJob thArray[3];
    thArray[0].start();
    //CloseHandle( thArray[0].m_thread.native_handle()) ;
    thArray[0].m_thread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(3));
    thArray[0].suspend();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    thArray[0].resume();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    thArray[0].stop();
    

    std::thread t1(Counter, 0, 10); // 1CORE
    std::thread t2(&tCounter::prcv, 0, 10);// 2CORE
    tCounter c;
    std::thread t3(&tCounter::loop, &c,  0, 10);// 2CORE

    auto lambda = [](int iStart, int iEnd) -> void
    {
        while (iStart++ < iEnd)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "ID:" << std::this_thread::get_id() << std::endl;
        }
    };
    std::thread t4(lambda, 0, 10);

    std::thread t5(lambda, 0, 10);
    t5.detach();
    //t5.join();
    // CloseHandle( CreateThread() ) // free thread

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    //std::cout << "Sum= " << iSum << std::endl;
}
