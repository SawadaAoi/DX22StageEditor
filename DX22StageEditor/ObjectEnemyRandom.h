/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L���������_���ړ�)�p�w�b�_
	------------------------------------
	�����F�����_���ړ�����G�L�����N�^�[
	------------------------------------
	ObjectEnemyRandom.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectEnemy.h"

// =============== �O���錾 =======================
class ComponentEnemyMoveRandom;

// =============== �N���X��` =====================
class ObjectEnemyRandom :
    public ObjectEnemy
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveDataEnemyRandom
	{
		float fMoveSpeed;				// �ړ����x
		float fTargetSwitchTime;		// �^�[�Q�b�g�؂�ւ�����
		Vector3<float> vMoveLimitCenter;	// �ړ��͈͒��S���W
		Vector3<float> vMoveLimitRF;	// �ړ��͈�(�E��)
		Vector3<float> vMoveLimitLF;	// �ړ��͈�(����)
		Vector3<float> vMoveLimitLB;	// �ړ��͈�(����)
		Vector3<float> vMoveLimitRB;	// �ړ��͈�(�E��)
	};
public:
	ObjectEnemyRandom(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyRandom)	// �I�u�W�F�N�g�̎��ID�擾�֐�
private:
	ComponentEnemyMoveRandom* m_pCompEnemyMoveRandom;
};

