#include "TFbxObject.h"
//struct tText
//{
//	int kk[100];
//};
//tText data[200];
//fwrite(data, sizeof(tText)*20, 1, fp);
//fread(data, sizeof(tText) * 20, 1, fp);

bool  TKgcFileFormat::Export(TKgcFileFormat* tFile, std::wstring szFileName)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"wb");
	if (err != 0) return false;

	TKgcFileHeader header;
	header.iFileNameSize = tFile->m_szFileName.size();
	header.iChildNodeCounter = tFile->m_ChildList.size();

	fwrite(&header, sizeof(TKgcFileHeader), 1, fp);
	fwrite(tFile->m_szFileName.c_str(), sizeof(wchar_t), header.iFileNameSize, fp);

	for (auto mesh : tFile->m_ChildList)
	{
		fwrite(&mesh->m_matWorld, sizeof(T::TMatrix), 1, fp);
		for (auto tex : mesh->m_szTexFileName)
		{
			int iLen = tex.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			fwrite(tex.c_str(), sizeof(wchar_t), iLen, fp);
		}
		if (mesh->m_vSubMeshVertexList.size() > 0)
		{
			for (auto sub : mesh->m_vSubMeshVertexList)
			{
				fwrite(&sub.at(0), sizeof(PNCT_Vertex), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIndexList)
			{
				fwrite(&sub.at(0), sizeof(DWORD), sub.size(), fp);
			}
		}
		else
		{
			int iLen = mesh->m_vVertexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vVertexList.at(0), sizeof(PNCT_Vertex), mesh->m_vVertexList.size(), fp);
			}

			iLen = mesh->m_vIndexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vIndexList.at(0), sizeof(DWORD), mesh->m_vIndexList.size(), fp);
			}
		}
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();
	return true;
}
bool  TKgcFileFormat::Import(std::wstring szFileName, 
	std::vector<TFbxModel*>& tFbxModel)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"rb");
	if (err != 0) return false;

	int iLen;
	TKgcFileHeader header;
	fread(&header, sizeof(TKgcFileHeader), 1, fp);

	WCHAR szLoadfilename[256] = { 0, };
	fread(szLoadfilename, sizeof(wchar_t), header.iFileNameSize, fp);

	for (int i = 0; i < header.iChildNodeCounter; i++)
	{
		/*fwrite(&mesh->m_matWorld, sizeof(T::TMatrix), 1, fp);
		for (auto tex : mesh->m_szTexFileName)
		{
			int iLen = tex.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			fwrite(tex.c_str(), sizeof(wchar_t), iLen, fp);
		}
		if (mesh->m_vSubMeshVertexList.size() > 0)
		{
			for (auto sub : mesh->m_vSubMeshVertexList)
			{
				fwrite(&sub.at(0), sizeof(PNCT_Vertex), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIndexList)
			{
				fwrite(&sub.at(0), sizeof(DWORD), sub.size(), fp);
			}
		}
		else
		{
			int iLen = mesh->m_vVertexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vVertexList.at(0), sizeof(PNCT_Vertex), mesh->m_vVertexList.size(), fp);
			}

			iLen = mesh->m_vIndexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vIndexList.at(0), sizeof(DWORD), mesh->m_vIndexList.size(), fp);
			}
		}*/
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();


	/*auto pGeomObject = kgcFormat;

	for (int iObj = 0; iObj < kgcFormat.size(); iObj++)
	{
		TFbxModel* pGeomModel = kgcFormat[iObj];

		std::wstring name = L"../../data/";
		if (m_pFbxfileList[iFbx][iObj]->m_szTexFileName.size() > 0)
		{
			std::wstring texPath = to_mw(m_pFbxfileList[iFbx][iObj]->m_szTexFileName[0]);
			wchar_t  szDrive[MAX_PATH] = { 0, };
			wchar_t  szDir[MAX_PATH] = { 0, };
			wchar_t  szFileName[MAX_PATH] = { 0, };
			wchar_t  szFileExt[MAX_PATH] = { 0, };
			_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
			name += szFileName;
			std::wstring ext = szFileExt;

			if (ext == L".tga")
			{
				name += L".dds";
			}
			else
			{
				name += szFileExt;
			}
		}

		pGeomModel->Create(name, L"vertexcolor.hlsl");
	}*/

	return true;
}
void	 TFbxModel::LoadTexture(std::wstring szPath) 
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
			std::wstring ext = szFileExt;

			if (ext == L".tga")
			{
				name += L".dds";
			}
			else
			{
				name += szFileExt;
			}

			m_pSubMeshTexture[iMesh] = I_Tex.Load(name).get();
			if (m_pSubMeshTexture[iMesh] != nullptr)
			{
				m_pSubMeshSRV[iMesh] = m_pSubMeshTexture[iMesh]->m_pSRV;
			}
		}
	}
}
bool     TFbxModel::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	if (m_vSubMeshVertexList.size() == 0)
	{
		TDxObject3D::CreateVertexBuffer(pd3dDevice);
		return true;
	}
	else
	{
		m_pSubMeshVertexBuffer.resize(m_vSubMeshVertexList.size());
		for (int iMesh = 0; iMesh < m_vSubMeshVertexList.size(); iMesh++)
		{			
			if (m_vSubMeshVertexList[iMesh].size() <= 0) continue;

			D3D11_BUFFER_DESC  bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
			bd.ByteWidth = sizeof(PNCT_Vertex) * m_vSubMeshVertexList[iMesh].size();			
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
bool     TFbxModel::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	if (m_vSubMeshIndexList.size() == 0)
	{
		TDxObject3D::CreateIndexBuffer(pd3dDevice);
		return true;
	}
	else
	{
		m_pSubMeshIndexBuffer.resize(m_vSubMeshIndexList.size());
		for (int iMesh = 0; iMesh < m_vSubMeshIndexList.size(); iMesh++)
		{
			if (m_vSubMeshIndexList[iMesh].size() <= 0) continue;
			D3D11_BUFFER_DESC  bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
			bd.ByteWidth = sizeof(DWORD) * m_vSubMeshIndexList[iMesh].size();			
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
			sd.pSysMem = &m_vSubMeshIndexList[iMesh].at(0);

			HRESULT hr = pd3dDevice->CreateBuffer(
				&bd,
				&sd,
				m_pSubMeshIndexBuffer[iMesh].GetAddressOf());
			if (FAILED(hr)) return false;
		}
	}
	return true;
}
void     TFbxModel::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	if (m_pSubMeshVertexBuffer.size())
	{
		for (int iSubMesh = 0; iSubMesh < m_pSubMeshVertexBuffer.size(); iSubMesh++)
		{
			UINT StartSlot = 0;
			UINT NumBuffers = 1;
			UINT pStrides = sizeof(PNCT_Vertex); // 1개의 정점 크기
			UINT pOffsets = 0; // 버퍼에 시작 인덱스
			pContext->IASetVertexBuffers(StartSlot, NumBuffers,
				m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);
			pContext->IASetIndexBuffer(m_pSubMeshIndexBuffer[iSubMesh].Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, m_pSubMeshSRV[iSubMesh].GetAddressOf());
			if (m_pSubMeshIndexBuffer[iSubMesh] != nullptr)
			{
				pContext->DrawIndexed(m_vSubMeshIndexList[iSubMesh].size(), 0, 0);
			}
			else
			{
				pContext->Draw(m_vSubMeshVertexList[iSubMesh].size(), 0);
			}
		}
	}
	else
	{
		PostRender(pContext);
	}

}
void     TFbxModel::Release() 
{
	TDxObject3D::Release();

	m_szTexFileName.clear();
	m_pSubMeshSRV.clear();
	m_pSubMeshTexture.clear();
	m_vSubMeshVertexList.clear();
	m_vSubMeshVertexList.clear();
	m_pSubMeshVertexBuffer.clear();
}
void     TFbxModel::SetVertexData() 
{
}
//void   TFbxModel::GenBufferVI(PNCT_Vertex& v)
//{
//	std::vector<PNCT_Vertex>    vertexArray;
//	std::vector<DWORD>			indexArray;
//	if (pGeomModel)
//	{
//		// 서브매터리얼 사용 유무
//		for (int iVertex = 0; iVertex < pGeomModel->m_vVertexList.size(); iVertex++)
//		{
//			PNCT_Vertex v1 = pGeomModel->m_vVertexList[iVertex];
//			// vertexArray에 모든 정점과 비교
//			int iVertexIndex = -1;
//			for (int iArray = 0; iArray < vertexArray.size(); iArray++)
//			{
//				PNCT_Vertex v2 = vertexArray[iArray];
//				if (v1 == v2)
//				{
//					iVertexIndex = iArray;
//					break;
//				}
//			}
//			if (iVertexIndex < 0)
//			{
//				vertexArray.emplace_back(v1);
//				iVertexIndex = vertexArray.size() - 1;
//			}
//			indexArray.emplace_back(iVertexIndex);
//		}
//
//		pGeomModel->m_vVertexList.clear();
//		pGeomModel->m_vIndexList = indexArray;
//		pGeomModel->m_vVertexList = vertexArray;
//	}
//}