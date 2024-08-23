#include "TShaderMgr.h"

void TShader::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}
	if (VS_Bytecode)
	{
		VS_Bytecode->Release();
		VS_Bytecode = nullptr;
	}
	if (PS_Bytecode)
	{
		PS_Bytecode->Release();
		PS_Bytecode = nullptr;
	}
}
bool TShader::Load(std::wstring filename)
{
	m_csName = filename;
	HRESULT hr;
	ID3DBlob* errormsg = nullptr; // ������ �� ���� �޼���
	hr = D3DCompileFromFile(
		filename.c_str(),
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0", // dx11 �������̴� �����Ϸ�
		0,
		0,
		&VS_Bytecode,
		&errormsg
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)errormsg->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}

	// ������Ʈ ������ �����ּ�
	const void* pShaderBytecode = VS_Bytecode->GetBufferPointer();
	// ������Ʈ ������ ũ��
	SIZE_T BytecodeLength = VS_Bytecode->GetBufferSize();

	hr = m_pd3dDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, nullptr, &m_pVertexShader);

	if (FAILED(hr))
	{
		return false;
	}
	if (errormsg) errormsg->Release();

	/// �Ƚ����̴� �ε� �� ����	
	hr = D3DCompileFromFile(
		filename.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0", // dx11 �������̴� �����Ϸ�
		0,
		0,
		&PS_Bytecode,
		&errormsg
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)errormsg->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}

	pShaderBytecode = PS_Bytecode->GetBufferPointer();
	BytecodeLength = PS_Bytecode->GetBufferSize();
	hr = m_pd3dDevice->CreatePixelShader(PS_Bytecode->GetBufferPointer(),
		PS_Bytecode->GetBufferSize(), nullptr, &m_pPixelShader);
	if (FAILED(hr)) return false;
	return true;
};