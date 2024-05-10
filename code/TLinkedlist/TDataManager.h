#pragma once
#include "TLinkedlist.cpp"
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
	// 복사생성자
	TCharacter(const TCharacter& copy)
	{
		jValue = copy.jValue;
	}
	// 대입연산자
	void operator = (const TCharacter& copy)
	{
		jValue = copy.jValue;
	}	
public:
	int jValue;
	void show() { std::cout << " " << jValue; }
	TCharacter(int data) : jValue(data) 
	{
		std::cout << "TCharacter";
	}
	TCharacter(int data, float dataF ) : jValue(data)
	{
		std::cout << "TCharacter";
	}
	TCharacter() { 
		jValue = -1; 
	};
	~TCharacter() {
		jValue = -1; 
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
class TDataManager
{
public:
public:
	TLinkedlist<TCharacter>		m_tList;
	TLinkedlist<TItem>			m_tItemList;
	TLinkedlist<TMap>			m_tmapList;
public:
	void    print(bool bForward);
	void    ShowFL(TIterator<TCharacter>* a, TIterator<TCharacter>* b);
	void    ShowLF(TIterator<TCharacter>* a, TIterator<TCharacter>* b);
	void    Save();
	int     Load();
	void    AllDeleteData();
public:
	void NewData();
	void FileSave();
	void FileLoad();
	void Update();
	void DelData(int iData);
	void sort();
	void Sample();

	TCharacter* NewData(int data);
};

