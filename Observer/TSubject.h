#pragma once
#include <list>
#include "TScoreCard.h"
#include "TObserver.h"
class TObserver;
class TSubject
{
public:
	std::list<TScoreCard*>  m_ScoreList;
	std::list<TObserver*>  observes;

public:
	void AddScore(TScoreCard* score);
};

