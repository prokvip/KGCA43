#include "TBarGraphObserver.h"
#include "TSubject.h"
void TBarGraphObserver::update()
{
	pScoreListCopy->m_ScoreList.clear();

	for (auto iter = pScoreData->m_ScoreList.begin();
		iter != pScoreData->m_ScoreList.end();
		iter++)
	{
		TScoreCard* pScore = (TScoreCard*)*iter;
		pScoreListCopy->m_ScoreList.push_back(pScore);
	}
}

void   TBarGraphObserver::print()
{
	for( auto iter = pScoreListCopy->m_ScoreList.begin();
		 iter != pScoreListCopy->m_ScoreList.end();
		 iter++)
	{
		TScoreCard* pScore = (TScoreCard*)*iter;
		std::wcout << pScore->name << std::endl;
	}
}

TBarGraphObserver::TBarGraphObserver(TSubject* pScores)
{
	pScores->observes.push_back(this);
	pScoreData = pScores;
	pScoreListCopy = new TSubject;
}