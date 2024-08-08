// FalseSharingTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <windows.h>
#include <iostream>
#include <thread>	// for 멀티 쓰레드 프로그래밍
#include <chrono>	// for 시간 측정
using namespace std;
constexpr int CACHE_LINE_SIZE = 64;	// 캐시 라인 사이즈: 64B
constexpr _int64 NUMBERS = 1000000000LL;
#define FASLE_SHARING
//#undef FASLE_SHARING

#ifndef FASLE_SHARING
    volatile _int64 num1 = 0;
    volatile _int64 num2 = 0;
#else
    alignas(CACHE_LINE_SIZE)volatile _int64 num1 = 0;
    alignas(CACHE_LINE_SIZE)volatile _int64 num2 = 0;
#endif
void job1()
{
    for (_int64 i = 0; i < NUMBERS; ++i) {
        num1 += 1LL;
    }
}

void job2()
{
    for (_int64 i = 0; i < NUMBERS; ++i) {
        num2 += 1LL;
    }
}

int main()
{
    auto beginTime = chrono::high_resolution_clock::now();
        std::thread task1(job1);
        std::thread task2(job2);

        task1.join();
        task2.join();

    auto endTime = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTime = endTime - beginTime;

    cout << "num1: " << num1 << endl;
    cout << "num2: " << num2 << endl;
    cout << elapsedTime.count() << "s" << endl; return 0;
}
