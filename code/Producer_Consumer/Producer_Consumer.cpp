#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

std::queue<int> notiQueue;
std::mutex mtx;
std::condition_variable produce;
std::condition_variable consume;


const int MAXSIZE = 10;
// 생산자 -> 소비자 패턴
void consumer(int id)
{
    while (1)
    {
        this_thread::sleep_for(chrono::milliseconds(1250));

        std::unique_lock<mutex> lk(mtx);

        // notiQueue 데이터 push 할때가지 대기한다.
        consume.wait(lk, []
            {
                return notiQueue.size() != 0;
            });

        std::cout << "->consumer " << id << ":";
        notiQueue.pop();
        std::cout << notiQueue.size() << endl;

        produce.notify_all();
        //lk.unlock();
    }
}
void producer(int id)
{
    while (1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
                    
        std::unique_lock<mutex> lk(mtx);
        // MAXSIZE 도달하면 pop 할 대까지 대기한다.
        produce.wait(lk, []
            {
                return notiQueue.size() != MAXSIZE;
            });     
          
        std::cout << "->producer " << id << ":";
        notiQueue.push(id);
        std::cout << notiQueue.size() << endl;
        consume.notify_all();
       // lk.unlock();        
    }
}
int main()
{
    //std::thread t1(consumer, 0);

    std::thread consumers[4];
    std::thread producers[3];
    for (int i = 0; i < 4; i++)
    {
        consumers[i] = std::thread(consumer, i + 1);
    }
    for (int i = 0; i < 3; i++)
    {
        producers[i] = std::thread(producer, i + 5);
    }

    for (int i = 0; i < 4; i++)
    {
        consumers[i].join();
    }
    for (int i = 0; i < 3; i++)
    {
        producers[i].join();
    }
    std::cout << "Hello World!\n";
}
