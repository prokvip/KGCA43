#pragma once
#include "TFiniteState.h"
class TFiniteStateMachine
{
public:
	std::wstring		m_csName;
	int			m_iStateIndex = 0;
	using T_FS = std::shared_ptr<TFiniteState>;
	std::map<int, T_FS > m_list;
public:
	void AddStateTransition(int InState, int event, int outState);
	int  GetTransition(int inState, int iEvent);

	TFiniteStateMachine() = default;
};

class TFiniteStateMachineManager
{
public:
	TFiniteStateMachineManager& Get()
	{
		static TFiniteStateMachineManager mgr;
		return *this;
	}
protected:
	std::map<std::wstring, TFiniteStateMachine* > m_list;
private:
	TFiniteStateMachineManager() {}
};
#define I_Fsm  TFiniteStateMachineManager::Get()