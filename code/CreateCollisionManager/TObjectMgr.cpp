#include "TObjectMgr.h"
#include "TInput.h"
void TObjectMgr::AddCollisionExecute(TActor* pOwner, CollisionFunction func)
{
}
void TObjectMgr::AddSelectExecute(TActor* pOwner, CollisionFunction func)
{
	pOwner->m_iSelectID = m_iExecuteSelectID++;

	m_SelectList.insert(std::make_pair(pOwner->m_iSelectID, pOwner));
	m_fnSelectExecute.insert(std::make_pair(pOwner->m_iSelectID, func));
}
void TObjectMgr::Frame()
{
	for (auto obj : m_SelectList)
	{
		TActor* pActor = (TActor*)obj.second;
		DWORD dwState = TSelectState::T_DEFAULT;
		if (TCollision::RectToPt(pActor->m_rt, I_Input.m_ptMousePos))
		{
			DWORD dwKeyState = I_Input.m_dwMouseState[0];
			dwState = TSelectState::T_HOVER;
			if (dwKeyState == KEY_PUSH)
			{				
				dwState = TSelectState::T_ACTIVE;
			}
			if (dwKeyState == KEY_HOLD)
			{
				dwState = TSelectState::T_FOCUS;
			}
			if (dwKeyState == KEY_UP)
			{
				dwState = TSelectState::T_SELECTED;
			}
			auto iter = m_fnSelectExecute.find(pActor->m_iSelectID);
			if (iter != m_fnSelectExecute.end())
			{
				SelectionFunction call = iter->second;
				call(pActor, dwState);
			}
		}
	}
	for (auto src : m_ObjectList)
	{
		TActor* pActorSrc = (TActor*)src.second;
		for (auto dest : m_ObjectList)
		{
			TActor* pActorDest = (TActor*)dest.second;
			if (TCollision::RectToRect(pActorSrc->m_rt, pActorDest->m_rt))
			{
				auto iter = m_fnCollisionExecute.find(pActorSrc->m_iSelectID);
				if (iter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = iter->second;
					call(pActorDest, 0);
				}
			}
		}
	}
}