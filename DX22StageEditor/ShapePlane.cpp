/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(�|���S��)�pcpp
	------------------------------------
	ShapePlane.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapePlane.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
ShapePlane::ShapePlane()
{
	MakeMesh();
}

/* ========================================
	���b�V���쐬�֐�
	-------------------------------------
	���e�F���b�V�����쐬����
=========================================== */
void ShapePlane::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}

/* ========================================
	���b�V���쐬(�ʏ�)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(�ʏ�)
=========================================== */
void ShapePlane::MakeMeshNormal()
{
	// �ʂ̖@��(�^��Ɍ�����)
	Vector3<float> vNormal = { 0.0f, 1.0f, 0.0f };	

	// ���_�f�[�^
	T_Vertex vtx[] = {
		{{-0.5f, 0.0f, 0.5f}, vNormal, { 0.0f, 0.0f}},	// ����
		{{ 0.5f, 0.0f, 0.5f}, vNormal, { 1.0f, 0.0f}},	// �E��
		{{-0.5f, 0.0f,-0.5f}, vNormal, { 0.0f, 1.0f}},	// ����
		{{ 0.5f, 0.0f,-0.5f}, vNormal, { 1.0f, 1.0f}},	// �E��
	};
	
	// �C���f�b�N�X�f�[�^
	int idx[] = 
	{
		0,1,2,1,3,2,
	};

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;										// ���_�f�[�^
	desc.vtxCount				= _countof(vtx);							// ���_�̐�(_countof�͔z��̐��𐔂���)
	desc.vtxSize				= sizeof(T_Vertex);							// ���_�������̃f�[�^�T�C�Y
	desc.pIdx					= idx;										// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount				= _countof(idx);							// �C���f�b�N�X�̐�
	desc.idxSize				= sizeof(int);								// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// �g�|���W�[�̐ݒ�(�O�p�`���X�g)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);						// ���b�V���o�b�t�@�̍쐬

}

/* ========================================
	���b�V���쐬(���C���t���[��)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(���C���t���[��)
=========================================== */
void ShapePlane::MakeMeshWire()
{
	
	// �ʂ̖@��(�^��Ɍ�����)
	Vector3<float> vNormal = { 0.0f, 1.0f, 0.0f };

	// ���_�f�[�^
	T_Vertex vtx[] = {
		{{-0.5f, 0.0f, 0.5f}, vNormal, { 0.0f, 0.0f}},
		{{ 0.5f, 0.0f, 0.5f}, vNormal, { 1.0f, 0.0f}},
		{{-0.5f, 0.0f,-0.5f}, vNormal, { 0.0f, 1.0f}},
		{{ 0.5f, 0.0f,-0.5f}, vNormal, { 1.0f, 1.0f}},
	};

	// �C���f�b�N�X�f�[�^
	int idx[] = {
		0,1,1,3,3,2,2,0,1,2,
	};

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc		= {};
	desc.pVtx						= vtx;									// ���_�f�[�^
	desc.vtxCount					= _countof(vtx);						// ���_�̐�(_countof�͔z��̐��𐔂���)
	desc.vtxSize					= sizeof(T_Vertex);						// ���_�������̃f�[�^�T�C�Y
	desc.pIdx						= idx;									// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount					= _countof(idx);						// �C���f�b�N�X�̐�
	desc.idxSize					= sizeof(int);							// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology					= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// �g�|���W�[�̐ݒ�(�O�p�`���X�g)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);					// ���b�V���o�b�t�@�̍쐬

}
