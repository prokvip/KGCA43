#include "Sample.h"
#include "TTexMgr.h"
void   Sample::LoadHeightMapTexture(T_STR texName, TMap& map)
{
	ComPtr<ID3D11Resource> pResource2D = NULL;

	size_t maxsize=0;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(
		TDevice::m_pd3dDevice.Get(),
		texName.c_str(),
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		DirectX::DX11::WIC_LOADER_DEFAULT,
		pResource2D.GetAddressOf(),
		nullptr);

	ComPtr<ID3D11Texture2D> pTexture2D = NULL;
	if (FAILED(pResource2D->QueryInterface(__uuidof(ID3D11Texture2D),
		(void**)pTexture2D.GetAddressOf())))
	{

	}

	pTexture2D->GetDesc(&map.m_HeightMapDesc);

	map.m_HeightList.resize(map.m_HeightMapDesc.Height * map.m_HeightMapDesc.Width);

	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(TDevice::m_pContext->Map(pTexture2D.Get(),
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_READ, 0, &mapped)))
	{
		UCHAR* pTexels = (UCHAR*)mapped.pData;
		// RGBA
		for (int iRow = 0; iRow < map.m_HeightMapDesc.Height; iRow++)
		{
			// 한줄의 시작 인덱스
			UINT rowStart = iRow * mapped.RowPitch;
			for (int iCol = 0; iCol < map.m_HeightMapDesc.Width; iCol++)
			{
				// 한 픽셀 단위로 이동
				UINT colStart = iCol * 4;
				UINT r = pTexels[rowStart + colStart + 0];
				UINT g = pTexels[rowStart + colStart + 1];
				UINT b = pTexels[rowStart + colStart + 2];
				UINT a = pTexels[rowStart + colStart + 3];
				map.m_HeightList[iRow * map.m_HeightMapDesc.Width + iCol] = (float)r;
			}
		}
		TDevice::m_pContext->Unmap(pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1));
	}
}
void   Sample::Init()
{
	LoadHeightMapTexture(
		//L"../../data/map/heightmap/HEIGHT_CASTLE.bmp",
		L"../../data/map/heightmap/heightMap513.bmp",		
		m_Map);

	TMapDesc desc = { m_Map.m_HeightMapDesc.Width,
					m_Map.m_HeightMapDesc.Height, 1.0f, 0.5f,
		//L"../../data/map/heightmap/castle.jpg",
		L"../../data/map/001.jpg",
		L"../../data/shader/pnct.hlsl"};

	if (!m_Map.Load(desc))
	{
		return;
	}

	T::TVector3 eye = { 0.0f, 100.0f, -100.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);
}
void    Sample::Frame()
{
}
void    Sample::Render()
{
	m_Map.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	m_Map.Render(TDevice::m_pContext);
}
void    Sample::Release()
{

}

T_GAME_START(800, 600);
