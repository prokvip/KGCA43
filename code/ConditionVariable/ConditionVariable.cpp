#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
// winapi Event : 스레드와 스레드의 통신(알림)을  담당한다.
std::mutex  g_mtx;
std::condition_variable  g_cv;
bool g_bBus = 0;
void wait()
{
    // g_mtx를 다른 곳에서 lock() 하지 않았다면 
    {
        std::unique_lock<std::mutex> lk(g_mtx);
        std::cout << "start waiting!" << std::this_thread::get_id() << std::endl;
              
        //g_cv.wait함수가 호출이 되면 unlock() 된다.
        g_cv.wait(lk, [] { return g_bBus == true; });       
    }
    //while (!g_bBus)
    //{
    //    // 반드시 wait  후에 통지가 된다면 사용 가능하다.
    //    g_cv.wait(lk); // 대기함수
    //}
    std::cout << "finished waiting!" << std::this_thread::get_id() << std::endl;
}
void signal()
{
    std::cout << "Nitifying!" << std::endl;
    {
        std::lock_guard<std::mutex> lk(g_mtx);
        g_bBus = 1;
    }
    //g_cv.notify_one();
    g_cv.notify_all();
}
int main()
{    
    std::thread t0(wait);
    std::thread t1(wait);    
    std::thread t2(wait);

    std::this_thread::sleep_for(std::chrono::seconds(1));    
    std::thread t3(signal);    
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //std::thread t4(signal);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //std::thread t5(signal);

    t1.join();
    t2.join();
    std::cout << "Hello World!\n";
}
