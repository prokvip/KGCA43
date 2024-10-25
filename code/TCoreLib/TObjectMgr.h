#pragma once
#include "TBaseManager.h"
#include "TFbxObject.h"
class TObject : public TResource
{	
	ID3D11Device* m_pd3dDevice = nullptr;
	std::shared_ptr<TDxObject>	  m_pdxObj = nullptr;
public:
	std::shared_ptr<TDxObject> Get() { return m_pdxObj; }
	TDxObject* GetPtr() { return m_pdxObj.get(); }
	virtual void Set(ID3D11Device* pDevice) override
	{
		m_pd3dDevice = pDevice;
	};
public:
	std::wstring m_csName;
public:
	void Release() override;
	bool Load(std::wstring filename)override;
};

class TObjectComposed : public TObject
{
public:
	std::list<TResource*> m_Components;
	void Add(TResource* pObj)
	{
		m_Components.emplace_back(pObj);
	}
};

class TObjectMgr : public TBaseManager<TObject, TObjectMgr>
{
	friend class TSingleton<TObjectMgr>;
private:
	TObjectMgr() {}
};

#define I_Object TObjectMgr::Get()

