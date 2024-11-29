#include "Sample.h"
ID3D11Buffer* Sample::CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer)
{
	ID3D11Buffer* debugbuf = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	if (SUCCEEDED(TDevice::m_pd3dDevice->CreateBuffer(&desc, NULL, &debugbuf)))
	{
		TDevice::m_pContext->CopyResource(debugbuf, pBuffer);
	}

	return debugbuf;
}

bool  Sample::CreateStructureBuffer(
	UINT uElementSize,
	UINT uCount,
	VOID* pInitData,
	ComPtr<ID3D11Buffer>& sBuffer)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = 
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = uElementSize * uCount;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = uElementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return TDevice::m_pd3dDevice->CreateBuffer(&desc, 
			&InitData, sBuffer.GetAddressOf());
	}
	return TDevice::m_pd3dDevice->CreateBuffer(&desc, NULL, 
			sBuffer.GetAddressOf());
}
bool Sample::CreateBufferSRV(ComPtr<ID3D11Buffer>& pBuffer,
	ComPtr<ID3D11ShaderResourceView>& srv)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	//if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	//{
	//	// This is a Raw Buffer

	//	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	//	desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	//	desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	//}
	//else
		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.BufferEx.NumElements = 
				descBuf.ByteWidth / descBuf.StructureByteStride;
		}
		else
		{
			return E_INVALIDARG;
		}

	return TDevice::m_pd3dDevice->CreateShaderResourceView(
										pBuffer.Get(),
										&desc, 
										srv.GetAddressOf());
}
bool Sample::CreateBufferUAV(ComPtr<ID3D11Buffer>& pBuffer,
	ComPtr<ID3D11UnorderedAccessView>& srv)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	//if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	//{
	//	// This is a Raw Buffer

	//	desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS, when creating Raw Unordered Access View
	//	desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	//	desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	//}
	//else
		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			// This is a Structured Buffer

			desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
			desc.Buffer.NumElements = 
				descBuf.ByteWidth / descBuf.StructureByteStride;
		}
		else
		{
			return E_INVALIDARG;
		}

	return TDevice::m_pd3dDevice->CreateUnorderedAccessView(
		pBuffer.Get(), &desc, srv.GetAddressOf());
}
void   Sample::Init()
{	
	m_pCS.Attach(
		DX::LoadComputeShaderFile(
		TDevice::m_pd3dDevice.Get(),
			L"CS.txt",nullptr, "CSMain"));
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		m_Data0[i].i = i;
		m_Data0[i].f = (float)i;
		m_Data1[i].i = i;
		m_Data1[i].f = (float)i;
	}
	CreateStructureBuffer(sizeof(BufType),NUM_ELEMENTS, &m_Data0, m_pBuf0 );
	CreateStructureBuffer(sizeof(BufType),NUM_ELEMENTS, &m_Data1, m_pBuf1);
	CreateStructureBuffer(sizeof(BufType), NUM_ELEMENTS, nullptr, m_pBufResult);

	CreateBufferSRV(m_pBuf0, m_pBuf0SRV);
	CreateBufferSRV(m_pBuf1, m_pBuf1SRV);
	CreateBufferUAV(m_pBufResult, m_pBufResultUAV);

	ID3D11ShaderResourceView* SRVArray[] = { 
		m_pBuf0SRV.Get() ,m_pBuf1SRV.Get() };

	TDevice::m_pContext->CSSetShader(m_pCS.Get(), NULL, 0);
	TDevice::m_pContext->CSSetShaderResources(0, 2, SRVArray);
	TDevice::m_pContext->CSSetUnorderedAccessViews(0, 1, m_pBufResultUAV.GetAddressOf(), NULL);

	// groupid= 1024*1*1
	// groupthreadid = [0~1023][1][1]
	// numthreads(1,1,1) -> thread = 0~1023;
	// SV_DispatchThreadID = 0~1024 -> CSMain()
	TDevice::m_pContext->Dispatch(NUM_ELEMENTS,1,1);


	ComPtr<ID3D11Buffer> ret;
	ret.Attach(CreateAndCopyToDebugBuf(m_pBufResult.Get()));
	
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	BufType* p;
	TDevice::m_pContext->Map(ret.Get(), 0, D3D11_MAP_READ, 0, &MappedResource);
	p = (BufType*)MappedResource.pData;
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		int		iGPUValue = p[i].i;
		float	fGPUValue = p[i].f;

		int iCPU_Ret = m_Data0[i].i + m_Data1[i].i;
		int fCPU_Ret = m_Data0[i].f + m_Data1[i].f;

		if (iGPUValue != iCPU_Ret || fGPUValue != fCPU_Ret)
		{
			break;
		}
	}

	TDevice::m_pContext->Unmap(ret.Get(),0);


	T::TVector3 eye = { 0.0f, 0.0f, -300.0f };
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
	
}
void    Sample::Release()
{	
}

T_GAME_START(800, 600);
