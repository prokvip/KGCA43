#pragma once
#include "TObserver.h"
class TSubject;
class TLineGraphObserver :   public TObserver
{
public:	
	using TSubjectNew = std::shared_ptr<TSubject>;
	TSubject* m_pScoreData = nullptr;
	std::list<TSubjectNew>  m_pUpdateList;
	virtual void update() override;
public:
	void   print();
public:
	TLineGraphObserver(TSubject* totalScore);
};

