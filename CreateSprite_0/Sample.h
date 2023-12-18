#pragma once
#include "TGameCore.h"
class Sample : public TGameCore
{	
public:
	ID3D11PixelShader* m_pPixelShaderAlphaTest = nullptr;
	std::vector<std::shared_ptr<TUiObj>>   m_uiList;
	std::shared_ptr<TUiObj>			m_bk;
	std::shared_ptr<TUiObj>			m_btnStart;
	std::shared_ptr<TUiObj>			m_Item;
	std::shared_ptr<TUiObj>			m_Number;	
	std::shared_ptr<TEffect>		m_EffectA;
	std::shared_ptr<TEffect>		m_EffectB;

	bool	CreatePixelShader();
public:
	bool    Init()		override;
	bool    Frame()		override;
	bool    Render()	override;
	bool    Release()	override;

};