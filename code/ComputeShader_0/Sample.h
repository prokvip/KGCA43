#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include "TCore.h"
const UINT NUM_ELEMENTS = 1024;
struct BufType
{
	int   i;
	float f;
};
class Sample : public TCore
{	
	BufType   m_Data0[NUM_ELEMENTS];
	BufType   m_Data1[NUM_ELEMENTS];
	ComPtr<ID3D11Buffer>	m_pBuf0;
	ComPtr<ID3D11Buffer>	m_pBuf1;
	ComPtr<ID3D11Buffer>	m_pBufResult;	
	ComPtr<ID3D11ShaderResourceView>  m_pBuf0SRV;
	ComPtr<ID3D11ShaderResourceView>  m_pBuf1SRV;
	ComPtr<ID3D11UnorderedAccessView>  m_pBufResultUAV;

	ComPtr<ID3D11ComputeShader>  m_pCS;
	bool  CreateStructureBuffer(UINT uElementSize, 
		UINT uCount, 
		VOID* pInitData, 
		ComPtr<ID3D11Buffer>& sBuffer);
	bool CreateBufferSRV(ComPtr<ID3D11Buffer>& pBuffer,
						 ComPtr<ID3D11ShaderResourceView>& srv);
	bool CreateBufferUAV(ComPtr<ID3D11Buffer>& pBuffer,
						 ComPtr<ID3D11UnorderedAccessView>& srv);
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer);
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	Sample() = default;
};