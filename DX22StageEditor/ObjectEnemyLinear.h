/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L�������ړ�)�p�w�b�_
	------------------------------------
	�����F���ړ�����G�L�����N�^�[
	------------------------------------
	ObjectEnemyLinear.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectEnemy.h"

// =============== �O���錾 =======================
class ComponentEnemyMoveLinear;

// =============== �N���X��` =====================
class ObjectEnemyLinear :
    public ObjectEnemy
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveDataEnemyLinear
	{
		int nWayPointNum;	// �ړ����W��
		bool bIsReverse;	// �t���t���O
	};

public:
	ObjectEnemyLinear(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyLinear)	// �I�u�W�F�N�g�̎��ID�擾�֐�
private:
	ComponentEnemyMoveLinear* m_pCompEnemyMoveLinear;

};

