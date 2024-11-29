/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��pcpp
	------------------------------------
	Shape.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapeBase.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "DirectXManager.h"
#include "TextureManager.h"	

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F����
=========================================== */
ShapeBase::ShapeBase()
	: m_pMeshBuffer{ nullptr, nullptr }
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_WVP()
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation({0.0f,0.0f,0.0f}, 1.0f)
	, m_fColor(1.0f, 1.0f, 1.0f)
	, m_eDrawMode(E_DrawMode::DRAW_NORMAL)
	, m_pTexture(nullptr)
	, m_bIsTex(false)
{
	// ���_�V�F�[�_�[�ǂݍ���
	m_pVS = GET_VS_DATA(VS_KEY::VS_SHAPE);
	// �s�N�Z���V�F�[�_�[�ǂݍ���
	m_pPS = GET_PS_DATA(PS_KEY::PS_SHAPE);

	//m_pTexture = GET_TEXTURE_DATA(TEX_KEY::TEST);	// �f�t�H���g�e�N�X�`��
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
ShapeBase::~ShapeBase()
{
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_NORMAL]);
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_WIRE_FRAME]);
}


/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ShapeBase::Draw()
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
	m_pPS->SetTexture(0, m_pTexture);

	// �V�F�[�_�[�̃o�C���h(�V�F�[�_�[�̐ݒ�� GPU �ɑ���)
	m_pVS->Bind();
	m_pPS->Bind();
	
	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@��GPU�ɐݒ肵�A�`��
	m_pMeshBuffer[m_eDrawMode]->Draw();
}


/* ========================================
	WVP�s��ݒ�֐�
	-------------------------------------
	���e�F���[���h�A�r���[�A�v���W�F�N�V�����s���ݒ肷��
=========================================== */
void ShapeBase::SetWVPMatrix()
{
	// ���[���h�s��̌v�Z
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4�ɕϊ����Ċi�[
		DirectX::XMMatrixTranspose(													// �]�n
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// �g��k��
			m_qRotation.ToDirectXMatrix() *											// �N�H�[�^�j�I����]
			DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z)				// �ړ�
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

}


/* ========================================
	�Q�b�^�[(���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float> fPos
=========================================== */
Vector3<float> ShapeBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	�Q�b�^�[(�T�C�Y)�֐�
	-------------------------------------
	�ߒl�FVector3<float> fSize
=========================================== */
Vector3<float> ShapeBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	�Q�b�^�[(��])�֐�
	-------------------------------------
	�ߒl�FQuaternion m_fRotation
=========================================== */
Quaternion ShapeBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	�Q�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�ߒl�FE_DrawMode eMode
=========================================== */
Vector3<float> ShapeBase::GetColor()
{
	return m_fColor;
}

/* ========================================
	�Q�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�ߒl�FE_DrawMode eMode
=========================================== */
ShapeBase::E_DrawMode ShapeBase::GetDrawMode()
{
	return m_eDrawMode;
}



/* ========================================
	�Z�b�^�[(���W)�֐�
	-------------------------------------
	�����FVector3<float> fPos
=========================================== */
void ShapeBase::SetPosition(Vector3<float> fPos)
{
	m_vPosition = fPos;
}

/* ========================================
	�Z�b�^�[(�T�C�Y)�֐�
	-------------------------------------
	�����FVector3<float> fSize
=========================================== */
void ShapeBase::SetScale(Vector3<float> fSize)
{
	m_vScale = fSize;
}

/* ========================================
	�Z�b�^�[(��])�֐�
	-------------------------------------
	�����FQuaternion fRot
=========================================== */
void ShapeBase::SetRotation(Quaternion fRot)
{
	m_qRotation = fRot;
}

/* ========================================
	�Z�b�^�[(�F)�֐�
	-------------------------------------
	�����FVector3<float> fColor
=========================================== */
void ShapeBase::SetColor(Vector3<float> fColor)
{
	m_fColor = fColor;
}

/* ========================================
	�Z�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�����FE_DrawMode eMode
=========================================== */
void ShapeBase::SetDrawMode(E_DrawMode eMode)
{
	m_eDrawMode = eMode;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	�����FTexture* pTexture
=========================================== */
void ShapeBase::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`���g�p�t���O)�֐�
	-------------------------------------
	�����Fint bIsTex
=========================================== */
void ShapeBase::SetUseTexture(int bIsTex)
{
	m_bIsTex = bIsTex;
}
