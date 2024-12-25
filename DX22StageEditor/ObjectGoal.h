/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�S�[��)�p�w�b�_
	------------------------------------
	�����F�X�e�[�W�̃S�[��
		�@�v���C���[���ڐG����ƃN���A
	------------------------------------
	ObjectGoal.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �N���X��` =====================
class ObjectGoal :
    public ObjectBase
{
public:
	ObjectGoal(SceneBase* pScene);

	void InitLocal();
	void UpdateLocal();

	void OnCollisionEnter(ObjectBase* pHit) override;

	// �Q�b�^�[
	bool GetIsGoal();

	// �Z�b�^�[
	void SetIsGoal(bool bIsGoal);

	DEFINE_OBJECT_TYPE(ObjectGoal)
private:
	bool m_bIsGoal;		// true: �S�[�� / false: ���S�[��
};

