/* ========================================
	DX22Base/
	------------------------------------
	アニメーションモデル(各種生成処理)用cpp
	------------------------------------
	ModelAnimeMake.cpp
========================================== */

// =============== インクルード ===================
#include "ModelAnime.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#if _MSC_VER >= 1930
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif
#elif _MSC_VER >= 1920
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#elif _MSC_VER >= 1910
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc141-mtd.lib")
#else
#pragma comment(lib, "assimp-vc141-mt.lib")
#endif
#endif


/* ========================================
	メッシュ作成関数
	-------------------------------------
	内容：読み込みデータからメッシュを作成する
	-------------------------------------
	引数1：const void* ptr	読み込みデータ
	引数2：float scale		拡大率
	引数3：Flip flip		反転種類
=========================================== */
void ModelAnime::MakeMesh(const void* ptr, float scale, E_Flip flip)
{
	// 事前準備
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);
	float zflip = (flip == E_Flip::ZFlip || flip == E_Flip::ZFlipUseAnime) ? -1.0f : 1.0f;
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
			aiVector3D pos = pScene->mMeshes[i]->mVertices[j];
			// 法線（後日解説
			aiVector3D normal = pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : zero3;
			// UV(1つのメッシュに複数のテクスチャが貼り付けられる為、テクスチャの枚数に応じてUVが設定されている
			aiVector3D uv = pScene->mMeshes[i]->HasTextureCoords(0) ?
				pScene->mMeshes[i]->mTextureCoords[0][j] : zero3;
			// 頂点に設定されている色
			aiColor4D color = pScene->mMeshes[i]->HasVertexColors(0) ? pScene->mMeshes[i]->mColors[0][j] : one4;
			//--- 値を設定
			m_MeshList[i].vertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale * zflip, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				DirectX::XMFLOAT4(color.r, color.g, color.b, color.a),
			};
		}

		//重み付け
		MakeWeight(pScene, i);

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
		desc.pVtx = m_MeshList[i].vertices.data();
		desc.vtxSize = sizeof(T_Vertex);
		desc.vtxCount = UINT(m_MeshList[i].vertices.size());
		desc.pIdx = m_MeshList[i].indices.data();
		desc.idxSize = sizeof(unsigned long);
		desc.idxCount = UINT(m_MeshList[i].indices.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
void ModelAnime::MakeMaterial(const void* ptr, std::string directory)
{
	// 事前準備
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// マテリアルの作成
	m_MaterialList.resize(pScene->mNumMaterials);
	for (unsigned int i = 0; i < m_MaterialList.size(); ++i)
	{
		// 各種パラメーター
		//=========================================
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

		//=========================================

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

/* ========================================
	アニメーションモデルデータ読込関数
	-------------------------------------
	内容：アニメーションモデルデータを読み込む
	-------------------------------------
	引数：const void* ptr	読み込みデータ
=========================================== */
void ModelAnime::MakeAnimation(const void* ptr)
{
	// アニメーションデータ確保
	aiAnimation* assimpAnime = reinterpret_cast<const aiScene*>(ptr)->mAnimations[0];
	m_AnimeList.push_back(T_Animation());
	T_Animation& anime = m_AnimeList.back();	// 追加するアニメーション

	// アニメーション設定
	float animeFrame	= static_cast<float>(assimpAnime->mTicksPerSecond);
	anime.totalTime		= static_cast<float>(assimpAnime->mDuration) / animeFrame;	// アニメーションの総時間

	// チャンネル(ボーンとアニメーションの紐付け)の作成
	anime.channels.resize(assimpAnime->mNumChannels);
	Channels::iterator channelIt = anime.channels.begin();
	while (channelIt != anime.channels.end())
	{
		// 対応するチャンネル(ボーン)を探索
		uint32_t channelIdx = static_cast<uint32_t>(channelIt - anime.channels.begin());
		aiNodeAnim* assimpChannel = assimpAnime->mChannels[channelIdx];
		ModelAnime::Nodes::iterator nodeIt = std::find_if(m_NodeList.begin(), m_NodeList.end(),
			[assimpChannel](T_Node& node) {
			return node.name == assimpChannel->mNodeName.data;
		});
		if (nodeIt == m_NodeList.end())
		{
			channelIt->index = INDEX_NONE;
			++channelIt;
			continue;
		}

		// 各キーの値を設定
		channelIt->index = static_cast<NodeIndex>(nodeIt - m_NodeList.begin());
		Timeline& timeline = channelIt->timeline;

		// 一度XMVECTOR型で格納
		using XMVectorKey = std::pair<float, DirectX::XMVECTOR>;
		using XMVectorKeys = std::map<float, DirectX::XMVECTOR>;
		XMVectorKeys keys[3];
		// 位置
		for (UINT i = 0; i < assimpChannel->mNumPositionKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mPositionKeys[i];
			keys[0].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)
			));
		}
		// 回転
		for (UINT i = 0; i < assimpChannel->mNumRotationKeys; ++i)
		{
			aiQuatKey& key = assimpChannel->mRotationKeys[i];
			keys[1].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w)));
		}
		// 拡縮
		for (UINT i = 0; i < assimpChannel->mNumScalingKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mScalingKeys[i];
			keys[2].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)));
		}

		// 各タイムラインの先頭の参照を設定
		XMVectorKeys::iterator it[] = { keys[0].begin(), keys[1].begin(), keys[2].begin() };
		for (int i = 0; i < 3; ++i)
		{
			// キーが一つしかない場合は、参照終了
			if (keys[i].size() == 1)
				++it[i];
		}

		// 各要素ごとのタイムラインではなく、すべての変換を含めたタイムラインの作成
		while (it[0] != keys[0].end() && it[1] != keys[1].end() && it[2] != keys[2].end())
		{
			// 現状の参照位置で一番小さい時間を取得
			float time = anime.totalTime;
			for (int i = 0; i < 3; ++i)
			{
				if (it[i] != keys[i].end())
				{
					time = std::min(it[i]->first, time);
				}
			}

			// 時間に基づいて補間値を計算
			DirectX::XMVECTOR result[3];
			for (int i = 0; i < 3; ++i)
			{
				// 先頭のキーより小さい時間であれば、先頭の値を設定
				if (time < keys[i].begin()->first)
				{
					result[i] = keys[i].begin()->second;
				}
				// 最終キーより大きい時間であれば、最終の値を設定
				else if (keys[i].rbegin()->first <= time)
				{
					result[i] = keys[i].rbegin()->second;
					it[i] = keys[i].end();
				}
				// キー同士に挟まれた時間であれば、補間値を計算
				else
				{
					// 参照している時間と同じであれば、次の参照へキーを進める
					if (it[i]->first <= time)
					{
						++it[i];
					}

					// 補間値の計算
					XMVectorKeys::iterator prev = it[i];
					--prev;
					float rate = (time - prev->first) / (it[i]->first - prev->first);
					result[i] = DirectX::XMVectorLerp(prev->second, it[i]->second, rate);
				}
			}

			// 指定時間に基づいたキーを追加
			T_Transform transform;
			DirectX::XMStoreFloat3(&transform.position, result[0]);
			DirectX::XMStoreFloat4(&transform.quaternion, result[1]);
			DirectX::XMStoreFloat3(&transform.scale, result[2]);
			timeline.insert(Key(time, transform));
		}

		++channelIt;
	}
}

