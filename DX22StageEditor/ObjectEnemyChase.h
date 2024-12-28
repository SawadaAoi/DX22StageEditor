/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L�����ǐՈړ�)�p�w�b�_
	------------------------------------
	�����F�ǐՈړ�����G�L�����N�^�[
	------------------------------------
	ObjectEnemyChase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectEnemy.h"

// =============== �O���錾 =======================
class ComponentEnemyMoveChase;

// =============== �N���X��` =====================
class ObjectEnemyChase :
    public ObjectEnemy
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveDataEnemyChase
	{
		float fMoveSpeed;		// �ړ����x
		float fChaseStartDist;	// �ǐՊJ�n����
		float fLimitDistSq;		// �ǐՌ��E����
	};
public:
	ObjectEnemyChase(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyChase)	// �I�u�W�F�N�g�̎��ID�擾�֐�
private:
	ComponentEnemyMoveChase* m_pCompEnemyMoveChase;
};

