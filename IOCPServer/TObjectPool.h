#pragma once
#include <string>
template<class T>
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE =8,
		POOL_SIZE_MASK = POOL_MAX_SIZE-1,
	};
public:
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			void* value = InterlockedExchangePointer(&m_Pool[i], nullptr);
			if (value != nullptr)
			{
				_aligned_free(value);
			}
		}
	}
	static void* operator new(size_t size)
	{
		//m_HeadPos++
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		// InterlockedExchangePointer 이전에 저장된.m_Pool[] 반환한다.
		void* value = InterlockedExchangePointer(&m_Pool[insert], nullptr);

		if (value != nullptr)
		{
			return value;
		}
		//MEMORY_ALLOCATION_ALIGNMENT 16
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete(void* obj)
	{
		//m_TailPos++
		//수 1을(를) '__int64'에서 'volatile LONG64 *'(으)로 변환할 수 없습니다.
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		//void *'에서 'volatile PVOID *'(으)로 변환할 수 없습니다.
		void* value = InterlockedExchangePointer(&m_Pool[insert], obj);
		if (value != nullptr)
		{
			_aligned_free(value);
		}
	}
private:
	static void* volatile m_Pool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};
template<typename T>
void* volatile TObjectPool<T>::m_Pool[POOL_MAX_SIZE] = {};

template<typename T>
long long volatile TObjectPool<T>::m_HeadPos(0);

template<typename T>
long long volatile TObjectPool<T>::m_TailPos(0);

// 1111 = 15
//  POOL_MAX_SIZE=15(1111) , POOL_SIZE_MASK=14(1110)
//  000000 10 0000 0000  1024    1000(8)
//  000000 01 1111 1111  1023    0111(7)
//  pos & POOL_SIZE_MASK
//  HeadPos= 0, Pool[0]    0000 & 0111 = 0 
//  HeadPos= 1, Pool[1]    0001 & 0111 = 1 
//  HeadPos= 2, Pool[2]    0010 & 0111 = 2
//  HeadPos= 7, Pool[7]    0111 & 0111 = 7
//  HeadPos= 8, Pool[0]    1000 & 0111 = 0
//  HeadPos= 9, Pool[1]    1001 & 0111 = 1
//  HeadPos= 10, Pool[2]    1010 & 0111 = 2