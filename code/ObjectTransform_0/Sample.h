#pragma once
#include "TCore.h"
#include "THero.h"
#include "TNpc.h"

class Sample : public TCore
{
	TActor				objScreen; // »ï°¢Çü
	std::vector<TActor> m_UIList;
	std::vector<TNpc>	m_npcList;
	THero hero;	
	UINT  m_iNpcCounter=0;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pNumber;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};