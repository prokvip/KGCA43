#pragma once
#include "TStd.h"
#include <tchar.h>
// 텍스처, 사운드, 쉐이더등의 "리소스 통합관리자"( 생성, 소멸 전담)

class TResource
{
public:
	virtual void Release() = 0;
	virtual bool Load(ID3D11Device* pd3dDevice, std::wstring filename) =0;
};

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
	void   ReleaseAll();

	std::wstring splitpath(std::wstring path, std::wstring filename);
public:		
	TBaseManager()
	{
	}
	virtual ~TBaseManager()
	{
		ReleaseAll();
	}
};
template <class T, class S>
T* TBaseManager<T,S>::Load(std::wstring fullpath)
{
	// 파일명+확장자 검출
	std::wstring name = splitpath(fullpath, L"");
	// 중복 검증
	T* pFindData = GetPtr(name);
	if (pFindData != nullptr)
	{
		return pFindData;
	}

	T* pData = new T;
	pData->m_csName = name;
	if (pData->Load(m_pd3dDevice, fullpath) == false)
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
template <class T, class S>
void TBaseManager<T, S>::ReleaseAll()
{
	for (auto pData : m_list)
	{
		T* data = (pData.second);
		data->Release();
		delete pData.second;
	}
	m_list.clear();
}
template <class T, class S>
std::wstring TBaseManager<T, S>::splitpath(
	std::wstring path, std::wstring entry)
{	
	wchar_t  szDrive[MAX_PATH] = { 0, };
	wchar_t  szDir[MAX_PATH] = { 0, };
	wchar_t  szFileName[MAX_PATH] = { 0, };
	wchar_t  szFileExt[MAX_PATH] = { 0, };
	//#include <tchar.h>
	_tsplitpath_s(path.c_str(), szDrive, szDir, szFileName, szFileExt);
	std::wstring name = szFileName;
	name +=szFileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}