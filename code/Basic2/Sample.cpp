#include "Sample.h"

bool     TObj3D::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cb);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cb;

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		this->m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr)) return false;
	return true;
}
void   Sample::Init()
{
	//2>D:\00_43\code\Basic2\pnct.hlsl(45,2-26):error X3017: cannot convert from 'const float4' to 'struct PS_Out'
	//2 > FXC : error X3501 : 'main' : entrypoint not found
	//2 > compilation object save succeeded; see D : \00_43\output\pnct.cso
	this->m_obj.Create(L"../../data/1234.jpg", L"pnct.hlsl");
	
}

void    Sample::Frame()
{
	this->m_obj.Frame();
}
void    Sample::Render()
{
	this->m_obj.Render(TDevice::m_pContext);
}
void    Sample::Release()
{
	this->m_obj.Release();
}

T_GAME_START(800, 600);
