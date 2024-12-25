/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g�pcpp
	------------------------------------
	ComponentEnemyMove.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

#include "ComponentEnemyState.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentEnemyMoveBase.h"
#include "ComponentEnemyAttackBase.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ComponentEnemyState::ComponentEnemyState(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyState)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_eEnemyState(ES_MOVE)
	, m_pCompEnemyMove(nullptr)
	, m_pCompEnemyAttack(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================== */
void ComponentEnemyState::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================== */
void ComponentEnemyState::Update()
{
	if (m_pCompEnemyMove)
	{
		if (m_eEnemyState == E_EnemyState::ES_MOVE)
			m_pCompEnemyMove->SetActive(true);
		else
			m_pCompEnemyMove->SetActive(false);
	}
	if (m_pCompEnemyAttack)
	{
		if (m_eEnemyState == E_EnemyState::ES_ATTACK)
			m_pCompEnemyAttack->SetActive(true);
		else
			m_pCompEnemyAttack->SetActive(false);
	}


}


/* ========================================
	�Q�b�^�[(�G�L�������)�֐�
	-------------------------------------
	�ߒl�F�G�L�������
=========================================== */
ComponentEnemyState::E_EnemyState ComponentEnemyState::GetEnemyState()
{
	return m_eEnemyState;
}


/* ========================================
	�Z�b�^�[(�G�L�������)�֐�
	-------------------------------------
	����1�F�G�L�������
=========================================== */
void ComponentEnemyState::SetEnemyState(E_EnemyState eState)
{
	m_eEnemyState = eState;
}

/* ========================================
	�Z�b�^�[(�ړ��R���|�[�l���g)�֐�
	-------------------------------------
	����1�F�ړ��R���|�[�l���g
=========================================== */
void ComponentEnemyState::SetMoveComponent(ComponentEnemyMoveBase* pMove)
{
	m_pCompEnemyMove = pMove;
}

/* ========================================
	�Z�b�^�[(�U���R���|�[�l���g)�֐�
	-------------------------------------
	����1�F�U���R���|�[�l���g
=========================================== */
void ComponentEnemyState::SetAttackComponent(ComponentEnemyAttackBase* pAttack)
{
	m_pCompEnemyAttack = pAttack;
}