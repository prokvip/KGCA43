#pragma once
#include <map>
#include <memory>
#include <string>
class TFiniteState
{	
public:
	int    m_StateID;
	//      event, state
	std::map<int, int > m_Event;
	void AdEventTransition(int event, int outState);
	int  GetTransition(int iEvent);
	TFiniteState(int id) :m_StateID(id) {}
};
