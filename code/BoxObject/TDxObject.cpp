#include "TDxObject.h"
/// <summary>
/// 
/// </summary>
void   TDxObject::UpdateVertexBuffer()
{
}
void   TDxObject::Frame()
{	
}
void   TDxObject::SetVertexData(RECT rt)
{
}
bool   TDxObject::Create(
	RECT rt, std::wstring texName, std::wstring hlsl) 
{	
	return true;
}
bool   TDxObject::Create(
	std::wstring texName, std::wstring hlsl)
{
	return true;
}
bool     TDxObject::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{	
	return true;
}
bool     TDxObject::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	return true;
}
bool   TDxObject::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	return true;
}
bool     TDxObject::LoadShader(std::wstring filename)
{
	m_pShader = I_Shader.Load(filename).get();
	if (m_pShader==nullptr) return false;
	return true;
}
// GPU 가 처리하는 순서
// 1) 지정된 정점버퍼에 접근하여 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1개의 정점 단위로 로드한다.UINT pStrides = sizeof(P2C4T2_Vertex); // 1개의 정점 크기
// 3) 지정된 인풋레이아웃에 따라서 정점 성분을 정점쉐이더에 전달한다
// 정점쉐이더는 정점단위로 호출된다.
// |x, y, r,g,b,a|,|x, y, r,g,b,a| ,|x, y, r,g,b,a| ,|x, y, r,g,b,a| 
//   0~7  8~11
// |x, y, r,g,b,a, u,v|,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| 
//   0~7  8~11   24~32
bool     TDxObject::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	return true;
}
void     TDxObject::PreRender(ID3D11DeviceContext* pContext)
{	
}
void     TDxObject::PostRender(ID3D11DeviceContext* pContext)
{		
}
void     TDxObject::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);	
}
void TDxObject::Release()
{
}



