/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(���)�p�w�b�_
	------------------------------------
	�����F�G�L�����̈ړ������̊��N���X
	------------------------------------
	ComponentEnemyMoveBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== �N���X��` =====================
class ComponentEnemyMoveBase :
    public ComponentBase
{
public:
	ComponentEnemyMoveBase(ObjectBase* pOwner);
	void Init() override;

	// �Q�b�^�[
	float GetMoveSpeed();

	// �Z�b�^�[
	void SetMoveSpeed(float fSpeed);

	DEFINE_COMPONENT_TYPE
protected:
	ComponentTransform* m_pCompTransform;
	ComponentRigidbody* m_pCompRigidbody;

	float			m_fMoveSpeed;
};

