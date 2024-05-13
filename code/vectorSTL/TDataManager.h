#pragma once
#include <iostream>
#include <vector>
class TObject
{
public:
	virtual void show() = 0;
};
class TItem : public TObject
{
public :
	int iValue;
	void show() { std::cout << " " << iValue; }
	TItem(int data) : iValue(data) {}
	TItem() { iValue = -1; };
};
class TCharacter : public TObject
{
public:
	std::vector<int> m_list;
	int  jValue;
	int* m_pData = nullptr;
public:	
	//// 복사생성자
	TCharacter(const TCharacter& copy)
	{
		jValue = copy.jValue;
		
		m_pData = new int[3];
		m_pData[0] = copy.m_pData[0];
		m_pData[1] = copy.m_pData[1];
		m_pData[2] = copy.m_pData[2];
		
		m_list = copy.m_list;
	}
	// 대입연산자
	void operator = (const TCharacter& copy)
	{
		jValue = copy.jValue;
		m_pData = new int[3];
		m_pData[0] = copy.m_pData[0];
		m_pData[1] = copy.m_pData[1];
		m_pData[2] = copy.m_pData[2];

		m_list = copy.m_list;
	}	
	// 이동생성자
	TCharacter(TCharacter&& copy) noexcept
	{
		jValue = copy.jValue;
		m_pData = copy.m_pData;
		m_list = std::move(copy.m_list);		
		copy.m_pData = nullptr;
	}
	 ///이동대입연산자
	void operator = (TCharacter&& copy) noexcept
	{
		jValue = copy.jValue;
		// 이미 m_pData가 할당되어 있으면
		if (m_pData != nullptr) delete[]  m_pData;
		if (!m_list.empty()) m_list.clear();
		m_pData = copy.m_pData;
		m_list = std::move(copy.m_list);
		copy.m_pData = nullptr;
	}
public:
	void show() { std::cout << " " << jValue; }
	/*TCharacter(int data) : jValue(data) 
	{
		m_pData = new int[3];
		m_pData[0] = 1;
		m_pData[1] = 2;
		m_pData[2] = 3;

		m_list.push_back(9);
		m_list.push_back(8);
		m_list.push_back(7);
		m_list.push_back(6);
		std::cout << "TCharacter";
	}*/
	TCharacter(int&& data) : jValue(data)
	{
		m_pData = new int[3];
		m_pData[0] = 1;
		m_pData[1] = 2;
		m_pData[2] = 3;

		m_list.push_back(9);
		m_list.push_back(8);
		m_list.push_back(7);
		m_list.push_back(6);
		std::cout << "TCharacter";
	}
	TCharacter(int data, float dataF ) : jValue(data)
	{
		std::cout << "TCharacter";
	}
	TCharacter() { 
		jValue = -1; 
	};
	virtual ~TCharacter() {
		jValue = -1; 
		delete[] m_pData;
		m_pData = nullptr;
		m_list.clear();
	};
};
class TMap : public TObject
{
public:
	int kValue;
	void show() { std::cout << " " << kValue; }
	TMap(int data) : kValue(data) {}
	TMap() { kValue = -1; };
};
