#include "TLineGraphObserver.h"
#include "TSubject.h"
void TLineGraphObserver::update()
{
	TSubjectNew pSave = std::make_shared<TSubject>();
	for (auto iter = m_pScoreData->m_ScoreListLine.begin();
		iter != m_pScoreData->m_ScoreListLine.end();
		iter++)
	{
		auto pScore = std::make_shared<TScoreCard>();
			pScore->name = (*iter)->name;
			pScore->iKorScore = (*iter)->iKorScore;
			pScore->iEngScore = (*iter)->iEngScore;
			pScore->iMatScore = (*iter)->iMatScore;
		pSave->m_ScoreListLine.push_back(pScore);
	}
	m_pUpdateList.push_front(pSave);
}

void   TLineGraphObserver::print()
{
	if (m_pUpdateList.empty()) return;
	TSubject* pSave = m_pUpdateList.front().get();
	if (pSave == nullptr) return;
	for (auto iter = pSave->m_ScoreListLine.begin();
		iter != pSave->m_ScoreListLine.end();
		iter++)
	{
		TScoreCard* pScore = iter->get();
		std::wcout << pScore->name << std::endl;
	}	
}
TLineGraphObserver::TLineGraphObserver(TSubject* pScores)
{
	pScores->observes.push_back(this);
	m_pScoreData = pScores;	
}