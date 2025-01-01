/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�N���A�����o)�p�w�b�_
	------------------------------------
	�����F�X�e�[�W���N���A�����ۂ̃J�������o
		�@�v���C���[�ɋ߂Â��ăY�[���A�E�g����(�}���I64��)
	------------------------------------
	ObjectCameraGameClear.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectCamera.h"

// =============== �N���X�̒�` =====================
class ObjectCameraGameClear :
    public ObjectCamera
{
public:
	ObjectCameraGameClear(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectCameraGameClear)	// �I�u�W�F�N�g�̎��ID�擾�֐�

private:
	void EventMoveNearPlayer();
	void EventZoomOutStart();
private:
	ObjectBase* m_pTarget;	// �^�[�Q�b�g(�v���C���[

	float	m_fStandByTimeCnt;	// �ҋ@���ԃJ�E���g
	bool	m_bZoomOutStart;	// �Y�[���A�E�g�J�n
};

