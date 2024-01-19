#include "TSubject.h"
#include "TObserver.h"

void TSubject::AddScore(TScoreCard* score)
{	
	m_ScoreListBar.push_back(score);
	m_ScoreListLine.push_back(std::make_shared<TScoreCard>(*score));
	for (auto observer : observes)
	{
		observer->update();
	}
}