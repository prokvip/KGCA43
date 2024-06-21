#pragma once
#include "TBaseManager.h"
#include <typeinfo>
//
#define DECLARE_CLASSNAME				static std::wstring lpszClassName;
#define IMPLEMENT_CLASSNAME(s)			char s##::lpszClassName = (L#s);

#define DECLARE_DYNCREATE				static TResource* CreateObject();
#define IMPLEMENT_DYNCREATE(s)			TResource* s::CreateObject()	{return new s;};

#define DECLARE_DYNAMIC(s)				static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s)			CRuntimeClass s::class##s={ L#s, sizeof(s), s::CreateObject };
//#define IMPLEMENT_DYNAMIC(s)			CRuntimeClass s::class##s={ s::CreateObject };

#define DECLARE_DYNCLASS_CREATE			CRuntimeClass* GetRuntimeClass() const;
#define IMPLEMENT_DYNCLASS_CREATE(s)	CRuntimeClass* s::GetRuntimeClass() const{return &class##s;};
//
#define RUNTIME_CLASS(s)				&s::class##s
#define RUNTIME_DECLARE(s)		DECLARE_DYNAMIC(s);		DECLARE_DYNCREATE;		DECLARE_DYNCLASS_CREATE;
#define RUNTIME_IMPLEMENT(s)	IMPLEMENT_DYNCREATE(s);	IMPLEMENT_DYNAMIC(s);	IMPLEMENT_DYNCLASS_CREATE(s);
//
struct CRuntimeClass
{
	std::wstring	m_lpszClassName;
	UINT			m_iObjectSize;
	TResource* (*pfnCreateObject)();
	TResource* CreateObject()
	{
		return (*pfnCreateObject)();
	}
};

template<class T>
class TAssetManager : public TSingleton<TAssetManager<T>>
{
	friend class TSingleton<TAssetManager>;
public:
	std::wstring m_csName = L"none";
public:
	std::map<std::wstring, std::shared_ptr<T>>  m_list;
	virtual std::shared_ptr<T>  Load(std::wstring filename, CRuntimeClass* pClass);
	virtual std::shared_ptr<T>  CreateObject(std::wstring path, std::wstring name, TResource* pResource);
	virtual std::shared_ptr<T>	GetPtr(std::wstring key);	
	virtual std::wstring		splitpath(std::wstring path, std::wstring filename);
	virtual void				Release();
private:
	TAssetManager() = default;
public:
	virtual ~TAssetManager() = default;
};

template<class T>
std::shared_ptr<T> TAssetManager<T>::Load(std::wstring fullpath, CRuntimeClass* pClass)
{
	// 파일명+확장자 검출
	std::wstring name = splitpath(fullpath, L"");
	// 중복 검증
	std::shared_ptr<T> pFindData = GetPtr(name);
	if (pFindData != nullptr)
	{
		return pFindData;
	}
	if (pClass == nullptr) return nullptr;

	return CreateObject(fullpath, name, pClass->CreateObject());
}
template <class T>
std::shared_ptr<T>  TAssetManager<T>::CreateObject(
	std::wstring path, std::wstring name, TResource* pResource)
{
	//std::shared_ptr<T>  pData = std::make_shared<T>();
	std::shared_ptr<T>  pData(pResource);
	pData->Init(name);
	if (pData->Load( path) == false)
	{
		return nullptr;
	}
	m_list.insert(std::make_pair(pData->m_csName, pData));
	return pData;
}
template<class T>
std::shared_ptr<T> TAssetManager<T>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
template<class T>
void TAssetManager<T>::Release()
{
	for (auto pData : m_list)
	{
		pData.second->Release();
	}
	
	m_list.clear();
}
template<class T>
std::wstring TAssetManager<T>::splitpath(std::wstring path, std::wstring entry)
{
	wchar_t  szDrive[MAX_PATH] = { 0, };
	wchar_t  szDir[MAX_PATH] = { 0, };
	wchar_t  szFileName[MAX_PATH] = { 0, };
	wchar_t  szFileExt[MAX_PATH] = { 0, };
	//#include <tchar.h>
	_tsplitpath_s(path.c_str(), szDrive, szDir, szFileName, szFileExt);
	std::wstring name = szFileName;
	name += szFileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}


