#include "TBarGraphObserver.h"
#include "TSubject.h"
void TBarGraphObserver::update()
{
	m_pUpdateList->m_ScoreListBar.clear();

	for (auto iter = m_pScoreData->m_ScoreListBar.begin();
		iter != m_pScoreData->m_ScoreListBar.end();
		iter++)
	{
		//TScoreCard* pScore = iter->get();
		m_pUpdateList->m_ScoreListBar.push_back(*iter);
	}
}

void   TBarGraphObserver::print()
{
	for( auto iter = m_pUpdateList->m_ScoreListBar.begin();
		 iter != m_pUpdateList->m_ScoreListBar.end();
		 iter++)
	{
		TScoreCard* pScore = (TScoreCard*)*iter;
		std::wcout << pScore->name << std::endl;
	}
}

TBarGraphObserver::TBarGraphObserver(TSubject* pScores)
{
	pScores->observes.push_back(this);
	// 스트링 생성자 호출
	m_pUpdateList = std::make_shared<TSubject>(L"bb");		
	//기본 복사생성자 호출
	m_pScoreData = std::make_shared<TSubject>(*pScores);	
	// 포인터를 받는 생성자가 있어야 한다.
	//m_pScoreData = std::make_shared<TSubject>(pScores);
	
}