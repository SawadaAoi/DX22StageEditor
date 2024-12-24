/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(���)�pcpp
	------------------------------------
	ComponentEnemyMoveBase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveBase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

#include "ObjectBase.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ComponentEnemyMoveBase::ComponentEnemyMoveBase(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyMove)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(1.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================= */
void ComponentEnemyMoveBase::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}


/* ========================================
	�Q�b�^�[(�ړ����x)�֐�
	-------------------------------------
	�ߒl�F�ړ����x
========================================== */
float ComponentEnemyMoveBase::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	�Z�b�^�[(�ړ����x)�֐�
	-------------------------------------
	����1�F�ړ����x
========================================== */
void ComponentEnemyMoveBase::SetMoveSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}