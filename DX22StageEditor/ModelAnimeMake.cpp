/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f��(�e�퐶������)�pcpp
	------------------------------------
	ModelAnimeMake.cpp
========================================== */

// =============== �C���N���[�h ===================
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
	���b�V���쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^���烁�b�V�����쐬����
	-------------------------------------
	����1�Fconst void* ptr	�ǂݍ��݃f�[�^
	����2�Ffloat scale		�g�嗦
	����3�FFlip flip		���]���
=========================================== */
void ModelAnime::MakeMesh(const void* ptr, float scale, E_Flip flip)
{
	// ���O����
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);
	float zflip = (flip == E_Flip::ZFlip || flip == E_Flip::ZFlipUseAnime) ? -1.0f : 1.0f;
	int idx1 = flip != E_Flip::None ? 2 : 1;
	int idx2 = flip != E_Flip::None ? 1 : 2;

	// ���b�V���̍쐬
	m_MeshList.resize(pScene->mNumMeshes);
	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
	{
		// ���_�̍쐬
		m_MeshList[i].vertices.resize(pScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < m_MeshList[i].vertices.size(); ++j) {
			//--- ���f���f�[�^����l�̎擾
			// ���W
			aiVector3D pos = pScene->mMeshes[i]->mVertices[j];
			// �@���i������
			aiVector3D normal = pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : zero3;
			// UV(1�̃��b�V���ɕ����̃e�N�X�`�����\��t������ׁA�e�N�X�`���̖����ɉ�����UV���ݒ肳��Ă���
			aiVector3D uv = pScene->mMeshes[i]->HasTextureCoords(0) ?
				pScene->mMeshes[i]->mTextureCoords[0][j] : zero3;
			// ���_�ɐݒ肳��Ă���F
			aiColor4D color = pScene->mMeshes[i]->HasVertexColors(0) ? pScene->mMeshes[i]->mColors[0][j] : one4;
			//--- �l��ݒ�
			m_MeshList[i].vertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale * zflip, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				DirectX::XMFLOAT4(color.r, color.g, color.b, color.a),
			};
		}

		//�d�ݕt��
		MakeWeight(pScene, i);

		// �C���f�b�N�X�̍쐬
		// mNumFaces�̓|���S���̐���\��(�P�|���S���łR�C���f�b�N�X
		m_MeshList[i].indices.resize(pScene->mMeshes[i]->mNumFaces * 3);	// �C���f�b�N�X�̕K�v�����m��
		for (unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; ++j) {
			aiFace face = pScene->mMeshes[i]->mFaces[j];
			int idx = j * 3;
			m_MeshList[i].indices[idx + 0] = face.mIndices[0];
			m_MeshList[i].indices[idx + 1] = face.mIndices[idx1];
			m_MeshList[i].indices[idx + 2] = face.mIndices[idx2];
		}

		// �}�e���A���̊��蓖��
		m_MeshList[i].materialID = pScene->mMeshes[i]->mMaterialIndex;

		// ���b�V�������ɒ��_�o�b�t�@�쐬
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
	�}�e���A���쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^����}�e���A�����쐬����
	-------------------------------------
	����1�Fconst void* ptr			�ǂݍ��݃f�[�^
	����2�Fstd::string directory	�f�B���N�g��
=========================================== */
void ModelAnime::MakeMaterial(const void* ptr, std::string directory)
{
	// ���O����
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// �}�e���A���̍쐬
	m_MaterialList.resize(pScene->mNumMaterials);
	for (unsigned int i = 0; i < m_MaterialList.size(); ++i)
	{
		// �e��p�����[�^�[
		//=========================================
		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess;

		//�g�U���̎擾
		m_MaterialList[i].diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//�����̎擾
		m_MaterialList[i].ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		//���˂̋������擾
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		//���ˌ��̎擾
		m_MaterialList[i].specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		//=========================================

		// �e�N�X�`��
		HRESULT hr;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// �e�N�X�`���̈�m��
			m_MaterialList[i].pTexture = new Texture;
			// ���̂܂ܒT��
			hr = m_MaterialList[i].pTexture->Create(path.C_Str());
			// ���f���Ɠ����K�w��T��
			if (FAILED(hr))
			{
				// �p�X����t�@�C�����̂ݎ擾
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
				// �e�N�X�`���̓Ǎ�
				hr = m_MaterialList[i].pTexture->Create((directory + fileName).c_str());
			}
			// �e�N�X�`����������Ȃ�����
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
	�A�j���[�V�������f���f�[�^�Ǎ��֐�
	-------------------------------------
	���e�F�A�j���[�V�������f���f�[�^��ǂݍ���
	-------------------------------------
	�����Fconst void* ptr	�ǂݍ��݃f�[�^
=========================================== */
void ModelAnime::MakeAnimation(const void* ptr)
{
	// �A�j���[�V�����f�[�^�m��
	aiAnimation* assimpAnime = reinterpret_cast<const aiScene*>(ptr)->mAnimations[0];
	m_AnimeList.push_back(T_Animation());
	T_Animation& anime = m_AnimeList.back();	// �ǉ�����A�j���[�V����

	// �A�j���[�V�����ݒ�
	float animeFrame	= static_cast<float>(assimpAnime->mTicksPerSecond);
	anime.totalTime		= static_cast<float>(assimpAnime->mDuration) / animeFrame;	// �A�j���[�V�����̑�����

	// �`�����l��(�{�[���ƃA�j���[�V�����̕R�t��)�̍쐬
	anime.channels.resize(assimpAnime->mNumChannels);
	Channels::iterator channelIt = anime.channels.begin();
	while (channelIt != anime.channels.end())
	{
		// �Ή�����`�����l��(�{�[��)��T��
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

		// �e�L�[�̒l��ݒ�
		channelIt->index = static_cast<NodeIndex>(nodeIt - m_NodeList.begin());
		Timeline& timeline = channelIt->timeline;

		// ��xXMVECTOR�^�Ŋi�[
		using XMVectorKey = std::pair<float, DirectX::XMVECTOR>;
		using XMVectorKeys = std::map<float, DirectX::XMVECTOR>;
		XMVectorKeys keys[3];
		// �ʒu
		for (UINT i = 0; i < assimpChannel->mNumPositionKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mPositionKeys[i];
			keys[0].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)
			));
		}
		// ��]
		for (UINT i = 0; i < assimpChannel->mNumRotationKeys; ++i)
		{
			aiQuatKey& key = assimpChannel->mRotationKeys[i];
			keys[1].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w)));
		}
		// �g�k
		for (UINT i = 0; i < assimpChannel->mNumScalingKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mScalingKeys[i];
			keys[2].insert(XMVectorKey(static_cast<float>(key.mTime) / animeFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)));
		}

		// �e�^�C�����C���̐擪�̎Q�Ƃ�ݒ�
		XMVectorKeys::iterator it[] = { keys[0].begin(), keys[1].begin(), keys[2].begin() };
		for (int i = 0; i < 3; ++i)
		{
			// �L�[��������Ȃ��ꍇ�́A�Q�ƏI��
			if (keys[i].size() == 1)
				++it[i];
		}

		// �e�v�f���Ƃ̃^�C�����C���ł͂Ȃ��A���ׂĂ̕ϊ����܂߂��^�C�����C���̍쐬
		while (it[0] != keys[0].end() && it[1] != keys[1].end() && it[2] != keys[2].end())
		{
			// ����̎Q�ƈʒu�ň�ԏ��������Ԃ��擾
			float time = anime.totalTime;
			for (int i = 0; i < 3; ++i)
			{
				if (it[i] != keys[i].end())
				{
					time = std::min(it[i]->first, time);
				}
			}

			// ���ԂɊ�Â��ĕ�Ԓl���v�Z
			DirectX::XMVECTOR result[3];
			for (int i = 0; i < 3; ++i)
			{
				// �擪�̃L�[��菬�������Ԃł���΁A�擪�̒l��ݒ�
				if (time < keys[i].begin()->first)
				{
					result[i] = keys[i].begin()->second;
				}
				// �ŏI�L�[���傫�����Ԃł���΁A�ŏI�̒l��ݒ�
				else if (keys[i].rbegin()->first <= time)
				{
					result[i] = keys[i].rbegin()->second;
					it[i] = keys[i].end();
				}
				// �L�[���m�ɋ��܂ꂽ���Ԃł���΁A��Ԓl���v�Z
				else
				{
					// �Q�Ƃ��Ă��鎞�ԂƓ����ł���΁A���̎Q�ƂփL�[��i�߂�
					if (it[i]->first <= time)
					{
						++it[i];
					}

					// ��Ԓl�̌v�Z
					XMVectorKeys::iterator prev = it[i];
					--prev;
					float rate = (time - prev->first) / (it[i]->first - prev->first);
					result[i] = DirectX::XMVectorLerp(prev->second, it[i]->second, rate);
				}
			}

			// �w�莞�ԂɊ�Â����L�[��ǉ�
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
	�{�[���m�[�h�쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^����{�[���m�[�h���쐬����
	-------------------------------------
	�����Fconst void* ptr	�ǂݍ��݃f�[�^
=========================================== */
void ModelAnime::MakeBoneNodes(const void* ptr)
{
	// �ċA������Assimp�̃m�[�h����ǂݎ��
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
			// Assimp�̃m�[�h�������f���N���X�֊i�[
			T_Node node;
			node.name = assimpNode->mName.data;
			node.parent = parent;
			node.children.resize(assimpNode->mNumChildren);
			node.mat = mat;

			// �m�[�h���X�g�ɒǉ�
			m_NodeList.push_back(node);
			NodeIndex nodeIndex = static_cast<NodeIndex>(m_NodeList.size() - 1);

			// �q�v�f�����l�ɕϊ�
			for (UINT i = 0; i < assimpNode->mNumChildren; ++i)
			{
				m_NodeList[nodeIndex].children[i] = FuncAssimpNodeConvert(
					assimpNode->mChildren[i], nodeIndex, DirectX::XMMatrixIdentity());
			}
			return nodeIndex;
		}
	};

	// �m�[�h�쐬
	m_NodeList.clear();
	FuncAssimpNodeConvert(reinterpret_cast<const aiScene*>(ptr)->mRootNode, INDEX_NONE, DirectX::XMMatrixIdentity());

	// �A�j���[�V�����v�Z�̈�ɁA�m�[�h�����̏����f�[�^���쐬
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
	�E�F�C�g�쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^����E�F�C�g���쐬����
	-------------------------------------
	����1�Fconst void* ptr	�ǂݍ��݃f�[�^
	����2�Fint meshIdx		���b�V���ԍ�
=========================================== */
void ModelAnime::MakeWeight(const void* ptr, int meshIdx)
{
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// ���b�V���ɑΉ�����{�[�������邩
	aiMesh* assimpMesh = pScene->mMeshes[meshIdx];
	T_Mesh& mesh = m_MeshList[meshIdx];
	if (assimpMesh->HasBones())
	{
		// ���b�V���Ɋ��蓖�Ă��Ă���{�[���̈�m��
		mesh.bones.resize(assimpMesh->mNumBones);
		for (auto boneIt = mesh.bones.begin(); boneIt != mesh.bones.end(); ++boneIt)
		{
			UINT boneIdx = static_cast<UINT>(boneIt - mesh.bones.begin());
			aiBone* assimpBone = assimpMesh->mBones[boneIdx];
			// �\�z�ς݂̃{�[���m�[�h����Y���m�[�h���擾
			std::string boneName = assimpBone->mName.data;
			auto nodeIt = std::find_if(m_NodeList.begin(), m_NodeList.end(),
				[boneName](const T_Node& val) {
				return val.name == boneName;
			});
			// ���b�V���Ɋ��蓖�Ă��Ă���{�[�����A�m�[�h�ɑ��݂��Ȃ�
			if (nodeIt == m_NodeList.end())
			{
				boneIt->index = INDEX_NONE;
				continue;
			}

			// ���b�V���̃{�[���ƃm�[�h�̕R�Â�
			boneIt->index = static_cast<NodeIndex>(nodeIt - m_NodeList.begin());
			boneIt->invOffset = GetMatrixFromAssimpMatrix(assimpBone->mOffsetMatrix);
			boneIt->invOffset.r[3].m128_f32[0] *= m_fLoadScale;
			boneIt->invOffset.r[3].m128_f32[1] *= m_fLoadScale;
			boneIt->invOffset.r[3].m128_f32[2] *= m_fLoadScale;
			boneIt->invOffset =
				DirectX::XMMatrixScaling(m_LoadFlip == ZFlipUseAnime ? -1.0f : 1.0f, 1.0f, 1.0f) *
				boneIt->invOffset *
				DirectX::XMMatrixScaling(1.f / m_fLoadScale, 1.f / m_fLoadScale, 1.f / m_fLoadScale);

			// �E�F�C�g�̐ݒ�
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
		// ���b�V���̐e�m�[�h���g�����X�t�H�[�����Ƃ��Čv�Z
		std::string nodeName = assimpMesh->mName.data;
		auto nodeIt = std::find_if(m_NodeList.begin(), m_NodeList.end(),
			[nodeName](const T_Node& val) {
			return val.name == nodeName;
		});
		if (nodeIt == m_NodeList.end())
		{
			return;	// �{�[���f�[�^�Ȃ�
		}

		// ���b�V���łȂ��e�m�[�h���ċA�T��
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
	assimp�s��XMMATRIX�ϊ��֐�
	-------------------------------------
	���e�Fassimp���̍s���XMMATRIX�^�ɕϊ�
	-------------------------------------
	�����FaiMatrix4x4	M assimp�̍s��
	-------------------------------------
	�ߒl�FDirectX::XMMATRIX		�ϊ���̍s��
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