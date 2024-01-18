#include "TLineGraphObserver.h"
#include "TSubject.h"
void TLineGraphObserver::update()
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

void   TLineGraphObserver::print()
{
	for (auto iter = pScoreListCopy->m_ScoreList.begin();
		iter != pScoreListCopy->m_ScoreList.end();
		iter++)
	{
		TScoreCard* pScore = (TScoreCard*)*iter;
		std::wcout << pScore->name << std::endl;
	}
}
TLineGraphObserver::TLineGraphObserver(TSubject* pScores)
{
	pScores->observes.push_back(this);
	pScoreData = pScores;
	pScoreListCopy = new TSubject;
}