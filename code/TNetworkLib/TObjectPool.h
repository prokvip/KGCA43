#pragma once
#include <windows.h>
#include <malloc.h>

/*
  POOL_MAX_SIZE 요소까지는 객체를 동적할당.
  이후에는 할당된 객체를 재사용한다.
*/
template< class T>
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 1024,// 2의 승수
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1
	};
	static void prepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			m_Pool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
		}
		InterlockedAdd64(&m_TailPos, POOL_MAX_SIZE);
	}
	static void allFree()
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
	// 0000 & 0111 = 0 
	// 0001 & 0111 = 1
	// 0010 & 0111 = 2
	// 0011 & 0111 = 3
	// 0100 & 0111 = 4
	// 0101 & 0111 = 5
	// 0110 & 0111 = 6
	// 0111 & 0111 = 7
	// 
	// 1000 & 0111 = 0
	// 1001 & 0111 = 1
	// 1010 & 0111 = 2
	// 1011 & 0111 = 3
	static void* operator new(size_t size)
	{
		//long long pos = m_HeadPos++;// InterlockedIncrement64(&m_HeadPos) - 1;
		//void* value = m_Pool[pos & POOL_SIZE_MASK];
		// 변수의 값을 증가시킨다.
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		//  교체전 값을 반환한다.
		void* value = InterlockedExchangePointer(&m_Pool[pos & POOL_SIZE_MASK], nullptr);
		if (value != nullptr)
		{
			return value;
		}
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete(void* pObj)
	{
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;		
		void* value = InterlockedExchangePointer(&m_Pool[pos & POOL_SIZE_MASK], pObj);
		if (value != nullptr)
		{
			_aligned_free(value);
		}				
	}
private:
	// 컴파일러의 최적화 방지
	static void* volatile m_Pool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};

template <class T>
void* volatile TObjectPool<T>::m_Pool[POOL_MAX_SIZE] = {};

template <class T>
long long volatile TObjectPool<T>::m_HeadPos(0);

template <class T>
long long volatile TObjectPool<T>::m_TailPos(0);