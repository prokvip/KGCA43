#include "TKgcFormat.h"
#include "TFbxObject.h"
bool  TKgcFileFormat::Export(TKgcFileFormat* tFile, std::wstring szFileName)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"wb");
	if (err != 0) return false;

	TKgcFileHeader fileHeader = tFile->m_Header;
	fileHeader.iLength = tFile->m_szFileName.size(); // 파일명 크기
	fileHeader.iChildNodeCounter = tFile->m_ChildList.size();
	fileHeader.iVersion = 100;
	fileHeader.matWorld = tFile->m_matWorld;
	fileHeader.iNumTrack = tFile->m_pAnimationMatrix.size();
	fileHeader.iStartFrame = fileHeader.iStartFrame;
	fileHeader.iLastFrame = fileHeader.iLastFrame;
	fileHeader.iFrameSpeed = fileHeader.iFrameSpeed;

	fwrite(&fileHeader, sizeof(TKgcFileHeader), 1, fp);
	fwrite(tFile->m_szFileName.c_str(), sizeof(wchar_t), fileHeader.iLength, fp);

	// TNode 
	fwrite(&tFile->m_ptNodeList.at(0), sizeof(TFbxNode),
		fileHeader.iNumNodeCounter, fp);

	// bone matrix
	// 71
	if (fileHeader.iLastFrame > 0)
	{
		for (int iBone = 0; iBone < tFile->m_pBoneAnimMatrix.size(); iBone++)
		{
			fwrite(&tFile->m_pBoneAnimMatrix[iBone].at(0),
				sizeof(T::TMatrix), fileHeader.iLastFrame, fp);
		}
	}


	for (auto mesh : tFile->m_ChildList)
	{
		TKgcFileHeader header;

		header.iLength = mesh->m_szTexFileList.size(); // 텍스처 개수
		header.iChildNodeCounter = mesh->m_ChildList.size();
		header.matWorld = mesh->m_matWorld;
		header.isSubMesh = (mesh->m_vSubMeshVertexList.size() > 0) ? 1 : 0;
		header.iSubVertexBufferCounter = mesh->m_vSubMeshVertexList.size();
		header.iSubIndexBufferCounter = mesh->m_vSubMeshIndexList.size();
		header.iSubIWVertexBufferCounter = mesh->m_vSubMeshIWVertexList.size();
		header.iNumTrack = mesh->m_pAnimationMatrix.size();
		header.iStartFrame = fileHeader.iStartFrame;
		header.iLastFrame = fileHeader.iLastFrame;
		header.iFrameSpeed = fileHeader.iFrameSpeed;
		fwrite(&header, sizeof(TKgcFileHeader), 1, fp);

		// Bindpose matrix
		int iSize = mesh->m_matBindPose.size();
		fwrite(&iSize, sizeof(int), 1, fp);
		if (iSize <= 0) continue;
		fwrite(&mesh->m_matBindPose.at(0),
			sizeof(T::TMatrix), iSize, fp);

		//fwrite(&mesh->m_pAnimationMatrix, sizeof(T::TMatrix), header.iNumTrack, fp);
		for (int iFrame = 0; iFrame < header.iNumTrack; iFrame++)
		{
			fwrite(&mesh->m_pAnimationMatrix[iFrame], sizeof(T::TMatrix), 1, fp);
		}

		for (auto tex : mesh->m_szTexFileList)
		{
			TTexFileHeader texHead;
			texHead.iLength = tex.size();
			fwrite(&texHead, sizeof(TTexFileHeader), 1, fp);
			fwrite(tex.c_str(), sizeof(wchar_t), texHead.iLength, fp);
		}
		if (header.isSubMesh > 0)
		{
			for (auto sub : mesh->m_vSubMeshVertexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(PNCT_Vertex), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIndexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(DWORD), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIWVertexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(IW_Vertex), sub.size(), fp);
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
			// index + weight
			iLen = mesh->m_vIWVertexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vIWVertexList.at(0), sizeof(IW_Vertex),
					mesh->m_vIWVertexList.size(), fp);
			}
		}
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();
	return true;
}
bool  TKgcFileFormat::Import(std::wstring szFileName, std::wstring szShaderFile,
	std::shared_ptr<TFbxModel>& tFbxModel)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"rb");
	if (err != 0) return false;

	int iLen;
	TKgcFileHeader fileHeader;
	fread(&fileHeader, sizeof(TKgcFileHeader), 1, fp);
	WCHAR szLoadfilename[256] = { 0, };
	fread(szLoadfilename, sizeof(wchar_t), fileHeader.iLength, fp);
	tFbxModel->m_FileHeader = fileHeader;

	// TNode 
	tFbxModel->m_pTNodeList.resize(fileHeader.iNumNodeCounter);
	fread(&tFbxModel->m_pTNodeList.at(0), sizeof(TFbxNode),
		fileHeader.iNumNodeCounter, fp);

	if (fileHeader.iLastFrame > 0)
	{
		tFbxModel->m_pBoneAnimMatrix.resize(fileHeader.iNumNodeCounter);
		for (int iBone = 0; iBone < tFbxModel->m_pBoneAnimMatrix.size(); iBone++)
		{
			tFbxModel->m_pBoneAnimMatrix[iBone].resize(fileHeader.iLastFrame);
			fread(&tFbxModel->m_pBoneAnimMatrix[iBone].at(0),
				sizeof(T::TMatrix), fileHeader.iLastFrame, fp);
		}

		// 프레임 과 프레임 사이의 보간 작업 데이터
		tFbxModel->m_matAnimTrack.resize(tFbxModel->m_pBoneAnimMatrix.size());
		for (int iBone = 0; iBone < tFbxModel->m_pBoneAnimMatrix.size(); iBone++)
		{
			for (int iFrame = 0; iFrame < fileHeader.iLastFrame; iFrame++)
			{
				// 4x4
				// m_pBoneAnimMatrix = self matrix(SRV) * parent matrix(SRT);
				// self matrix = m_pBoneAnimMatrix * Inverse(parent boneAnimMatrix);
				T::TMatrix mat = tFbxModel->m_pBoneAnimMatrix[iBone][iFrame];
				// 행렬을 분해 S,R,T
				T::TMatrix matScale, matTrans, matRotate;
				T::TVector3 v, s;
				T::TQuaternion q;
				D3DXMatrixDecompose(&s, &q, &v, &mat );
				/*D3DXMatrixScaling(&matScale, s.x, s.y, s.z);
				D3DXMatrixTranslation(&matTrans, v.x, v.y, v.z);
				D3DXMatrixRotationQuaternion(&matRotate, &q);*/				
				//TTrack track(iFrame, s, q, v);
				tFbxModel->m_matAnimTrack[iBone].emplace_back(iFrame, s,q,v);
			}
		}
	}

	for (int iMesh = 0; iMesh < fileHeader.iChildNodeCounter; iMesh++)
	{
		std::shared_ptr<TFbxModel>  fbxModel = std::make_shared<TFbxModel>();

		TKgcFileHeader header;
		fread(&header, sizeof(TKgcFileHeader), 1, fp);
		fbxModel->m_FileHeader = header;

		// Bindpose matrix
		int iSize;
		fread(&iSize, sizeof(int), 1, fp);
		if (iSize <= 0) continue;
		fbxModel->m_matBindPose.resize(iSize);
		fread(&fbxModel->m_matBindPose.at(0), sizeof(T::TMatrix), iSize, fp);

		// animation
		/*fbxModel->m_pAnimationMatrix.resize(header.iNumTrack);
		for (int iFrame = 0; iFrame < header.iNumTrack; iFrame++)
		{
			fread(&fbxModel->m_pAnimationMatrix[iFrame], sizeof(T::TMatrix), 1, fp);

		}*/

		//fbxModel->m_pAnimationMatrix.resize(header.iNumTrack);
		//fread(&fbxModel->m_pAnimationMatrix, sizeof(T::TMatrix), header.iNumTrack, fp);

		fbxModel->m_matWorld = header.matWorld;
		fbxModel->m_matInitWorld = header.matWorld;

		fbxModel->m_szTexFileList.resize(header.iLength);
		for (int iTex = 0; iTex < header.iLength; iTex++)
		{
			TTexFileHeader texHead;
			fread(&texHead, sizeof(TTexFileHeader), 1, fp);
			wchar_t  szTexName[256] = { 0, };
			fread(szTexName, sizeof(wchar_t), texHead.iLength, fp);
			fbxModel->m_szTexFileList[iTex] = szTexName;
		}
		if (header.isSubMesh > 0)
		{
			for (int iVertex = 0; iVertex < header.iSubVertexBufferCounter; iVertex++)
			{
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				vList  subVertexList(iSize);
				fread(&subVertexList.at(0), sizeof(PNCT_Vertex), iSize, fp);
				fbxModel->m_vSubMeshVertexList.emplace_back(subVertexList);
			}


			for (int iIndex = 0; iIndex < header.iSubIndexBufferCounter; iIndex++)
			{
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				iList  subIndexList(iSize);
				fread(&subIndexList.at(0), sizeof(DWORD), iSize, fp);
				fbxModel->m_vSubMeshIndexList.emplace_back(subIndexList);
			}
			// skinning
			for (int iIndex = 0; iIndex < header.iSubIWVertexBufferCounter; iIndex++)
			{
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				iwList  subIW(iSize);
				fread(&subIW.at(0), sizeof(IW_Vertex), iSize, fp);
				fbxModel->m_vSubMeshIWVertexList.emplace_back(subIW);
			}
		}
		else
		{
			int iLen = 0;
			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vVertexList.resize(iLen);
				fread(&fbxModel->m_vVertexList.at(0), sizeof(PNCT_Vertex),
					iLen, fp);
			}

			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vIndexList.resize(iLen);
				fread(&fbxModel->m_vIndexList.at(0), sizeof(DWORD),
					iLen, fp);
			}

			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vIWVertexList.resize(iLen);
				fread(&fbxModel->m_vIWVertexList.at(0), sizeof(IW_Vertex), iLen, fp);
			}
		}

		tFbxModel->m_ChildModel.emplace_back(fbxModel);
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();


	for (int iObj = 0; iObj < tFbxModel->m_ChildModel.size(); iObj++)
	{
		auto pFbxMesh = tFbxModel->m_ChildModel[iObj];
		std::wstring name = L"../../data/";
		if (pFbxMesh->m_szTexFileList.size() > 0)
		{
			std::wstring texPath = pFbxMesh->m_szTexFileList[0];
			wchar_t  szDrive[MAX_PATH] = { 0, };
			wchar_t  szDir[MAX_PATH] = { 0, };
			wchar_t  szFileName[MAX_PATH] = { 0, };
			wchar_t  szFileExt[MAX_PATH] = { 0, };
			_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
			name += szFileName;
			std::wstring ext = szFileExt;

			if (ext == L".tga" || ext == L".TGA")
			{
				name += L".dds";
			}
			else
			{
				name += szFileExt;
			}
		}
		pFbxMesh->Create(name, szShaderFile);
	}

	return true;
}