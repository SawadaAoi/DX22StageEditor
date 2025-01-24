/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(������)�pcpp
	------------------------------------
	ShapeBox.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapeBox.h"

// =============== �萔��` =======================
const int MAX_TEXTURE_NUM = 6;	// �e�N�X�`���ő吔
const int MAX_UV_NUM = 3;		// uv�ő吔(Z,X,Y)

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
ShapeBox::ShapeBox()
	: ShapeBase()
{
	MakeMesh();	// ���b�V���̍쐬

	m_pTextures.resize(MAX_TEXTURE_NUM);	// �e�N�X�`���̐���ݒ�
	m_fUvOffset.resize(MAX_UV_NUM);	// uv�I�t�Z�b�g�̐���ݒ�
	m_fUvScale.resize(MAX_UV_NUM);	// uv�X�P�[���̐���ݒ�
}

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�FfPos		���W
	����2�FfSize	�T�C�Y
	����3�FfColor	�F
	����4�FeMode	�`�惂�[�h
=========================================== */
ShapeBox::ShapeBox(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode)
	: ShapeBox()
{
	SetPosition(fPos);
	SetScale(fSize);
	SetColor(fColor);
	SetDrawMode(eMode);
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`����s��
=========================================== */
void ShapeBox::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// �[�x�o�b�t�@�̗L����

	SetWVPMatrix();	// WVP�s��̐ݒ�

	// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)�̏�������
	m_pVS->WriteBuffer(0, m_WVP);



	// �萔�o�b�t�@(�F�A�\�����[�h�A�e�N�X�`���g�p�t���O)�̏�������
	float	fData[3] = { m_fColor.x, m_fColor.y, m_fColor.z };	// �F
	int		nData[2] = { m_eDrawMode, m_bIsTex };				// �\�����[�h�A�e�N�X�`���g�p�t���O
	m_pPS->WriteBuffer(0, fData);
	m_pPS->WriteBuffer(1, nData);

	// �V�F�[�_�[�̃o�C���h(�V�F�[�_�[�̐ݒ�� GPU �ɑ���)
	m_pVS->Bind();
	m_pPS->Bind();

	if (m_bIsCulling)
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// �J�����O�L��
	else
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_NONE);	// �J�����O����

	// �e�N�X�`��(�C���f�b�N�X����؂��ĕ`�悷��)
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		int nIdx = i / 2;	// 0,1,2,3,4,5 -> 0,0,1,1,2,2
		// Uv�X�P�[���A�I�t�Z�b�g�̐ݒ�
		DirectX::XMFLOAT4 fUvBuf = { m_fUvScale[nIdx].x, m_fUvScale[nIdx].y, m_fUvOffset[nIdx].x, m_fUvOffset[nIdx].y };
		m_pVS->WriteBuffer(1, &fUvBuf);

		m_pPS->SetTexture(0, m_pTextures[i]);		// �e�N�X�`���̐ݒ�
		m_pMeshBuffer[m_eDrawMode]->Draw(6, i * 6);	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@��GPU�ɐݒ肵�A�`��

	}

	// �J�����O�����ɖ߂�
	DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// �J�����O�L��
}

/* ========================================
	���b�V���쐬�֐�
	-------------------------------------
	���e�F���b�V�����쐬����
=========================================== */
void ShapeBox::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}


/* ========================================
	���b�V���쐬(�ʏ�)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(�ʏ�)
=========================================== */
void ShapeBox::MakeMeshNormal()
{
	// �e�ʂ̖@��
	Vector3<float> fNormal[6] = {
		{ 0.0f, 0.0f,-1.0f},	// -Z��
		{ 0.0f, 0.0f, 1.0f},	// +Z��
		{-1.0f, 0.0f, 0.0f},	// -X��
		{ 1.0f, 0.0f, 0.0f},	// +X��
		{ 0.0f,-1.0f, 0.0f},	// -Y��
		{ 0.0f, 1.0f, 0.0f},	// +Y��
	};


	// �e���_�̍��W�E�@���Euv�f�[�^�̍쐬
	T_Vertex vtx[] = {

		// ���_�̏��Ԃ�Z���ɂȂ��Ă���
		// ��O�A�� ----------------------------------
		// -Z��
		{{-0.5f, 0.5f,-0.5f}, {fNormal[0]}, { 0.0f, 0.0f}},	// ����
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[0]}, { 1.0f, 0.0f}},	// �E��
		{{-0.5f,-0.5f,-0.5f}, {fNormal[0]}, { 0.0f, 1.0f}},	// ����	
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[0]}, { 1.0f, 1.0f}},	// �E��

		// +Z��				 
		{{  0.5f, 0.5f,0.5f}, {fNormal[1]}, { 0.0f, 0.0f}},
		{{ -0.5f, 0.5f,0.5f}, {fNormal[1]}, { 1.0f, 0.0f}},
		{{  0.5f,-0.5f,0.5f}, {fNormal[1]}, { 0.0f, 1.0f}},
		{{ -0.5f,-0.5f,0.5f}, {fNormal[1]}, { 1.0f, 1.0f}},

		// ���E ----------------------------------
		// -X��
		{{-0.5f, 0.5f, 0.5f}, {fNormal[2]}, { 0.0f, 0.0f}},
		{{-0.5f, 0.5f,-0.5f}, {fNormal[2]}, { 1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.5f}, {fNormal[2]}, { 0.0f, 1.0f}},
		{{-0.5f,-0.5f,-0.5f}, {fNormal[2]}, { 1.0f, 1.0f}},

		// +X��				 		  
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[3]}, { 0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.5f}, {fNormal[3]}, { 1.0f, 0.0f}},
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[3]}, { 0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f}, {fNormal[3]}, { 1.0f, 1.0f}},

		// �㉺ ----------------------------------
		// -Y��
		{{-0.5f,-0.5f,-0.5f}, {fNormal[4]}, { 0.0f, 0.0f}},
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[4]}, { 1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.5f}, {fNormal[4]}, { 0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f}, {fNormal[4]}, { 1.0f, 1.0f}},

		// +Y��				 		  
		{{-0.5f, 0.5f, 0.5f}, {fNormal[5]}, { 0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.5f}, {fNormal[5]}, { 1.0f, 0.0f}},
		{{-0.5f, 0.5f,-0.5f}, {fNormal[5]}, { 0.0f, 1.0f}},
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[5]}, { 1.0f, 1.0f}},
	};
	int idx[] = {
		0,1,2,1,3,2,
		4,5,6,5,7,6,
		8,9,10,9,11,10,
		12,13,14,13,15,14,
		16,17,18,17,19,18,
		20,21,22,21,23,22,

	};

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;					// ���_�f�[�^
	desc.vtxCount				= _countof(vtx);		// ���_�̐�(_countof�͔z��̐��𐔂���)
	desc.vtxSize				= sizeof(T_Vertex);		// ���_�������̃f�[�^�T�C�Y
	desc.pIdx					= idx;					// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount				= _countof(idx);		// �C���f�b�N�X�̐�
	desc.idxSize				= sizeof(int);			// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// �g�|���W�[�̐ݒ�(�O�p�`���X�g)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);	// ���b�V���o�b�t�@�̍쐬
}

