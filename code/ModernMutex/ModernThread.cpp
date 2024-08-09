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

//
std::mutex mtx;
std::recursive_mutex rMtx;
int iSum = 0;
// 
//// c++11 -> std::thread
//// c++11 -> mutex, timed_mutex, recusive_mutex, recusive_timed_mutex
//// c++11 -> lock_guard, unique_lock, lock(), try_lock()
//// unique_lock의 옵션 -> std::adopt_lock(lock된 상태에서 다시 lock함), std::defer_lock(원하는 시점에 잠금), std::try_to_lock(잠금 여부를 시도해보고 잠금)

//// ** read, write access
//// c++14 -> std::shared_lock(), shared_timed_lock()
//// c++17 -> std::shared_mutex(), std::scoped_lock()
//// c++20 -> std::jthread, stop_token, strop_source
class tBase
{
public:
    CRITICAL_SECTION cs;
    tBase()
    {
        InitializeCriticalSection(&cs);
    }
    ~tBase()
    {
        DeleteCriticalSection(&cs);
    }
};
class tLock
{
    tBase* pBase = nullptr;
public:
    tLock(tBase* obj) : pBase(obj)
    {
        EnterCriticalSection(&obj->cs);
    }
    ~tLock()
    {
        if (pBase != nullptr)
        {
            LeaveCriticalSection(&pBase->cs);
        }
    }
    
};

class tObject : public tBase
{
    int   m_Value = 0;
public:
    void run()
    {
        {
            tLock lock(this);
            m_Value++;
        }
    }    
};



void ProcessRecursive_mutex()
{
    rMtx.lock();
        iSum++;
        std::cout << "ID:" << std::this_thread::get_id() << std::endl;
        rMtx.unlock();
}
void TestRecursive_mutex()
{
    rMtx.lock();
        ProcessRecursive_mutex();
    rMtx.unlock();
}

void Counter(int iStart, int iEnd)
{    
    while (iStart++ < iEnd)
    {        
       this_thread::sleep_for(seconds(1));
       // 임계영역 ( A스레드가 lock() 반드시 A스레드만 unlock() )
       mtx.lock();
        iSum++;
       std::cout << "ID:" << std::this_thread::get_id() << std::endl;
       mtx.unlock();
       
       // std::lock_guard   복사, 대입 X
       //{
       //    std::lock_guard<std::mutex> lock(mtx); // 생성자(lock()), 해제자(unlock)           
       //    iSum++;
       //    std::cout << "ID:" << std::this_thread::get_id() << std::endl;
       //}

       //std::unique_lock 복사, 대입 X, 이동 O
      /* {
           std::unique_lock<std::mutex> lock1(mtx);
            iSum++;
            std::cout << "ID:" << std::this_thread::get_id() << std::endl;
       }*/

       //{
       //    std::unique_lock lock1(mtx, std::defer_lock);
       //    std::unique_lock lock2(mtx, std::defer_lock);
       //    //std::lock(lock1, lock2); // 동시에 lock() 가능

       //    // 복사,대입 X -> 이동 가능
       //    std::unique_lock<std::mutex> lock1(mtx, std::defer_lock); // 생성자(lock()), 해제자(unlock)
       //    // 기타 작업을 하고 
       //    // 이시점에 lock() 가능하다.
       //    lock1.lock();
       //     iSum++;
       //     std::cout << "ID:" << std::this_thread::get_id() << std::endl;
       //    lock1.unlock();
       //}
       
    }    
}
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
    int add(int id)
    {
        //// 오직 1개의 스레드만 접근 가능하다.
        ////  대기 중인 스레드가 존재하면 모든 스레드가 처리될 때까지 read 할수 없다.
        {
            std::lock_guard lg{ smtx };
            g_iCnt++;
            std::cout << id << "->write" << "[" << g_iCnt << "]" << std::endl;
            return g_iCnt;
        }
    }
    // read 전용
    int getValue(int id)
    {
        // 다수의 스레드가 접근 가능하다.
        std::shared_lock sl{ smtx };
        std::cout << id << "->read" << "[" << g_iCnt << "]" << std::endl;
        return g_iCnt;
    }

    static void threadFunWrite(tData& data, int id)
    {
        for( int i=0; i < 3; i++)
        {          
            data.add(id);
        }
    }
    static void threadFunRead(tData& data, int id)
    {
        for (int i = 0; i < 3; i++)
        {
            data.getValue(id);
        }
    }
};


int main()
{
    // legacy window api 사용방법(critical section)
    {
        tObject obj;
        obj.run();
    }

    //recursive_mutex
    {
        TestRecursive_mutex();
    }

    // 1초 동안 lock() 시도함.
    {
        std::timed_mutex tMutex;
        std::unique_lock<std::timed_mutex> lock(tMutex, std::chrono::seconds(1));
    }

    // 1초 동안 lock() 시도함.
    {
        std::shared_timed_mutex stMutex;
        std::unique_lock<std::shared_timed_mutex> lock(stMutex, std::chrono::seconds(1));
    }

    //
    {
        std::mutex m1, m2;
        std::lock(m1, m2); 
        std::unique_lock<std::mutex> lock1(m1, std::adopt_lock); // 입양 받아서 lock 한다.
        std::unique_lock<std::mutex> lock2(m2, std::adopt_lock);        
    }

    {
        std::mutex m1, m2;      
        std::unique_lock<std::mutex> lock1(m1, std::defer_lock);
        std::unique_lock<std::mutex> lock2(m2, std::defer_lock);
        std::lock(lock1, lock2);       
    }

    {
        std::mutex m1, m2;
        std::unique_lock<std::mutex> lock1(m1, std::try_to_lock);
        if(lock1){}
    }

    {
        using namespace std::chrono_literals;
        std::timed_mutex mux;
        // 2초 동안 잠금을 유지함.
        auto t = std::jthread([&mux]
            {
                std::unique_lock lock(mux);
                std::this_thread::sleep_for(2s);
            });

        auto runner = [&mux]
        {
            // 잠금을 얻으려고 시도하고 200ms 후에 포기한다. 
            std::unique_lock lock(mux, 200ms);
            // 내부적으로 다음이 호출된다.
            // for a time point: mux.try_lock_until()
            // for a duration:   mux.try_lock_for()
            if (!lock.owns_lock()) {
                std::cout << "Unable to obtain lock" << std::endl;
                return;
            }
            std::cout <<"Lock was obtained" << std::endl;
        };

        std::this_thread::sleep_for(100ms);
        // t 스레드가 점유하고 있어서 0,1, 번은 시간초과 , 2, 3 번은 lock() 획득
        for (size_t i = 0; i < 4; ++i) {
            auto r = std::jthread(runner);
            std::this_thread::sleep_for(1s);
        }
    }



    

    std::vector < std::thread>  thList2;
    thList2.emplace_back(Counter, 0, 3);
    thList2.emplace_back(Counter, 0, 3);
    thList2.emplace_back(Counter, 0, 3);
    for (auto& th : thList2)
    {
        th.join();
    }
    std::cout << "Sum= " << iSum << std::endl;



    tData data;
    std::vector < std::thread>  thList1;
    thList1.emplace_back(tData::threadFunWrite, std::ref(data), 0);    
    thList1.emplace_back(tData::threadFunRead, std::ref(data),1);
    thList1.emplace_back(tData::threadFunWrite, std::ref(data), 2);
    thList1.emplace_back(tData::threadFunRead, std::ref(data),3);
    for (auto& th : thList1)
    {
        th.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    
}
