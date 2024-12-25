/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(�U��)�pcpp
	------------------------------------
	ComponentEnemyAttackBase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentEnemyAttackBase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

#include "ObjectBase.h"

const float DEFAULT_ATK_INTERVAL = 2.0f;


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�҃I�u�W�F�N�g
========================================= */
ComponentEnemyAttackBase::ComponentEnemyAttackBase(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyAttack)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fAtkInvTime(DEFAULT_ATK_INTERVAL)
	, m_fAtkInvCnt(0.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ComponentEnemyAttackBase::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}


/* ========================================
	�Q�b�^�[(�U���Ԋu����)�֐�
	-------------------------------------
	�ߒl�Ffloat		�U���Ԋu����
=========================================== */
float ComponentEnemyAttackBase::GetAtkIntervalTime()
{
	return m_fAtkInvTime;
}


/* ========================================
	�Z�b�^�[(�U���Ԋu����)�֐�
	-------------------------------------
	�����Ffloat		�U���Ԋu����
=========================================== */
void ComponentEnemyAttackBase::SetAtkIntervalTime(float fInterval)
{
	m_fAtkInvTime = fInterval;
}
