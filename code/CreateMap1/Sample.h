#pragma once
#include "TCore.h"
#include "TMap.h"

#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment (lib,"windowscodecs.lib")

class Sample : public TCore
{	
	TMap		m_Map;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   LoadHeightMapTexture(T_STR texName, TMap& map);
	void   LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave);
	void   SaveFile(T_STR name, ID3D11Texture2D* pRes);
};