/* ========================================
	���b�V���쐬(���C���t���[��)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(���C���t���[��)
=========================================== */
void ShapeBox::MakeMeshWire()
{
	// �e�ʂ̖@��
	Vector3<float> fNormal = { 0.0f, 0.0f, 0.0f };	// �@���͎g��Ȃ��̂�0�ł���

	// �e���_�̍��W�Euv�f�[�^�̍쐬
	T_Vertex vtx[] = {

		// ���_�̏��Ԃ�Z���ɂȂ��Ă���
		// �㉺ ----------------------------------
		// +Y��				 		  
		{{-0.5f, 0.5f, 0.5f}, fNormal, { 0.0f, 0.0f}},	// ���� 0
		{{ 0.5f, 0.5f, 0.5f}, fNormal, { 1.0f, 0.0f}},	// �E�� 1
		{{-0.5f, 0.5f,-0.5f}, fNormal, { 0.0f, 1.0f}},	// ���� 2
		{{ 0.5f, 0.5f,-0.5f}, fNormal, { 1.0f, 1.0f}},	// �E�� 3

		// -Y��
		{{-0.5f,-0.5f,-0.5f}, fNormal, { 0.0f, 0.0f}},	// ���� 4
		{{ 0.5f,-0.5f,-0.5f}, fNormal, { 1.0f, 0.0f}},	// �E�� 5
		{{-0.5f,-0.5f, 0.5f}, fNormal, { 0.0f, 1.0f}},	// ���� 6
		{{ 0.5f,-0.5f, 0.5f}, fNormal, { 1.0f, 1.0f}},	// �E�� 7
	};


	int idx[] = {
		0,1, 1,3, 3,2, 2,0,		// ���
		4,5, 5,7, 7,6, 6,4,		// ����
		0,6, 1,7, 2,4, 3,5,		// ��ʂƉ��ʂ����Ԑ�
	};

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;					// ���_�f�[�^
	desc.vtxCount				= _countof(vtx);		// ���_�̐�(_countof�͔z��̐��𐔂���)
	desc.vtxSize				= sizeof(T_Vertex);		// ���_�������̃f�[�^�T�C�Y
	desc.pIdx					= idx;					// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount				= _countof(idx);		// �C���f�b�N�X�̐�
	desc.idxSize				= sizeof(int);			// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// �g�|���W�[�̐ݒ�(�O�p�`���X�g)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);	// ���b�V���o�b�t�@�̍쐬
}
