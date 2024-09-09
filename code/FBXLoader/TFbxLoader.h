#pragma once
#include <tstd.h>
#include <fbxsdk.h>
#include "TDxObj3D.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxModel : public TDxObject3D
{
public:
	std::vector<std::string> m_szTexFileName; 
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_pSubMeshSRV;
	std::vector<TTexture*>							m_pSubMeshTexture;

	using vList = std::vector<PNCT_Vertex>;
	std::vector<vList>					m_vSubMeshVertexList;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshVertexBuffer;
public:
	virtual void	 LoadTexture(std::wstring szPath) override
	{
		if (m_vSubMeshVertexList.size() == 0)
		{
			TDxObject3D::LoadTexture(szPath);
			return;
		}
		else
		{
			m_pSubMeshSRV.resize(m_vSubMeshVertexList.size());
			m_pSubMeshTexture.resize(m_vSubMeshVertexList.size());
			for (int iMesh = 0; iMesh < m_pSubMeshSRV.size(); iMesh++)
			{
				std::wstring name = L"../../data/";
				std::wstring texPath = to_mw(m_szTexFileName[iMesh]);
				wchar_t  szDrive[MAX_PATH] = { 0, };
				wchar_t  szDir[MAX_PATH] = { 0, };
				wchar_t  szFileName[MAX_PATH] = { 0, };
				wchar_t  szFileExt[MAX_PATH] = { 0, };
				_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
				name += szFileName;
				name += szFileExt;

				m_pSubMeshTexture[iMesh] = I_Tex.Load(name).get();
				if (m_pSubMeshTexture[iMesh] != nullptr)
				{
					m_pSubMeshSRV[iMesh] = m_pSubMeshTexture[iMesh]->m_pSRV;
				}
			}
		}
	}
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice)override
	{
		if (m_vSubMeshVertexList.size() == 0)
		{
			TDxObject3D::CreateVertexBuffer(pd3dDevice);
			return true;
		}else
		{
			m_pSubMeshVertexBuffer.resize(m_vSubMeshVertexList.size());
			for (int iMesh = 0; iMesh < m_vSubMeshVertexList.size(); iMesh++)
			{
				// 버퍼 할당 크기를 세팅한다.
				D3D11_BUFFER_DESC  bd;
				ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
				bd.ByteWidth = sizeof(PNCT_Vertex) * m_vSubMeshVertexList[iMesh].size();
				// 연결에 용도가 뭐냐? 
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


				// 할당된 버퍼에 
				// 시스템메모리가 ->GPU메모리로 체워지는 데이터
				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
				sd.pSysMem = &m_vSubMeshVertexList[iMesh].at(0);

				HRESULT hr = pd3dDevice->CreateBuffer(
					&bd,
					&sd,
					m_pSubMeshVertexBuffer[iMesh].GetAddressOf());
				if (FAILED(hr)) return false;
			}
		}
		return true;
	}
	virtual void     Render(ID3D11DeviceContext* pContext)override
	{
		PreRender(pContext);
		for (int iSubMesh = 0; iSubMesh < m_pSubMeshVertexBuffer.size(); iSubMesh++)
		{
			UINT StartSlot = 0;
			UINT NumBuffers = 1;
			UINT pStrides = sizeof(PNCT_Vertex); // 1개의 정점 크기
			UINT pOffsets = 0; // 버퍼에 시작 인덱스
			pContext->IASetVertexBuffers(StartSlot, NumBuffers, 
				m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);
			pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, m_pSubMeshSRV[iSubMesh].GetAddressOf());
			/*if (m_pIndexBuffer != nullptr)
			{
				pContext->DrawIndexed(m_vIndexList.size(), 0, 0);
			}
			else*/
			{
				// ONLY VB
				pContext->Draw(m_vSubMeshVertexList[iSubMesh].size(), 0);
			}
		}
		
	}
	virtual void     Release() override
	{
		TDxObject3D::Release();

		m_szTexFileName.clear();
		m_pSubMeshSRV.clear();
		m_pSubMeshTexture.clear();
		m_vSubMeshVertexList.clear();
		m_vSubMeshVertexList.clear();
		m_pSubMeshVertexBuffer.clear();
		/*for (int iVB = 0; iVB < m_pSubMeshVertexBuffer.size(); iVB++)
		{
			m_pSubMeshVertexBuffer[iVB]->Release();
		}*/
	}
	virtual void     SetVertexData() override
	{

	}
};

class TFbxLoader
{
public:
	static FbxManager*		m_pManager;
	FbxImporter*	m_pImporter = nullptr;
	FbxScene*		m_pScene = nullptr;
	FbxNode*		m_pRootNode = nullptr;

	std::vector<FbxMesh*>		m_pFbxMeshList;	
public:
	void   Init();
	bool   Load(C_STR filename, std::vector<TFbxModel*>& model);
	void   PreProcess(FbxNode* node);
	void   LoadMesh(int iMesh, std::vector<TFbxModel*>& m_pModelList);
	FbxVector2  GetUV(	FbxMesh* fbxMesh, 
						FbxLayerElementUV* uv,
						int iVertexPosIndex, 
						int iVertexUVIndex);
	FbxColor   GetColor(FbxMesh* fbxMesh,
						FbxLayerElementVertexColor* uv,
						int iVertexPosIndex,
						int iVertexColorIndex); 
	FbxVector4  GetNormal(FbxMesh* fbxMesh,
						FbxLayerElementNormal* VertexColorSet,
						int iVertexPosIndex,
						int iVertexColorIndex);
	T::TMatrix  ConvertFbxAMatrix(FbxAMatrix& m);
	int   GetSubMaterialPolygonIndex(int iPoly, 
									 FbxLayerElementMaterial* pMaterialaterial);
	void   Release();
	TFbxLoader() = default;
};


