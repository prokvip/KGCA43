#include "TSubject.h"
#include "TObserver.h"
void TSubject::AddScore(TScoreCard* score)
{
	m_ScoreList.push_back(score);
	for (auto observer : observes)
	{
		observer->update();
	}
}