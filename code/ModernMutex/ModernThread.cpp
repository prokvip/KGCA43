#include <windows.h>
#include <iostream>
#include <thread> // os따라서 달리 구현된다.
#include <mutex> //recusive_mutex
#include <shared_mutex> //shared_mutex
#include <atomic>
#include <vector>
#include <string>
using namespace std;
using namespace std::chrono;

//// c++11 -> std::thread
//// c++11 -> mutex, timed_mutex, recusive_mutex, recusive_timed_mutex
//// c++11 -> lock_guard, unique_lock, lock(), try_lock()
//
//// ** read, write access
//// c++14 -> std::shared_lock(), shared_timed_lock()
//// c++17 -> std::shared_mutex(), std::scoped_lock()
//// c++20 -> std::jthread, stop_token, strop_source
//class tBase
//{
//public:
//    CRITICAL_SECTION cs;
//    tBase()
//    {
//        InitializeCriticalSection(&cs);
//    }
//    ~tBase()
//    {
//        DeleteCriticalSection(&cs);
//    }
//};
//class tLock
//{
//    tBase* pBase = nullptr;
//public:
//    tLock(tBase* obj) : pBase(obj)
//    {
//        EnterCriticalSection(&obj->cs);
//    }
//    ~tLock()
//    {
//        if (pBase != nullptr)
//        {
//            LeaveCriticalSection(&pBase->cs);
//        }
//    }
//    
//};
//
//class tObject : public tBase
//{
//    int   m_Value = 0;
//public:
//    void run()
//    {
//        {
//            tLock lock(this);
//            m_Value++;
//        }
//    }    
//};
//
//std::mutex mtx;
//std::recursive_mutex rMtx;
//int iSum = 0;
//
////void Process()
////{
////    if (mtx.try_lock())
////    {
////        //mtx.lock(); // deadlock
////            iSum++;
////            std::cout << "ID:" << std::this_thread::get_id() << std::endl;
////        //mtx.unlock();
////    }
////}
////void Counter(int iStart, int iEnd)
////{    
////    while (iStart++ < iEnd)
////    {        
////       this_thread::sleep_for(seconds(1));
////       // 임계영역 ( A스레드가 lock() 반드시 A스레드만 unlock() )
////       //mtx.lock();
////       // iSum++;
////       //std::cout << "ID:" << std::this_thread::get_id() << std::endl;
////       //mtx.unlock();
////       
////       // std::lock_guard   복사, 대입 X
////       ////{
////       ////    std::lock_guard<std::mutex> lock(mtx); // 생성자(lock()), 해제자(unlock)           
////       ////    iSum++;
////       ////    std::cout << "ID:" << std::this_thread::get_id() << std::endl;
////       ////}
////
////       //std::unique_lock 복사, 대입 X, 이동 O
////       {
////           std::unique_lock<std::mutex> lock1(mtx);
////            iSum++;
////            std::cout << "ID:" << std::this_thread::get_id() << std::endl;
////       }
////
////       //{
////       //    std::unique_lock lock1(mtx, std::defer_lock);
////       //    std::unique_lock lock2(mtx, std::defer_lock);
////       //    //std::lock(lock1, lock2); // 동시에 lock() 가능
////
////       //    // 복사,대입 X -> 이동 가능
////       //    std::unique_lock<std::mutex> lock1(mtx, std::defer_lock); // 생성자(lock()), 해제자(unlock)
////       //    // 기타 작업을 하고 
////       //    // 이시점에 lock() 가능하다.
////       //    lock1.lock();
////       //     iSum++;
////       //     std::cout << "ID:" << std::this_thread::get_id() << std::endl;
////       //    lock1.unlock();
////       //}
////       
////    }    
////}
//std::shared_timed_mutex stMutex;
//
////shared_timed_mutex :  다수의 스레드가 동시에 공유 데이터에 엑세스 방지한다.
//// 접근 권한 2가지 있다.
//// 독점(write) :  1개의 스레드만 소유권을 갖는다.
//// 공유(read)  : 다수의 스레드가 소유권을 공유한다.
//void fn1(int id)
//{
//    {
//        std::shared_lock<std::shared_timed_mutex> sLock(stMutex);
//        std::cout << id << "read" << std::this_thread::get_id() << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(3));
//        std::cout << id << "write" << std::this_thread::get_id() << std::endl;
//    }
//    std::cout << id << std::this_thread::get_id() << std::endl;
//}





class tData
{
private:
    int   g_iCnt = 0;
    std::shared_mutex smtx;
public:
    
    tData()
    {
        std::cout << this_thread::get_id() << std::endl;
    }   
    // write 전용
    int add()
    {
        //// 오직 1개의 스레드만 접근 가능하다.
        ////  대기 중인 스레드가 존재하면 모든 스레드가 처리될 때까지 read 할수 없다.
        {
            std::lock_guard lg{ smtx };
            g_iCnt++;
        }
    }
    // read 전용
    int getValue()
    {
        // 다수의 스레드가 접근 가능하다.
        std::shared_lock sl{ smtx };
        return g_iCnt;
    }

    static void threadFun(tData& data)
    {
        while(1)
        {            
            data.getValue();
            data.add();
            std::cout <<  this_thread::get_id() << std::endl;
        }
    }
};


int main()
{
    tData data;
    std::vector < std::thread>  thList;
    thList.emplace_back(tData::threadFun, std::ref(data));
    thList.emplace_back(tData::threadFun, std::ref(data));
    thList.emplace_back(tData::threadFun, std::ref(data));
    for (auto& th : thList)
    {
        th.join();
    }

   /* std::vector < std::thread>  thList;
    thList.emplace_back(Counter, 0, 3);
    thList.emplace_back(Counter, 0, 3);
    thList.emplace_back(Counter, 0, 3);    
    for (auto& th : thList)
    {
        th.join();
    }

    std::cout << "Sum= " << iSum << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));*/
    
}
