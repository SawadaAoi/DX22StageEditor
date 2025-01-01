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
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"
#include "ComponentModelStatic.h"

#include "ModelStaticManager.h"

// =============== �萔��` =======================
const int	MAX_HP				= 5;	// �v���C���[�̍ő�HP
const float INVINCIBLE_TIME		= 2.0f;	// ���G����
const float INV_FLASH_INTERVAL	= 0.1f;	// ���G�_�ŊԊu
// ���C
const float				RAY_LENGTH	= 0.2f;									// ���C�̒���
const Vector3<float>	RAY_OFFSET	= Vector3<float>(0.0f, -0.4f, 0.0f);	// ���C�̊J�n�ʒu
// ���W�b�h�{�f�B
const float				GROUND_DRAG = 0.9f;	// �n�ʖ��C

#define animeOrStatic 0;

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
	, m_pCompModelStatic(nullptr)
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
	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(RAY_OFFSET);
	m_pCompGroundRaycast->SetRayLength(RAY_LENGTH);

	//AddComponent<ComponentCollisionOBB>();
	//AddComponent<ComponentCollisionAABB>();
	AddComponent<ComponentCollisionSphere>();
	
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetUseGravity(true);
	m_pCompRigidbody->SetGroundDrag(GROUND_DRAG);

	m_pCompPlayerController = AddComponent<ComponentPlayerController>();

#if animeOrStatic
	m_pCompModelAnime = AddComponent<ComponentModelAnime>();

	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_PLAYER));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE, true, 1.0f);
#else
	m_pCompModelStatic = AddComponent<ComponentModelStatic>();
	m_pCompModelStatic->SetModel(GET_MODEL_STATIC(MODEL_KEY::PLAYER_CAT));
	//m_pCompModelStatic->SetPosOffset(Vector3<float>(0.0f, 0.3f, 0.0f));
#endif

	SetLightMaterial(1.0f, 0.9f, 0.3f);
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

void ObjectPlayer::GameClear()
{
	m_pCompPlayerController->SetInputEnable(false);			// ����𖳌���
	m_pCompTransform->LookAt(-Vector3<float>::Forward());	// �v���C���[�̌�����-Z������
	m_pCompRigidbody->SetVelocity(Vector3<float>::Zero());	// �ړ����x��0��
}

/* ========================================
	�Փ˔���(�p����)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�p����)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectPlayer::OnCollisionStay(ObjectBase* pHit)
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
	if (m_bInvincible)	return;	// ���G���Ԓ��̓_���[�W���󂯂Ȃ�
	if (m_nHp <= 0)		return;	// HP��0�ȉ��̏ꍇ�̓_���[�W���󂯂Ȃ�

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
#if animeOrStatic
		m_pCompModelAnime->SetIsVisible(!bVisible);			// ���f���̕\���؂�ւ�
#else
		m_pCompModelStatic->SetIsVisible(!bVisible);			// ���f���̕\���؂�ւ�
#endif
	}
	// ���G���ԏI��
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible	= false;
		m_fInvCnt		= 0.0f;
		m_fInvFlashCnt	= 0.0f;
#if animeOrStatic
		m_pCompModelAnime->SetIsVisible(true);	// ���f���̕\��ON
#else
		m_pCompModelStatic->SetIsVisible(true);	// ���f���̕\��ON
#endif
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

