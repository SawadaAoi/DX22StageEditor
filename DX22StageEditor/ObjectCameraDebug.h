/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�f�o�b�O)�p�w�b�_
	------------------------------------
	�����F�}�E�X�A�L�[�Ŏ��R�ɓ�������J����
	------------------------------------
	ObjectCameraDebug.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectCamera.h"

// =============== �N���X�̒�` =====================
class ObjectCameraDebug :
    public ObjectCamera
{
public:
	ObjectCameraDebug(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectCameraDebug)	// �I�u�W�F�N�g�̎��ID�擾�֐�
};

