/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����U���R���|�[�l���g(�ːi)�pcpp
	------------------------------------
	ComponentEnemyAttackCharge.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentEnemyAttackCharge.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ShapeSphere.h"
#include "ColorVec3.h"
#include <format>

// =============== �萔 =======================
const float LIMIT_DIST_SQ			= 0.1f * 0.1f;	// ���B���苗��
const float DEFAULT_CHARGE_DIST		= 5.0f;			// �ːi����
const float DEFAULT_CHARGE_SPEED	= 10.0f;		// �ːi���x

// �ړI���W�`��
const Vector3<float> TARGET_COLOR			= ColorVec3::BLUE;
const Vector3<float> TARGET_SPHERE_SCALE	= { 0.2f, 0.2f, 0.2f };

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�҃I�u�W�F�N�g
======================================== */
ComponentEnemyAttackCharge::ComponentEnemyAttackCharge(ObjectBase* pOwner)
	:ComponentEnemyAttackBase(pOwner)
	, m_fChargeDist(DEFAULT_CHARGE_DIST)
	, m_fChargeSpeed(DEFAULT_CHARGE_SPEED)
	, m_eChargeState(CS_IDLE)
	, m_pTarget(nullptr)
	, m_vChargeEndPos(Vector3<float>::Zero())
#ifdef _DEBUG
	, m_bDispChargeEndSphere(true)
#else
	, m_bDispChargeEndSphere(false)
#endif // DEBUG
	, m_pChargeEndSphere(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ComponentEnemyAttackCharge::Init()
{
	ComponentEnemyAttackBase::Init();

	m_pTarget = m_pOwnerObj->GetOwnerScene()->FindSceneObject("Player");

	// �ːi�I�����W�p�X�t�B�A����
	m_pChargeEndSphere = std::make_unique<ShapeSphere>();
	m_pChargeEndSphere->SetPosition(Vector3<float>::Zero());
	m_pChargeEndSphere->SetScale(TARGET_SPHERE_SCALE);
	m_pChargeEndSphere->SetColor(TARGET_COLOR);
	m_pChargeEndSphere->SetDrawMode(ShapeSphere::E_DrawMode::DRAW_NORMAL);

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ComponentEnemyAttackCharge::Update()
{
	if (!m_pTarget)
	{
		m_pTarget = m_pOwnerObj->GetOwnerScene()->FindSceneObject("Player");
		return;
	}

	switch (m_eChargeState)
	{
	case E_ChargeState::CS_IDLE:
		Idle();
		break;
	case E_ChargeState::CS_ATTACK:
		Attack();
		break;
	}
#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyAttackCharge"))
	{
		WIN_OBJ_INFO["EnemyAttackCharge"]["ChargeState"].SetText(
			(m_eChargeState == E_ChargeState::CS_IDLE) ? "Idle" : "Attack");
	}
#endif // _DEBUG

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================= */
void ComponentEnemyAttackCharge::Draw()
{
	if (m_bDispChargeEndSphere)
	{
		m_pChargeEndSphere->SetPosition(m_vChargeEndPos);
		m_pChargeEndSphere->Draw();
	}
}

/* ========================================
	�ҋ@�֐�
	-------------------------------------
	���e�F�ːi�U���̑ҋ@����
========================================= */
void ComponentEnemyAttackCharge::Idle()
{
	// �v���C���[���W���擾
	Vector3<float> vTargetPos = m_pTarget->GetTransform()->GetWorldPosition();

	// ������ύX
	Vector3<float> vLookPos = vTargetPos;
	vLookPos.y				= m_pCompTransform->GetWorldPosition().y;	// �����͕ύX���Ȃ�
	m_pCompTransform->LookAt(vLookPos);

	m_fAtkInvCnt += DELTA_TIME;
	if (m_fAtkInvCnt >= m_fAtkInvTime)
	{
		m_fAtkInvCnt	= 0.0f;			// �J�E���g���Z�b�g

		// �ːi�I�����W��ݒ�
		Vector3<float> vDir = vTargetPos - m_pCompTransform->GetWorldPosition();
		vDir.y = 0.0f;	// �����͕ύX���Ȃ�
		m_vChargeEndPos		= m_pCompTransform->GetWorldPosition() + (vDir.GetNormalize() * m_fChargeDist);

		m_eChargeState	= CS_ATTACK;	// �U����ԂɑJ��
	}
}

/* ========================================
	�ːi�֐�
	-------------------------------------
	���e�F�ːi�U������
========================================= */
void ComponentEnemyAttackCharge::Attack()
{
	// �ړI���W�܂ł̃x�N�g�������߂�
	Vector3<float> vDir = m_vChargeEndPos - m_pCompTransform->GetWorldPosition();

	// �ړ�
	m_pCompRigidbody->SetVelocity(vDir.GetNormalize() * m_fChargeSpeed);

	// �ːi�I�����W�ɓ��B������
	if (vDir.LengthSq() <= LIMIT_DIST_SQ)
	{
		m_pCompRigidbody->SetVelocity(Vector3<float>::Zero());	// �ړ���~
		m_eChargeState = CS_IDLE;	// �ҋ@��ԂɑJ��
		return;
	}

}

/* ========================================
	�Q�b�^�[(�ːi���)�֐�
	-------------------------------------
	�ߒl�FE_ChargeState		�ːi���
========================================== */
ComponentEnemyAttackCharge::E_ChargeState ComponentEnemyAttackCharge::GetChargeState()
{
	return m_eChargeState;
}

/* ========================================
	�Q�b�^�[(�ːi����)�֐�
	-------------------------------------
	�ߒl�Ffloat		�ːi����
========================================== */
float ComponentEnemyAttackCharge::GetChargeDist()
{
	return m_fChargeDist;
}

/* ========================================
	�Q�b�^�[(�ːi���x)�֐�
	-------------------------------------
	�ߒl�Ffloat		�ːi���x
========================================== */
float ComponentEnemyAttackCharge::GetChargeSpeed()
{
	return m_fChargeSpeed;
}

/* ========================================
	�Z�b�^�[(�ːi���)�֐�
	-------------------------------------
	�����FE_ChargeState		�ːi���
========================================== */
void ComponentEnemyAttackCharge::SetChargeState(E_ChargeState eState)
{
	m_eChargeState = eState;
}

/* ========================================
	�Z�b�^�[(�ːi����)�֐�
	-------------------------------------
	�����Ffloat		�ːi����
========================================== */
void ComponentEnemyAttackCharge::SetChargeDist(float fDist)
{
	m_fChargeDist = fDist;
}

/* ========================================
	�Z�b�^�[(�ːi���x)�֐�
	-------------------------------------
	�����Ffloat		�ːi���x
========================================== */
void ComponentEnemyAttackCharge::SetChargeSpeed(float fSpeed)
{
	m_fChargeSpeed = fSpeed;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentEnemyAttackCharge::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* EnemyAttackCharge = Item::CreateGroup("EnemyAttackCharge");

	EnemyAttackCharge->AddGroupItem(Item::CreateBind("AtkInvTime", Item::Kind::Float, &m_fAtkInvTime));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("AtkInvCnt", Item::Kind::Float, &m_fAtkInvCnt));
	EnemyAttackCharge->AddGroupItem(Item::CreateValue("ChargeState", Item::Kind::Text));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeDist", Item::Kind::Float, &m_fChargeDist));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeSpeed", Item::Kind::Float, &m_fChargeSpeed));

	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeEndPos", Item::Kind::Vector, &m_vChargeEndPos));

	window.AddItem(EnemyAttackCharge);

	WIN_OBJ_INFO["EnemyAttackCharge"]["ChargeState"].SetText(
		(m_eChargeState == E_ChargeState::CS_IDLE) ? "Idle" : "Attack");

}

#endif // _DEBUG