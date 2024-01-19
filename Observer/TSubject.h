#pragma once
#include <list>
#include "TScoreCard.h"
#include "TObserver.h"
class TObserver;
class TSubject
{
public:
	std::list<TScoreCard*>	m_ScoreListBar;
	std::list<std::shared_ptr<TScoreCard>>  m_ScoreListLine;
	std::list<TObserver*>  observes;
	std::wstring name;

	template<typename T>
	void operator()(T* p) {
		std::wcout << L"operator()" << std::endl;
		delete p;
	}
public:
	void AddScore(TScoreCard* score);
	/*TSubject(const TSubject& src)
	{
		int k = 0;
	}*/
	/*TSubject(TSubject* src)
	{
		int k = 0;
	}*/
	TSubject() {
		std::wcout << L"~TSubject" << std::endl;
	};// = default;
	TSubject(const std::wstring& _name) : name{ _name } {
		std::wcout << L"~TSubject" << std::endl;
	}

	virtual ~TSubject()
	{
		std::wcout << L"~TSubject" << std::endl;
	}
};

