#pragma once
#include "TCore.h"
#include "TMap.h"

#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment (lib,"windowscodecs.lib")
struct TLightInfo
{
	T::TVector4 m_vLightDir;	
	T::TVector4 m_vLightPos;
};
class Sample : public TCore
{	
	TMap		m_Map;
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