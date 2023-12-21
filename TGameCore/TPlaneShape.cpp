#include "TPlaneShape.h"
bool    TPlaneShape::LoadTexture(std::wstring texFileName) 
{
    m_ptTex = TTextureMgr::Get().Load(texFileName);
    if (m_ptTex != nullptr)
        return true;
    return false;
};
bool	TPlaneShape::LoadTexture(T_STR_VECTOR texArray)
{
    for (int i = 0; i < texArray.size(); i++)
    {
        m_pTexArray.push_back(TTextureMgr::Get().Load(texArray[i]));
    }
    m_ptTex = m_pTexArray[0];
    return false;
}
ID3D11Buffer* TPlaneShape::CreateBuffer(UINT ByteWidth, UINT BindFlags, void** pAddress)
{
    ID3D11Buffer* pBuffer = nullptr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = ByteWidth; //  메모리 할당 크기
    bd.Usage = D3D11_USAGE_DEFAULT;     // 버퍼의 저장소 및 접근권한 설정
    bd.BindFlags = BindFlags; //  버퍼의 용도


    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = pAddress;


    HRESULT hr = m_pd3dDevice->CreateBuffer(
        &bd,
        &sd,
        &pBuffer);
    if (FAILED(hr))
    {
        return pBuffer;
    }
    return pBuffer;
}
ID3DBlob* TPlaneShape::CreateShader(
    LPCWSTR pFileName,
    LPCSTR pEntrypoint,
    LPCSTR pTarget)
{
    ID3DBlob* pByteCode = nullptr;
    ID3DBlob* ppErrorMsgs = nullptr;
    HRESULT hr = D3DCompileFromFile(
        pFileName,
        nullptr,
        nullptr,
        pEntrypoint,
        pTarget,
        0,
        0,
        &pByteCode,
        &ppErrorMsgs);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, (char*)ppErrorMsgs->GetBufferPointer(), "Error", MB_OK);
        if (ppErrorMsgs) ppErrorMsgs->Release();
        return pByteCode;
    }
    if (ppErrorMsgs) ppErrorMsgs->Release();
    return pByteCode;
}
bool TPlaneShape::CreateVertexBuffer()
{
    UINT iSize = m_VertexList.size() * sizeof(TVertex);
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = iSize; //  메모리 할당 크기
    bd.Usage = D3D11_USAGE_DEFAULT;     // 버퍼의 저장소 및 접근권한 설정
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //  버퍼의 용도


    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_VertexList.at(0);

    HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool TPlaneShape::CreateIndexBuffer()
{
    UINT iSize = m_IndexList.size() * sizeof(DWORD);
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = iSize; //  메모리 할당 크기
    bd.Usage = D3D11_USAGE_DEFAULT;     // 버퍼의 저장소 및 접근권한 설정
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER; //  버퍼의 용도


    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_IndexList.at(0);

    HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool TPlaneShape::CreateVertexShader()
{
    ID3DBlob* ppErrorMsgs = nullptr;
    HRESULT hr = D3DCompileFromFile(
        L"../../data/shader/Shader.txt",
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &m_pVertexShaderByteCode,
        &ppErrorMsgs);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, (char*)ppErrorMsgs->GetBufferPointer(), "Error", MB_OK);
        if (ppErrorMsgs) ppErrorMsgs->Release();
        return false;
    }
    if (ppErrorMsgs) ppErrorMsgs->Release();

    hr = m_pd3dDevice->CreateVertexShader(
        m_pVertexShaderByteCode->GetBufferPointer(),
        m_pVertexShaderByteCode->GetBufferSize(),
        nullptr,
        &m_pVertexShader);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
bool TPlaneShape::CreatePixelShader()
{
    ID3DBlob* ppErrorMsgs = nullptr;
    HRESULT hr = D3DCompileFromFile(
        L"../../data/shader/Shader.txt",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &m_pPixelShaderByteCode,
        &ppErrorMsgs);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, (char*)ppErrorMsgs->GetBufferPointer(), "Error", MB_OK);
        if (ppErrorMsgs) ppErrorMsgs->Release();
        return false;
    }
    if (ppErrorMsgs) ppErrorMsgs->Release();

    //m_pd3dDevice->CreatePixelShader();

    hr = m_pd3dDevice->CreatePixelShader(
        m_pPixelShaderByteCode->GetBufferPointer(),
        m_pPixelShaderByteCode->GetBufferSize(),
        nullptr,
        &m_pPixelShader);

    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool TPlaneShape::CreateInputLayout()
{
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //D3D11_INPUT_ELEMENT_DESC pInputElementDescs;
    //pInputElementDescs.SemanticName = "POSITION";
    //pInputElementDescs.SemanticIndex = 0;
    //pInputElementDescs.Format= DXGI_FORMAT_R32G32B32_FLOAT;
    //pInputElementDescs.InputSlot=0;
    //pInputElementDescs.AlignedByteOffset=0;
    //pInputElementDescs.InputSlotClass= D3D11_INPUT_PER_VERTEX_DATA;
    //pInputElementDescs.InstanceDataStepRate = 0;

    UINT iNumLayout = sizeof(layout) / sizeof(layout[0]);
    HRESULT hr = m_pd3dDevice->CreateInputLayout(
        layout,
        iNumLayout,
        m_pVertexShaderByteCode->GetBufferPointer(),
        m_pVertexShaderByteCode->GetBufferSize(),
        &m_pVertexlayout);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
bool    TPlaneShape::Init()
{
    return true;
}
bool    TPlaneShape::Create(W_STR name, T_STR_VECTOR texFileName)
{
    m_csName = name;
    //// v0       v1
    ////
    //// v3       v2
    //m_VertexList.emplace_back(TVector3(0.0f, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    //m_VertexList.emplace_back(TVector3(m_rtClient.right, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    //m_VertexList.emplace_back(TVector3(m_rtClient.right, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    //m_VertexList.emplace_back(TVector3(0.0f, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3
    m_IndexList.push_back(0);
    m_IndexList.push_back(1);
    m_IndexList.push_back(2);

    m_IndexList.push_back(2);
    m_IndexList.push_back(3);
    m_IndexList.push_back(0);

    if (!CreateVertexBuffer() || !CreateIndexBuffer())
    {
        return false;
    }
    if (!CreateVertexShader() || !CreatePixelShader())
    {
        return false;
    }

    if (!CreateInputLayout())
    {
        return false;
    }
    if (!LoadTexture(texFileName))
    {
        return false;
    }

    /*  UINT iSize = m_VertexList.size() * sizeof(TVertex);
   m_pVertexBuffer = CreateBuffer(iSize, D3D11_BIND_VERTEX_BUFFER,(void**)&m_VertexList.at(0));
   iSize = m_IndexList.size() * sizeof(DWORD);
   m_pIndexBuffer = CreateBuffer(iSize, D3D11_BIND_INDEX_BUFFER, (void**)&m_IndexList.at(0));

   m_pVertexShaderByteCode = CreateShader(L"Shader.txt", "VS", "vs_5_0");
   m_pPixelShaderByteCode = CreateShader(L"Shader.txt", "PS", "ps_5_0");
   if (m_pVertexShaderByteCode == nullptr || m_pVertexShaderByteCode == nullptr)
   {
       return false;
   }*/
    return true;
}
bool    TPlaneShape::Create(W_STR name, W_STR texFileName)
{
    m_csName = name;
    if (!texFileName.empty())
    {
        if (!LoadTexture(texFileName))
        {
            return false;
        }
    }

    //// v0       v1
    ////
    //// v3       v2
    //m_VertexList.emplace_back(TVector3(0.0f, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(0.0f, 0.0f));      // 0
    //m_VertexList.emplace_back(TVector3(m_rtClient.right, 0.0f, 0.5f), TVector4(0, 1, 0, 1), TVector2(1.0f, 0.0f));    // 1
    //m_VertexList.emplace_back(TVector3(m_rtClient.right, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(1.0f, 1.0f));  // 2
    //m_VertexList.emplace_back(TVector3(0.0f, m_rtClient.bottom, 0.5f), TVector4(0, 0, 1, 1), TVector2(0.0f, 1.0f));    // 3
    m_IndexList.push_back(0);
    m_IndexList.push_back(1);
    m_IndexList.push_back(2);

    m_IndexList.push_back(2);
    m_IndexList.push_back(3);
    m_IndexList.push_back(0);

    if (!CreateVertexBuffer() || !CreateIndexBuffer())
    {
        return false;
    }
    if (!CreateVertexShader() || !CreatePixelShader())
    {
        return false;
    }

    if (!CreateInputLayout())
    {
        return false;
    }
   

    /*  UINT iSize = m_VertexList.size() * sizeof(TVertex);
   m_pVertexBuffer = CreateBuffer(iSize, D3D11_BIND_VERTEX_BUFFER,(void**)&m_VertexList.at(0));
   iSize = m_IndexList.size() * sizeof(DWORD);
   m_pIndexBuffer = CreateBuffer(iSize, D3D11_BIND_INDEX_BUFFER, (void**)&m_IndexList.at(0));

   m_pVertexShaderByteCode = CreateShader(L"../../data/shader/Shader.txt", "VS", "vs_5_0");
   m_pPixelShaderByteCode = CreateShader(L"../../data/shader/Shader.txt", "PS", "ps_5_0");
   if (m_pVertexShaderByteCode == nullptr || m_pVertexShaderByteCode == nullptr)
   {
       return false;
   }*/
    return true;
}
bool    TPlaneShape::PreRender()
{
    m_pd3dContext->PSSetShaderResources(0, 1, &m_ptTex->m_pTextureSRV);
    m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
    m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
    m_pd3dContext->IASetInputLayout(m_pVertexlayout);
    m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT pStrides = sizeof(TVertex);
    UINT pOffsets = 0;
    m_pd3dContext->IASetVertexBuffers(
        0,
        1,
        &m_pVertexBuffer,
        &pStrides,
        &pOffsets);
    m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}
bool    TPlaneShape::PostRender()
{
    m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
    return true;
}
bool    TPlaneShape::Frame()
{
    return true;
}
bool    TPlaneShape::Render()
{
    PreRender();
    PostRender();
    return true;
}
bool    TPlaneShape::Release()
{
    if (m_pVertexlayout)m_pVertexlayout->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pIndexBuffer) m_pIndexBuffer->Release();
    if (m_pPixelShaderByteCode) m_pPixelShaderByteCode->Release();
    if (m_pVertexShaderByteCode) m_pVertexShaderByteCode->Release();
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_pVertexShader) m_pVertexShader->Release();
    return true;
}
