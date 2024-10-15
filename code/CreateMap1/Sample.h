#pragma once
#include "TCore.h"
#include "TMap.h"

class Sample : public TCore
{	
	TMap		m_Map;
	
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   LoadHeightMapTexture(T_STR texName, TMap& map);
};