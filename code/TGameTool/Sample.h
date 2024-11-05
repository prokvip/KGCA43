#pragma once
#include "TCore.h"
#include "TQuadtree.h"
#include "TFbxLoader.h"
#include "TObjectMgr.h"
#include "TSelect.h"

struct TLightInfo
{
	//T::TVector4 m_vLightInfo; // x = num, y = 
	T::TVector4 m_vLightDir;	
	T::TVector4 m_vLightPos;
};
struct TMapObject
{
	int iObjectType = 0;
	TObject* pObject = nullptr;
	T::TVector3 vPos;
	T::TVector3 vScale;
	T::TVector3 vRotate;
	T::TMatrix  matWorld;
};
class Sample : public TCore
{	
public:
	TSelect		m_Select;
	TMap		m_Map;
	TQuadtree   m_Quadtree;
	using tModel = std::shared_ptr<TObject>;
	std::vector<tModel>			m_pFbxfileList;
	std::vector<TMapObject>		m_pMapObjectList;
	std::vector<std::wstring>   m_LoadFiles;
public:
	TLightInfo  m_LightInfo;
	ComPtr<ID3D11Buffer>  m_pConstantBufferLight;	
public:
	void   Init() override;
	void   PreFrame() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave);
	void   SetObject(T::TVector3 vPos);
};