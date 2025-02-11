/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(��b)�pcpp
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
	, m_pCompCameraBase(nullptr)
	, m_bActive(false)
{
	SetTag(E_ObjectTag::Camera);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ObjectCamera::InitLocal()
{
	// �R���|�[�l���g�̒ǉ�
	m_pCompCameraBase = AddComponent<ComponentCameraBase>();	// �J�����̊�{�@�\
	m_pCompTransform = GetComponent<ComponentTransform>();		// ���W�A��]�A�g��k��(���߂���Z�b�g�ς�)

	// �J�����}�l�[�W���ɃJ������ǉ�
	CAMERA_MNG_INST.AddCamera(this);	// InitLocal�Œǉ�����͖̂��O���ݒ肳�ꂽ��ɂ��邽��

#ifdef _DEBUG
	// �f�o�b�O�p�J�����A�C�R��
	AddComponent<ComponentBillboard>();
	GetComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::DEBUG_CAMERA_ICON));
#endif // _DEBUG
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
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
	return m_pCompCameraBase->GetViewMatrix();
}

/* ========================================
	�Q�b�^�[(�r���[�s��̋t�s��)�֐�
	-------------------------------------
	�ߒl�F�r���[�s��̋t�s��
=========================================== */
DirectX::XMMATRIX ObjectCamera::GetInvViewMatrix()
{
	return m_pCompCameraBase->GetInvViewMatrix();
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
		return m_pCompCameraBase->GetProjectionMatrixOrtho();
	}
	else
	{
		return m_pCompCameraBase->GetProjectionMatrix();
	}

#else
	return m_pCompCameraBase->GetProjectionMatrix();

#endif // _DEBUG

}

/* ========================================
	�Q�b�^�[(2D�p�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�ߒl�F2D�p�v���W�F�N�V�����s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixUI()
{
	return m_pCompCameraBase->GetProjectionMatrixUI();
}

/* ========================================
	�Q�b�^�[(���s���e�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�ߒl�F���s���e�v���W�F�N�V�����s��
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixOrtho()
{
	return m_pCompCameraBase->GetProjectionMatrixOrtho();
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
	�Q�b�^�[(�J������{�@�\)�֐�
	-------------------------------------
	�ߒl�F�J������{�@�\
=========================================== */
ComponentCameraBase* ObjectCamera::GetCameraBase()
{
	return m_pCompCameraBase;
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
