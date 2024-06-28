#include "TFiniteStateMachine.h"
//// GAME_START	-> EVENT_AUTO			-> GAME_TITLE
//// GAME_TITLE	-> EVENT_CLICK			-> GAME_INGAME
//// GAME_TITLE	-> EVENT_TIMER:3		-> GAME_INGAME
//// GAME_INGAME	-> EVENT_TIMER:10		-> GAME_TITLE
//TFiniteStateMachine::Get().AddStateTransition(GAME_START, EVENT_AUTO, GAME_TITLE);
//TFiniteStateMachine::Get().AddStateTransition(GAME_TITLE, EVENT_CLICK, GAME_INGAME);
//TFiniteStateMachine::Get().AddStateTransition(GAME_TITLE, EVENT_TIMER, GAME_INGAME);
//TFiniteStateMachine::Get().AddStateTransition(GAME_INGAME, EVENT_TIMER, GAME_TITLE);


void TFiniteStateMachine::AddStateTransition(
	int InState, int iEvent, int outState)
{
	T_FS pStateClass = nullptr;
	auto state = m_list.find(InState);
	if (state != m_list.end())
	{
		pStateClass = state->second;
	}
	else
	{
		pStateClass = std::make_shared<TFiniteState>(InState);
	}
	pStateClass->AdEventTransition(iEvent, outState);
	m_list.insert(std::make_pair(InState, pStateClass));
}
int  TFiniteStateMachine::GetTransition(int inState, int iEvent)
{
	auto outState = m_list.find(inState);
	if (outState != m_list.end())
	{
		T_FS pState = outState->second;
		return pState->GetTransition(iEvent);
	}
	return -1;
}