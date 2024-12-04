/* ========================================
	DX22Base/
	------------------------------------
	�J�����I�u�W�F�N�g�pcpp
	------------------------------------
	ObjectCamera.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectCamera.h"
#include "ComponentCameraBase.h"
#include "ComponentTransform.h"
#include "CameraManager.h"
#include "ComponentCameraDebug.h"
#include "CameraManager.h"
#include "ComponentBillboard.h"
#include "TextureManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1: �V�[���N���X�̃|�C���^
=========================================== */
ObjectCamera::ObjectCamera(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCameraBase(nullptr)
	, m_pTransform(nullptr)
	, m_bActive(false)
{

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�R���|�[�l���g�̒ǉ�
=========================================== */
void ObjectCamera::InitLocal()
{
	// �R���|�[�l���g�̒ǉ�
	m_pCameraBase = AddComponent<ComponentCameraBase>();	// �J�����̊�{�@�\
	m_pTransform = GetComponent<ComponentTransform>();		// ���W�A��]�A�g��k��(���߂���Z�b�g�ς�)

	m_pTransform->SetLocalPosition(Vector3(0.0f, 1.0f, -5.0f));

	AddComponent<ComponentCameraDebug>();	// �f�o�b�O�J����

	CAMERA_MNG_INST.AddCamera(this);	// �J�����}�l�[�W���ɃJ������ǉ�


	AddComponent<ComponentBillboard>();	// �r���{�[�h
	GetComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::CAMERA_ICON));	// �e�N�X�`��
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�J�����̍폜
=========================================== */
void ObjectCamera::UninitLocal()
{
	CAMERA_MNG_INST.RemoveCamera(this);	// �J�����}�l�[�W������J�������폜
}


/* ========================================
	�Q�b�^�[(�r���[�s��)�֐�
	-------------------------------------
	�ߒl�F�r���[�s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetViewMatrix()
{
	return m_pCameraBase->GetViewMatrix();
}

/* ========================================
	�Q�b�^�[(�r���[�s��̋t�s��)�֐�
	-------------------------------------
	�ߒl�F�r���[�s��̋t�s��
=========================================== */
DirectX::XMMATRIX ObjectCamera::GetInvViewMatrix()
{
	return m_pCameraBase->GetInvViewMatrix();
}

/* ========================================
	�Q�b�^�[(�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�ߒl�F�v���W�F�N�V�����s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrix()
{
#ifdef _DEBUG
	// �f�o�b�O���j���[�̕��s���e�t���O��ON�̏ꍇ
	if (WIN_CAMERA_INFO["Orthographic"].GetBool())
	{
		return m_pCameraBase->GetProjectionMatrixOrtho();
	}
	else
	{
		return m_pCameraBase->GetProjectionMatrix();
	}

#else
	return m_pCameraBase->GetProjectionMatrix();

#endif // _DEBUG

}

/* ========================================
	�Q�b�^�[(2D�p�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�ߒl�F2D�p�v���W�F�N�V�����s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixUI()
{
	return m_pCameraBase->GetProjectionMatrixUI();
}

/* ========================================
	�Q�b�^�[(���s���e�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�ߒl�F���s���e�v���W�F�N�V�����s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixOrtho()
{
	return m_pCameraBase->GetProjectionMatrixOrtho();
}

/* ========================================
	�Q�b�^�[(�L���t���O)�֐�
	-------------------------------------
	�ߒl�F�L���t���O���
=========================================== */
bool ObjectCamera::GetActive()
{
	return m_bActive;
}

/* ========================================
	�Z�b�^�[(�L���t���O)�֐�
	-------------------------------------
	����1�F�L���t���O
=========================================== */
void ObjectCamera::SetActive(bool bActive)
{
	m_bActive = bActive;
}
