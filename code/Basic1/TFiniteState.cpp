#include "TFiniteState.h"
void TFiniteState::AdEventTransition(int event, int outState)
{
	m_Event.insert(std::make_pair(event, outState));
}
int  TFiniteState::GetTransition(int iEvent)
{
	auto outState = m_Event.find(iEvent);
	if (outState != m_Event.end())
	{
		return outState->second;
	}
	return -1;
}