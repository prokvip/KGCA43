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
	int jValue;
	void show() { std::cout << " " << jValue; }
	TCharacter(int data) : jValue(data) {}
	TCharacter() { jValue = -1; };
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
	TLinkedlist<TCharacter>		m_tList;
	TLinkedlist<TItem>			m_tItemList;
	TLinkedlist<TMap>			m_tmapList;
public:
	void    print(bool bForward);
	void    ShowFL(TNode<TCharacter>* a, TNode<TCharacter>* b);
	void    ShowLF(TNode<TCharacter>* a, TNode<TCharacter>* b);
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

