#pragma once
#include <windows.h>
#include <malloc.h>

/*
  POOL_MAX_SIZE ��ұ����� ��ü�� �����Ҵ�.
  ���Ŀ��� �Ҵ�� ��ü�� �����Ѵ�.
*/
template< class T>
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 1024,// 2�� �¼�
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
		// ������ ���� ������Ų��.
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		//  ��ü�� ���� ��ȯ�Ѵ�.
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
	// �����Ϸ��� ����ȭ ����
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