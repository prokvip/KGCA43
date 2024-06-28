#pragma once
#include "TActor.h"

using CollisionFunction = std::function<void(TActor*, DWORD)>;
using SelectionFunction = std::function<void(TActor*, DWORD)>;

class TObjectMgr : public TSingleton<TObjectMgr>
{
	friend TSingleton<TObjectMgr>;
	int		m_iExecuteCollisionID = 0;
	int		m_iExecuteSelectID = 0;
public:
	std::map<int, TActor*>  m_ObjectList; // obj
	std::map<int, TActor*>  m_SelectList; // ui
	std::map<int, CollisionFunction>  m_fnCollisionExecute; // obj
	std::map<int, SelectionFunction>  m_fnSelectExecute; // ui
public:
	void AddCollisionExecute(TActor* pOwner, CollisionFunction func);
	void AddSelectExecute(TActor* pOwner, CollisionFunction func);
	void Release();
	void DelCollisionExecute(TActor* pOwner);
	void DelSelectExecute(TActor* pOwner);
	void Frame();
};
#define I_ObjMgr TObjectMgr::Get()