/* ========================================
	ボーンノード作成関数
	-------------------------------------
	内容：読み込みデータからボーンノードを作成する
	-------------------------------------
	引数：const void* ptr	読み込みデータ
=========================================== */
void ModelAnime::MakeBoneNodes(const void* ptr)
{
	// 再帰処理でAssimpのノード情報を読み取り
	std::function<NodeIndex(aiNode*, NodeIndex, DirectX::XMMATRIX mat)> FuncAssimpNodeConvert =
		[&FuncAssimpNodeConvert, this](aiNode* assimpNode, NodeIndex parent, DirectX::XMMATRIX mat)
	{
		DirectX::XMMATRIX transform = GetMatrixFromAssimpMatrix(assimpNode->mTransformation);
		std::string name = assimpNode->mName.data;
		if (name.find("$AssimpFbx") != std::string::npos)
		{
			mat = transform * mat;
			return FuncAssimpNodeConvert(assimpNode->mChildren[0], parent, mat);
		}
		else
		{
			// Assimpのノード情報をモデルクラスへ格納
			T_Node node;
			node.name = assimpNode->mName.data;
			node.parent = parent;
			node.children.resize(assimpNode->mNumChildren);
			node.mat = mat;

			// ノードリストに追加
			m_NodeList.push_back(node);
			NodeIndex nodeIndex = static_cast<NodeIndex>(m_NodeList.size() - 1);

			// 子要素も同様に変換
			for (UINT i = 0; i < assimpNode->mNumChildren; ++i)
			{
				m_NodeList[nodeIndex].children[i] = FuncAssimpNodeConvert(
					assimpNode->mChildren[i], nodeIndex, DirectX::XMMatrixIdentity());
			}
			return nodeIndex;
		}
	};

	// ノード作成
	m_NodeList.clear();
	FuncAssimpNodeConvert(reinterpret_cast<const aiScene*>(ptr)->mRootNode, INDEX_NONE, DirectX::XMMatrixIdentity());

	// アニメーション計算領域に、ノード数分の初期データを作成
	T_Transform init = {
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)
	};
	for (int i = 0; i < MAX_TRANSFORM; ++i)
	{
		m_NodeTransform[i].resize(m_NodeList.size(), init);
	}
}

