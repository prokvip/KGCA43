#pragma once
#include "TObserver.h"
class TSubject;
class TBarGraphObserver :   public TObserver
{
public:
	TSubject* pScoreData = nullptr;
	TSubject* pScoreListCopy = nullptr;
public:
	virtual void update() override;
public:
	void   print();
public:
	TBarGraphObserver(TSubject* totalScore);
};

