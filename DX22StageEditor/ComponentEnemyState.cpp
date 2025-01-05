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
#include "ComponentModelAnime.h"
#include "ModelAnimeManager.h"

// =============== �萔��` =====================
const float	ANIME_SPEED_IDLE = 1.0f;	// �ҋ@�A�j���[�V�������x
const float	ANIME_SPEED_WALK = 1.0f;	// �ړ��A�j���[�V�������x

const float ANIME_BLEND_TIME = 0.2f;	// �A�j���[�V�����u�����h����

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
	, m_pCompEnemyMove(nullptr)
	, m_pCompEnemyAttack(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_eEnemyState(ES_MOVE)
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
	m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();
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

	// �A�j���[�V�����̍Đ�
	MoveAnime();
}


/* ========================================
	�A�j���[�V�����Đ��֐�
	-------------------------------------
	���e�F�ړ��A�j���[�V�����̍Đ�
======================================== */
void ComponentEnemyState::MoveAnime()
{
	float fSpeedX = m_pCompRigidbody->GetVelocity().x;
	float fSpeedZ = m_pCompRigidbody->GetVelocity().z;

	// �ړ���
	if (fSpeedX != 0.0f || fSpeedZ != 0.0f)
	{
		// ���ɍĐ����łȂ��ꍇ
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK, ANIME_BLEND_TIME, true, ANIME_SPEED_WALK);
	}
	else
	{
		// ���ɍĐ����łȂ��ꍇ
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE, ANIME_BLEND_TIME, true, ANIME_SPEED_IDLE);
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

