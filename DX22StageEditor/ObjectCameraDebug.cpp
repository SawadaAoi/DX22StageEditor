/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�f�o�b�O)�pcpp
	------------------------------------
	ObjectCameraDebug.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectCameraDebug.h"
#include "ComponentCameraDebug.h"
#include "ComponentTransform.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1: �V�[���N���X�̃|�C���^
=========================================== */
ObjectCameraDebug::ObjectCameraDebug(SceneBase* pScene)
	: ObjectCamera(pScene)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectCameraDebug::InitLocal()
{
	ObjectCamera::InitLocal();										// ���N���X�̏������֐����Ă�
	m_pCompTransform->SetPosition(Vector3(0.0f, 1.0f, -5.0f));	// �J�����ʒu��ݒ�
	AddComponent<ComponentCameraDebug>();							// �f�o�b�O�@�\��ǉ�
}
