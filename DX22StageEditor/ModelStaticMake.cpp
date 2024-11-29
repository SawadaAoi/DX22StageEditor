/* ========================================
	DX22Base/
	------------------------------------
	静的モデル各種生成処理用cpp
	------------------------------------
	ModelStaticMake.cpp
========================================== */

// =============== インクルード ===================
#include "ModelStatic.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/* ========================================
	メッシュ作成関数
	-------------------------------------
	内容：読み込みデータからメッシュを作成する
	-------------------------------------
	引数1：const void* ptr	読み込みデータ
	引数2：float scale		拡大率
	引数3：Flip flip		反転種類
=========================================== */
void ModelStatic::MakeMesh(const void* ptr, float scale, E_Flip flip)
{
	// 事前準備
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);
	float zflip = flip == E_Flip::ZFlip ? -1.0f : 1.0f;
	int idx1 = flip != E_Flip::None ? 2 : 1;
	int idx2 = flip != E_Flip::None ? 1 : 2;

	// メッシュの作成
	m_MeshList.resize(pScene->mNumMeshes);
	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
	{
		// 頂点の作成
		m_MeshList[i].vertices.resize(pScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < m_MeshList[i].vertices.size(); ++j) {
			//--- モデルデータから値の取得
			// 座標
			aiVector3D pos		= pScene->mMeshes[i]->mVertices[j];
			// 法線（後日解説
			aiVector3D normal	= pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : zero3;
			// UV(1つのメッシュに複数のテクスチャが貼り付けられる為、テクスチャの枚数に応じてUVが設定されている
			aiVector3D uv		= pScene->mMeshes[i]->HasTextureCoords(0) ?
								  pScene->mMeshes[i]->mTextureCoords[0][j] : zero3;
			// 頂点に設定されている色
			aiColor4D color		= pScene->mMeshes[i]->HasVertexColors(0) ? pScene->mMeshes[i]->mColors[0][j] : one4;
			//--- 値を設定
			m_MeshList[i].vertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale * zflip, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				DirectX::XMFLOAT4(color.r, color.g, color.b, color.a)
			};
		}
		
		// インデックスの作成
		// mNumFacesはポリゴンの数を表す(１ポリゴンで３インデックス
		m_MeshList[i].indices.resize(pScene->mMeshes[i]->mNumFaces * 3);	// インデックスの必要数分確保
		for (unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; ++j) {
			aiFace face = pScene->mMeshes[i]->mFaces[j];
			int idx = j * 3;
			m_MeshList[i].indices[idx + 0] = face.mIndices[0];
			m_MeshList[i].indices[idx + 1] = face.mIndices[idx1];
			m_MeshList[i].indices[idx + 2] = face.mIndices[idx2];
		}

		// マテリアルの割り当て
		m_MeshList[i].materialID = pScene->mMeshes[i]->mMaterialIndex;

		// メッシュを元に頂点バッファ作成
		MeshBuffer::T_MeshData desc = {};
		desc.pVtx			= m_MeshList[i].vertices.data();
		desc.vtxSize		= sizeof(T_Vertex);		
		desc.vtxCount		= m_MeshList[i].vertices.size();		
		desc.pIdx			= m_MeshList[i].indices.data();
		desc.idxSize		= sizeof(unsigned long);			
		desc.idxCount		= m_MeshList[i].indices.size();		
		desc.topology		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_MeshList[i].pMesh = new MeshBuffer(desc);
	}
}

/* ========================================
	マテリアル作成関数
	-------------------------------------
	内容：読み込みデータからマテリアルを作成する
	-------------------------------------
	引数1：const void* ptr			読み込みデータ
	引数2：std::string directory	ディレクトリ
=========================================== */
void ModelStatic::MakeMaterial(const void* ptr, std::string directory)
{
	// 事前準備
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// マテリアルの作成
	m_MaterialList.resize(pScene->mNumMaterials);
	for (unsigned int i = 0; i < m_MaterialList.size(); ++i)
	{
		// 各種パラメーター
		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess;

		//拡散光の取得
		m_MaterialList[i].diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//環境光の取得
		m_MaterialList[i].ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		//反射の強さを取得
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		//反射光の取得
		m_MaterialList[i].specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// テクスチャ
		HRESULT hr;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// テクスチャ領域確保
			m_MaterialList[i].pTexture = new Texture;
			// そのまま探索
			hr = m_MaterialList[i].pTexture->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr))
			{
				// パスからファイル名のみ取得
				std::string fullPath = path.C_Str();
				std::string::iterator strIt = fullPath.begin();
				while (strIt != fullPath.end())
				{
					if (*strIt == '/')
						*strIt = '\\';
					++strIt;
				}
				size_t find = fullPath.find_last_of("\\");
				std::string fileName = fullPath;
				if (find != std::string::npos)
					fileName = fileName.substr(find + 1);
				// テクスチャの読込
				hr = m_MaterialList[i].pTexture->Create((directory + fileName).c_str());
			}
			// テクスチャが見つからなかった
			if (FAILED(hr)) {
				delete m_MaterialList[i].pTexture;
				m_MaterialList[i].pTexture = nullptr;
#ifdef _DEBUG
				m_sErrorMessage += path.C_Str();
#endif
			}
		}
		else {
			m_MaterialList[i].pTexture = nullptr;
		}
	}
}

