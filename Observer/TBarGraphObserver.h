#pragma once
#include "TObserver.h"
class TSubject;
class TBarGraphObserver :   public TObserver
{
public:
	using TSubjectNew = std::shared_ptr<TSubject>;
	TSubjectNew  m_pScoreData = nullptr;
	TSubjectNew  m_pUpdateList = nullptr;
public:
	virtual void update() override;
public:
	void   print();
public:
	TBarGraphObserver(TSubject* totalScore);
};

