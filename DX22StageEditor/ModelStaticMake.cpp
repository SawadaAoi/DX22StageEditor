/* ========================================
	DX22Base/
	------------------------------------
	�ÓI���f���e�퐶�������pcpp
	------------------------------------
	ModelStaticMake.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ModelStatic.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/* ========================================
	���b�V���쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^���烁�b�V�����쐬����
	-------------------------------------
	����1�Fconst void* ptr	�ǂݍ��݃f�[�^
	����2�Ffloat scale		�g�嗦
	����3�FFlip flip		���]���
=========================================== */
void ModelStatic::MakeMesh(const void* ptr, float scale, E_Flip flip)
{
	// ���O����
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);
	float zflip = flip == E_Flip::ZFlip ? -1.0f : 1.0f;
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
			aiVector3D pos		= pScene->mMeshes[i]->mVertices[j];
			// �@���i������
			aiVector3D normal	= pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : zero3;
			// UV(1�̃��b�V���ɕ����̃e�N�X�`�����\��t������ׁA�e�N�X�`���̖����ɉ�����UV���ݒ肳��Ă���
			aiVector3D uv		= pScene->mMeshes[i]->HasTextureCoords(0) ?
								  pScene->mMeshes[i]->mTextureCoords[0][j] : zero3;
			// ���_�ɐݒ肳��Ă���F
			aiColor4D color		= pScene->mMeshes[i]->HasVertexColors(0) ? pScene->mMeshes[i]->mColors[0][j] : one4;
			//--- �l��ݒ�
			m_MeshList[i].vertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale * zflip, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				DirectX::XMFLOAT4(color.r, color.g, color.b, color.a)
			};
		}
		
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
	�}�e���A���쐬�֐�
	-------------------------------------
	���e�F�ǂݍ��݃f�[�^����}�e���A�����쐬����
	-------------------------------------
	����1�Fconst void* ptr			�ǂݍ��݃f�[�^
	����2�Fstd::string directory	�f�B���N�g��
=========================================== */
void ModelStatic::MakeMaterial(const void* ptr, std::string directory)
{
	// ���O����
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// �}�e���A���̍쐬
	m_MaterialList.resize(pScene->mNumMaterials);
	for (unsigned int i = 0; i < m_MaterialList.size(); ++i)
	{
		// �e��p�����[�^�[
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

