#pragma once
#include "TCore.h"
#include "TFbxLoader.h"

class Sample : public TCore
{	
	TFbxLoader   m_fbxLoader;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   Reset() override;	
};