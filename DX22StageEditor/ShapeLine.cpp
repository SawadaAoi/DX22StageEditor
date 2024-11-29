/* ========================================
	DX22Base/
	------------------------------------
	���`��pcpp
	------------------------------------
	ShapeLine.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapeLine.h"
#include "ShaderManager.h"	// ���O�ɓǂݍ��܂ꂽ�V�F�[�_�[���g�p�����
#include "DirectXManager.h"	// �����_�[�^�[�Q�b�g�̐ݒ���s����
#include "CameraManager.h"	// �J�����̏����擾�����
#include "Quaternion.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
ShapeLine::ShapeLine()
	: m_vPos(0.0f, 0.0f, 0.0f)
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(0)
	, m_pVertex(nullptr)
	, m_WVP{}
{
}

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������(�����̐���`�悷�鎞�Ɏg�p)
	-------------------------------------
	����1�FDrawNum	�`�悷����̐�
=========================================== */
ShapeLine::ShapeLine(int DrawNum)
	: m_vPos(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation()
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(DrawNum)
	, m_pVertex(nullptr)
	, m_WVP{}

{
	m_pVS = GET_VS_DATA(VS_KEY::VS_LINE);	// ���_�V�F�[�_�[�ǂݍ���
	m_pPS = GET_PS_DATA(PS_KEY::PS_LINE);	// �s�N�Z���V�F�[�_�[�ǂݍ���


	MakeLine();
}

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������(1�{�̐���`�悷�鎞�Ɏg�p)
	-------------------------------------
	����1�Fstart	�n�_
	����2�Fend		�I�_
	����3�Fcolor	�F
=========================================== */
ShapeLine::ShapeLine(Vector3<float> start, Vector3<float> end, Vector3<float> color)
	: ShapeLine(1)	// 1�{�̐���`�悷��̂ŁA�ő吔��1
{
	AddLine(start, end, color);	// ����ǉ�
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
ShapeLine::~ShapeLine()
{
	SAFE_DELETE(m_pMeshBuffer);
	SAFE_DELETE(m_pVertex);
}

/* ========================================
	�R�s�[�R���X�g���N�^
	-------------------------------------
	���e�F�R�s�[�R���X�g���N�^
=========================================== */
ShapeLine::ShapeLine(const ShapeLine& other)
	: m_vPos{ 0.0f, 0.0f, 0.0f }
	, m_vScale{ 1.0f, 1.0f, 1.0f }
	, m_qRotation()
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(0)
	, m_pVertex(nullptr)
	, m_WVP{}
{
	if (this == &other) return;

	int nVertexNum = other.m_nMaxLineCnt * 2;	// 1�{2���_

	m_pMeshBuffer	= new MeshBuffer(*other.m_pMeshBuffer);
	m_pVertex		= new T_LineVertex[nVertexNum];
	memcpy_s(m_pVertex, sizeof(T_LineVertex) * nVertexNum, other.m_pVertex, sizeof(T_LineVertex) * nVertexNum);

	m_pPS			= other.m_pPS;
	m_pVS			= other.m_pVS;
	m_nLineCnt		= other.m_nLineCnt;
	m_nMaxLineCnt	= other.m_nMaxLineCnt;
	m_vPos			= other.m_vPos;
	m_vScale		= other.m_vScale;
	m_qRotation		= other.m_qRotation;
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ShapeLine::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// �[�x�o�b�t�@�̗L����

	SetWVP();	// WVP�s��̐ݒ�
	// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)�̏�������
	m_pVS->WriteBuffer(0, m_WVP);

	// �V�F�[�_�[�̃o�C���h(�V�F�[�_�[�̐ݒ�� GPU �ɑ���)
	m_pVS->Bind();
	m_pPS->Bind();
	
	// �`��
	m_pMeshBuffer->Draw();

}

/* ========================================
	���ǉ��֐�
	-------------------------------------
	���e�F�`�悷�����ǉ�����
	-------------------------------------
	����1�Fstart	�n�_
	����2�Fend		�I�_
	����3�Fcolor	�F
=========================================== */
void ShapeLine::AddLine(Vector3<float> start, Vector3<float> end, Vector3<float> color)
{
	// �ő吔�𒴂�����`�悵�Ȃ�
	if (m_nLineCnt < m_nMaxLineCnt)
	{
		T_LineVertex* pVtx = reinterpret_cast<T_LineVertex*>(m_pVertex);	// ���݂̒��_�f�[�^���擾
		int nLastIndex = m_nLineCnt * 2;	// ���_�f�[�^�̈�Ԍ��̈ʒu(���݂̐��� * 2(1�{2���_�̈�))
		
		// ���_�f�[�^��ǉ�
		pVtx[nLastIndex + 0] = { start.x, start.y, start.z, color.x, color.y, color.z, 1.0f };	// �n�_
		pVtx[nLastIndex + 1] = {   end.x,   end.y,   end.z, color.x, color.y, color.z, 1.0f };	// �I�_

		m_nLineCnt++;	// ���̐��𑝂₷

		// pVtx�̃|�C���^��m_pMeshBuffer�̃|�C���^�͓����A�h���X���w���Ă���̂ŁA
		// pVtx�Ƀf�[�^���������ނ�m_pMeshBuffer�ɂ��f�[�^���������܂��
	}

	// ���_�f�[�^���X�V
	m_pMeshBuffer->WriteVertexData(m_pVertex);	

}

/* ========================================
	���X�V�֐�
	-------------------------------------
	���e�F�`�悷������X�V����
	-------------------------------------
	����1�FnLineNum	�X�V���������̔ԍ�(1�{�ڂ�1)
	����2�Fstart	�n�_
	����3�Fend		�I�_
	����4�Fcolor	�F
=========================================== */
void ShapeLine::UpdateLine(int nLineNum, Vector3<float> start, Vector3<float> end, Vector3<float> color)
{
	if (nLineNum <= 0) return;	// ���̔ԍ���0�ȉ��̏ꍇ�͍X�V���Ȃ�

	if (nLineNum <= m_nMaxLineCnt)
	{
		T_LineVertex* pVtx = reinterpret_cast<T_LineVertex*>(m_pVertex);	// ���݂̒��_�f�[�^���擾

		// �X�V���������̒��_�f�[�^�̈ʒu(�X�V���������� * 2(1�{2���_�̈�))
		// ���_�f�[�^��0����n�܂�̂ŁA-1����
		int nLastIndex = (nLineNum - 1) * 2;	

		// ���_�f�[�^���㏑��
		pVtx[nLastIndex + 0] = { start.x, start.y, start.z, color.x, color.y, color.z, 1.0f };	// �n�_
		pVtx[nLastIndex + 1] = { end.x, end.y, end.z, color.x, color.y, color.z, 1.0f };		// �I�_

		// ���_�f�[�^���X�V
		m_pMeshBuffer->WriteVertexData(m_pVertex);
	}
}



/* ========================================
	���쐬�֐�
	-------------------------------------
	���e�F�����쐬����
=========================================== */
void ShapeLine::MakeLine()
{
	m_pVertex		= new T_LineVertex[m_nMaxLineCnt * 2];	// 1�{2���_
	m_nLineCnt		= 0;	// �`�搔������

	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= m_pVertex;
	desc.vtxCount				= m_nMaxLineCnt * 2;					// ���ƂŒǉ�����\��������̂ōő吔��ݒ�(1�{2���_)
	desc.vtxSize				= sizeof(T_LineVertex);
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// �g�|���W�[�̐ݒ�(���C�����X�g)
	desc.bWrite					= true;									// �ǉ�����̂ŏ������݉\
	m_pMeshBuffer				= new MeshBuffer(desc);
}

/* ========================================
	WVP�s��ݒ�֐�
	-------------------------------------
	���e�F���[���h�A�r���[�A�v���W�F�N�V�����s���ݒ肷��
=========================================== */
void ShapeLine::SetWVP()
{
	// ���[���h�s��̌v�Z
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4�ɕϊ����Ċi�[
		DirectX::XMMatrixTranspose(													// �]�n
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// �X�P�[��
			m_qRotation.ToDirectXMatrix() *											// ��]
			DirectX::XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z)				// �ړ�
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();
}


/* ========================================
	�Z�b�^�[(���W)�֐�
	-------------------------------------
	�����Fpos	���W
=========================================== */
void ShapeLine::SetPos(Vector3<float> pos)
{
	m_vPos = pos;
}

/* ========================================
	�Z�b�^�[(��])�֐�
	-------------------------------------
	�����Frot	��]
=========================================== */
void ShapeLine::SetRotation(Quaternion rot)
{
	m_qRotation = rot;
}

/* ========================================
	�Z�b�^�[(�X�P�[��)�֐�
	-------------------------------------
	�����Fscale	�X�P�[��
=========================================== */
void ShapeLine::SetScale(Vector3<float> scale)
{
	m_vScale = scale;
}
