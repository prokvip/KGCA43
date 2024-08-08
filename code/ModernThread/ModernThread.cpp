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
class Runnable
{
public:
    Runnable() : m_stop(), m_thread() {}
    virtual ~Runnable() { stop(); }

    Runnable(Runnable const&) = delete;
    Runnable& operator = (Runnable const&) = delete;

    void start()
    {
        m_thread = std::thread(&Runnable::run, this);
    }
    void stop()
    {
        m_stop = true;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    void suspend() { SuspendThread(m_thread.native_handle()); }
    void resume() { ResumeThread(m_thread.native_handle()); }
    bool isStop() { return m_stop; }
protected:
    virtual void run() = 0;
    std::atomic<bool> m_stop;
public:
    std::thread m_thread;
};
class myThread : public Runnable
{
protected:
    void run()
    {
        while (!m_stop)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
            std::cout << "." ;
        }
    }
};
int main()
{
    std::cout.sync_with_stdio(true);

    myThread t;
    t.start();
    //HANDLE threadID = t.m_thread.native_handle();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    t.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    t.resume();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    t.stop();
    

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
