#pragma once
#include "TStd.h"
// 텍스처, 사운드, 쉐이더등의 "리소스 통합관리자"( 생성, 소멸 전담)
//template<class T, class S>
template<class T>
class TSingleton
{
public:
	static T& Get()
	{
		static T mgr;
		return mgr;
	}
};
template <class T, class S>
class TBaseManager : public TSingleton<S>
{
public:
	std::wstring m_csName = L"none";
public:
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	void Set(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext)
	{
		m_pd3dDevice = pd3dDevice;
		m_pContext = pContext;
	}

	std::map<std::wstring, T*>  m_list;
	virtual T* Load(std::wstring filename);
	T* GetPtr(std::wstring key);
public:		
	TBaseManager()
	{
	}
};
template <class T, class S>
T* TBaseManager<T,S>::Load(std::wstring filename)
{
	// 중복 검증
	T* pFindData = GetPtr(filename);
	if (pFindData != nullptr)
	{
		return pFindData;
	}

	T* pData = new T;
	if (pData->Load(m_pd3dDevice, filename) == false)
	{
		delete pData;
		return nullptr;
	}
	m_list.insert(std::make_pair(pData->m_csName, pData));
	return pData;
}
template <class T, class S>
T* TBaseManager<T,S>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
