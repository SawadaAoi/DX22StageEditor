/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����U���R���|�[�l���g(���)�p�w�b�_
	------------------------------------
	�����F�G�L�����̍U�������̊��N���X
	------------------------------------
	ComponentEnemyAttackBase.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== �N���X��` ===================
class ComponentEnemyAttackBase :
    public ComponentBase
{
public:
	ComponentEnemyAttackBase(ObjectBase* pOwner);
	void Init() override;

	// �Q�b�^�[
	float GetAtkIntervalTime();

	// �Z�b�^�[
	void SetAtkIntervalTime(float fInterval);

	DEFINE_COMPONENT_TYPE
protected:
	ComponentTransform* m_pCompTransform;
	ComponentRigidbody* m_pCompRigidbody;

	float m_fAtkInvTime;	// �U���Ԋu
	float m_fAtkInvCnt;	// �U���Ԋu�J�E���g
};

