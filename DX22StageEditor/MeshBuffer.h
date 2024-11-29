/* ========================================
	DX22Base/
	------------------------------------
	���b�V���o�b�t�@�p�w�b�_
	------------------------------------
	�����F���b�V���o�b�t�@���Ǘ�����N���X
	(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@��)
	------------------------------------
	MeshBuffer.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "DirectXManager.h"

// =============== �N���X ===================
class MeshBuffer
{
public:

	// ���b�V���f�[�^�̍\����
	struct T_MeshData
	{
		const void*					pVtx;		// ���_�f�[�^�̃|�C���^
		UINT						vtxSize;	// �e���_�̃T�C�Y	
		UINT						vtxCount;	// ���_��	
		bool						bWrite;		// ���_�f�[�^���������݉\��
		const void*					pIdx;		// �C���f�b�N�X�f�[�^�̃|�C���^
		UINT						idxSize;	// �e�C���f�b�N�X�̃T�C�Y
		UINT						idxCount;	// �C���f�b�N�X��
		D3D11_PRIMITIVE_TOPOLOGY	topology;	// �v���~�e�B�u�g�|���W�[�i��: �O�p�`���X�g�⃉�C�����X�g�j
	};
public:
	MeshBuffer(const T_MeshData& desc);
	~MeshBuffer();

	// �R�s�[�R���X�g���N�^
	MeshBuffer(const MeshBuffer& other);

	void Draw(int count = 0);
	HRESULT WriteVertexData(void* pVtx);

	// ���b�V���f�[�^�̎擾
	T_MeshData GetDesc();

private:
	HRESULT CreateVertexBuffer(const void* pIdx, UINT size, UINT count, bool isWrite);
	HRESULT CreateIndexBuffer(const void* pVtx, UINT size, UINT count);

private:
	ID3D11Buffer* m_pVtxBuffer;		// ���_�o�b�t�@
	ID3D11Buffer* m_pIdxBuffer;		// �C���f�b�N�X�o�b�t�@
	T_MeshData	m_tMeshDesc;		// ���b�V���f�[�^�̍\����

};