/* ========================================
	ウェイト作成関数
	-------------------------------------
	内容：読み込みデータからウェイトを作成する
	-------------------------------------
	引数1：const void* ptr	読み込みデータ
	引数2：int meshIdx		メッシュ番号
=========================================== */
void ModelAnime::MakeWeight(const void* ptr, int meshIdx)
{
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// メッシュに対応するボーンがあるか
	aiMesh* assimpMesh = pScene->mMeshes[meshIdx];
	T_Mesh& mesh = m_MeshList[meshIdx];
	if (assimpMesh->HasBones())
	{
		// メッシュに割り当てられているボーン領域確保
		mesh.bones.resize(assimpMesh->mNumBones);
		for (auto boneIt = mesh.bones.begin(); boneIt != mesh.bones.end(); ++boneIt)
		{
			UINT boneIdx = static_cast<UINT>(boneIt - mesh.bones.begin());
			aiBone* assimpBone = assimpMesh->mBones[boneIdx];
			// 構築済みのボーンノードから該当ノードを取得
			std::string boneName = assimpBone->mName.data;
			auto nodeIt = std::find_if(m_NodeList.begin(), m_NodeList.end(),
				[boneName](const T_Node& val) {
				return val.name == boneName;
			});
			// メッシュに割り当てられているボーンが、ノードに存在しない
			if (nodeIt == m_NodeList.end())
			{
				boneIt->index = INDEX_NONE;
				continue;
			}

			// メッシュのボーンとノードの紐づけ
			boneIt->index = static_cast<NodeIndex>(nodeIt - m_NodeList.begin());
			boneIt->invOffset = GetMatrixFromAssimpMatrix(assimpBone->mOffsetMatrix);
			boneIt->invOffset.r[3].m128_f32[0] *= m_fLoadScale;
			boneIt->invOffset.r[3].m128_f32[1] *= m_fLoadScale;
			boneIt->invOffset.r[3].m128_f32[2] *= m_fLoadScale;
			boneIt->invOffset =
				DirectX::XMMatrixScaling(m_LoadFlip == ZFlipUseAnime ? -1.0f : 1.0f, 1.0f, 1.0f) *
				boneIt->invOffset *
				DirectX::XMMatrixScaling(1.f / m_fLoadScale, 1.f / m_fLoadScale, 1.f / m_fLoadScale);

			// ウェイトの設定
			UINT weightNum = assimpBone->mNumWeights;
			for (UINT i = 0; i < weightNum; ++i)
			{
				aiVertexWeight weight = assimpBone->mWeights[i];
				T_Vertex& vtx = mesh.vertices[weight.mVertexId];
				for (int j = 0; j < 4; ++j)
				{
					if (vtx.weight[j] <= 0.0f)
					{
						vtx.index[j] = boneIdx;
						vtx.weight[j] = weight.mWeight;
						break;
					}
				}
			}
		}
	}
	else
	{
		// メッシュの親ノードをトランスフォーム元として計算
		std::string nodeName = assimpMesh->mName.data;
		auto nodeIt = std::find_if(m_NodeList.begin(), m_NodeList.end(),
			[nodeName](const T_Node& val) {
			return val.name == nodeName;
		});
		if (nodeIt == m_NodeList.end())
		{
			return;	// ボーンデータなし
		}

		// メッシュでない親ノードを再帰探索
		std::function<int(int)> FuncFindNode =
			[&FuncFindNode, this, pScene](NodeIndex parent)
		{
			std::string name = m_NodeList[parent].name;
			for (UINT i = 0; i < pScene->mNumMeshes; ++i)
			{
				if (name == pScene->mMeshes[i]->mName.data)
				{
					return FuncFindNode(m_NodeList[parent].parent);
				}
			}
			return parent;
		};

		T_Bone bone;
		bone.index = FuncFindNode(nodeIt->parent);
		bone.invOffset = DirectX::XMMatrixInverse(nullptr, m_NodeList[bone.index].mat);
		for (auto vtxIt = mesh.vertices.begin(); vtxIt != mesh.vertices.end(); ++vtxIt)
		{
			vtxIt->weight[0] = 1.0f;
		}

		mesh.bones.resize(1);
		mesh.bones[0] = bone;
	}
}

/* ========================================
	assimp行列→XMMATRIX変換関数
	-------------------------------------
	内容：assimp内の行列をXMMATRIX型に変換
	-------------------------------------
	引数：aiMatrix4x4	M assimpの行列
	-------------------------------------
	戻値：DirectX::XMMATRIX		変換後の行列
=========================================== */
DirectX::XMMATRIX ModelAnime::GetMatrixFromAssimpMatrix(aiMatrix4x4 M)
{
	return DirectX::XMMatrixSet(
		M.a1, M.b1, M.c1, M.d1,
		M.a2, M.b2, M.c2, M.d2,
		M.a3, M.b3, M.c3, M.d3,
		M.a4, M.b4, M.c4, M.d4
	);
}