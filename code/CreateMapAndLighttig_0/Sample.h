#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TFbxLoader.h"
#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment (lib,"windowscodecs.lib")
struct TLightInfo
{
	T::TVector4 m_vLightDir;	
	T::TVector4 m_vLightPos;
};
struct TMapObject
{
	int iObjectType = 0;
	T::TVector3 vPos;
	T::TVector3 vScale;
	T::TVector3 vRotate;
	T::TMatrix  matWorld;
};
class Sample : public TCore
{	
	TMap		m_Map;

	TFbxLoader					m_fbxLoader;
	using tModel = std::shared_ptr<TFbxModel>;
	std::vector<tModel>			m_pFbxfileList;
	std::vector<TMapObject>		m_pMapObjectList;
	std::vector<std::wstring>   m_LoadFiles;
public:
	TLightInfo  m_LightInfo;
	ComPtr<ID3D11Buffer>  m_pConstantBufferLight;
	ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pd3dDevice, 
		void* data, UINT iNumIndex, UINT iSize, 
		bool bDynamic = false);
	bool	 CreateConstantBuffer(ID3D11Device* pd3dDevice);
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   LoadHeightMapTexture(T_STR texName, TMap& map);
	void   LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave);
	void   SaveFile(T_STR name, ID3D11Texture2D* pRes);
};