/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�v���C���[)�pcpp
	------------------------------------
	ObjectPlayer.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectPlayer.h"
// �R���|�[�l���g
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionOBB.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"

// =============== �萔��` =======================
const int	MAX_HP = 5;	// �v���C���[�̍ő�HP
const float INVINCIBLE_TIME = 2.0f;	// ���G����
const float INV_FLASH_INTERVAL = 0.1f;	// ���G�_�ŊԊu

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectPlayer::ObjectPlayer(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGroundRaycast(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_pCompPlayerController(nullptr)
	, m_nMaxHp(MAX_HP)
	, m_nHp(MAX_HP)
	, m_bInvincible(false)
	, m_fInvCnt(0.0f)
	, m_fInvFlashCnt(0.0f)

{
	SetTag(E_ObjectTag::Player);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ObjectPlayer::InitLocal()
{
	m_pCompTransform =GetComponent<ComponentTransform>();

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pCompGroundRaycast->SetRayLength(0.2f);

	AddComponent<ComponentCollisionOBB>();
	
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetUseGravity(true);
	m_pCompRigidbody->SetGroundDrag(0.9f);

	m_pCompPlayerController = AddComponent<ComponentPlayerController>();

	m_pCompModelAnime = AddComponent<ComponentModelAnime>();

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ObjectPlayer::UpdateLocal()
{
	// �ڒn����
	CheckGround();

	// �_���[�W��̖��G���ԏ���
	if (m_bInvincible)	InvincibleUpdate();
		

}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectPlayer::OnCollisionEnter(ObjectBase* pHit)
{
	// �G�L�����ƐڐG������
	if (pHit->GetTag() == E_ObjectTag::Enemy)
	{
		Damage();
	}
}

/* ========================================
	�ڒn����֐�
	-------------------------------------
	���e�F�n�ʂɐݒu���Ă��邩����
========================================= */
void ObjectPlayer::CheckGround()
{
	// �n�ʂɐݒu���Ă���ꍇ�@���@�W�����v���łȂ��ꍇ
	if (m_pCompGroundRaycast->GetHitFlg() 
		&& m_pCompRigidbody->GetVelocity().y <= 0.0f)
	{
		// �n�ʂɐݒu���Ă���ʒu�Ƀv���C���[��ݒ�
		Vector3<float> vPos = m_pCompTransform->GetWorldPosition();
		vPos.y = m_pCompGroundRaycast->GetHitPos().y + 0.5f;
		m_pCompTransform->SetLocalPosition(vPos);

		m_pCompPlayerController->SetUseJump(true);
	}
	else
	{
		m_pCompPlayerController->SetUseJump(false);
	}
}

/* ========================================
	�_���[�W�֐�
	-------------------------------------
	���e�F�_���[�W����
========================================= */
void ObjectPlayer::Damage()
{
	// �_���[�W�A�j���[�V����

	m_bInvincible = true;	// ���G���ԊJ�n
	m_nHp--;				// HP����

	if (m_nHp <= 0)
	{
		// ���S
	}
}

/* ========================================
	���G���ԍX�V�֐�
	-------------------------------------
	���e�F���G���ԏ���
========================================= */
void ObjectPlayer::InvincibleUpdate()
{
	// �J�E���g���Z
	m_fInvFlashCnt	+= DELTA_TIME;
	m_fInvCnt		+= DELTA_TIME;

	// ��莞�Ԃ��Ƃɓ_��
	if (m_fInvFlashCnt >= INV_FLASH_INTERVAL)
	{
		m_fInvFlashCnt	= 0.0f;
		bool bVisible	= m_pCompModelAnime->GetIsVisible();
		m_pCompModelAnime->SetIsVisible(!bVisible);			// ���f���̕\���؂�ւ�
	}
	// ���G���ԏI��
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible	= false;
		m_fInvCnt		= 0.0f;
		m_fInvFlashCnt	= 0.0f;
		m_pCompModelAnime->SetIsVisible(true);	// ���f���̕\��ON
	}
}

/* ========================================
	�Q�b�^�[(HP)�֐�
	-------------------------------------
	�ߒl�Fint	HP
=========================================== */
int ObjectPlayer::GetHp()
{
	return m_nHp;
}

/* ========================================
	�Q�b�^�[(�ő�HP)�֐�
	-------------------------------------
	�ߒl�Fint	�ő�HP
=========================================== */
int ObjectPlayer::GetMaxHp()
{
	return m_nMaxHp;
}

/* ========================================
	�Q�b�^�[(���G����)�֐�
	-------------------------------------
	�ߒl�Fbool	���G���ԃt���O
=========================================== */
bool ObjectPlayer::GetInvincible()
{
	return m_bInvincible;
}

/* ========================================
	�Z�b�^�[(HP)�֐�
	-------------------------------------
	�����Fint	HP
=========================================== */
void ObjectPlayer::SetHp(int hp)
{
	m_nHp = hp;
}

/* ========================================
	�Z�b�^�[(�ő�HP)�֐�
	-------------------------------------
	�����Fint	�ő�HP
=========================================== */
void ObjectPlayer::SetMaxHp(int maxHp)
{
	m_nMaxHp = maxHp;
}

/* ========================================
	�Z�b�^�[(���G����)�֐�
	-------------------------------------
	�����Fbool	���G���ԃt���O
=========================================== */
void ObjectPlayer::SetInvincible(bool bInvincible)
{
	m_bInvincible = bInvincible;
}

