#pragma once
#include "TObserver.h"
class TSubject;
class TLineGraphObserver :   public TObserver
{
public:
	TSubject* pScoreData = nullptr;
	TSubject* pScoreListCopy = nullptr;
	virtual void update() override;
public:
	void   print();
public:
	TLineGraphObserver(TSubject* totalScore);
};

