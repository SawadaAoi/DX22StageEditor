/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L����)�pcpp
	------------------------------------
	ObjectEnemy.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectEnemy.h"

#include "ComponentCollisionSphere.h"
#include "ComponentEnemyState.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "ModelAnimeManager.h"

#include "ComponentEnemyMoveLinear.h"

// =============== �萔��` =======================
const int		MAX_HP				= 1;	// �ő�HP
const float		INVINCIBLE_TIME		= 1.0f;	// ���G����
const float		INV_FLASH_INTERVAL	= 0.1f;	// ���G�_�ŊԊu
// ���C
const float				RAY_LENGTH = 0.2f;								// ���C�̒���
const Vector3<float>	RAY_OFFSET = Vector3<float>(0.0f, -0.4f, 0.0f);	// ���C�̊J�n�ʒu
// ���W�b�h�{�f�B
const float				GROUND_DRAG = 0.9f;	// �n�ʖ��C

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectEnemy::ObjectEnemy(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCompColSphere(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompGroundRaycast(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_pCompEnemyState(nullptr)
	, m_nHp(MAX_HP)
	, m_nMaxHp(MAX_HP)
	, m_bInvincible(false)
	, m_fInvCnt(0.0f)
	, m_fInvFlashCnt(0.0f)
{
	SetTag(E_ObjectTag::Enemy);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================== */
void ObjectEnemy::InitLocal()
{
	m_pCompColSphere = AddComponent<ComponentCollisionSphere>();
	m_pCompColSphere->SetRadius(1.0f);
	m_pCompColSphere->SetTrigger(true);

	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetGroundDrag(GROUND_DRAG);

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(RAY_OFFSET);
	m_pCompGroundRaycast->SetRayLength(RAY_LENGTH);

	m_pCompModelAnime = AddComponent<ComponentModelAnime>();
	m_pCompModelAnime->SetPosOffset(Vector3<float>(0.0f, -0.5f, 0.0f));
	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_ENEMY_MOUSE));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE, true, 1.0f);

	m_pCompEnemyState = AddComponent<ComponentEnemyState>();

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================== */
void ObjectEnemy::UpdateLocal()
{	
	CheckGround();	// �n�ʔ���

	// �_���[�W��̖��G���ԏ���
	if (m_bInvincible)	InvincibleUpdate();
}

/* ========================================
	�Փ˔���(�p����)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�p����)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectEnemy::OnCollisionStay(ObjectBase* pHit)
{
	if (pHit->GetTag() == E_ObjectTag::PlayerBullet)	// �Փˑ��肪�v���C���[�̏ꍇ
	{
		Damage();	// �_���[�W����
	}
}

/* ========================================
	�n�ʔ���֐�
	-------------------------------------
	���e�F�n�ʔ��菈��
======================================== */
void ObjectEnemy::CheckGround()
{	
	// �n�ʂɐݒu���Ă���ꍇ�@���@�W�����v���łȂ��ꍇ
	if (m_pCompGroundRaycast->GetHitFlg()
		&& m_pCompRigidbody->GetVelocity().y <= 0.0f)
	{
		// �n�ʂɐݒu���Ă���ʒu�Ƀv���C���[��ݒ�
		Vector3<float> vPos = m_pCompTransform->GetWorldPosition();
		vPos.y = m_pCompGroundRaycast->GetHitPos().y + 0.5f;
		m_pCompTransform->SetLocalPosition(vPos);

	}
}

/* ========================================
	�_���[�W�֐�
	-------------------------------------
	���e�F�G�L�����̃_���[�W����
======================================== */
void ObjectEnemy::Damage()
{
	if (m_bInvincible)	return;	// ���G���Ԓ��̓_���[�W���󂯂Ȃ�
	if (m_nHp <= 0)		return;	// HP��0�ȉ��̏ꍇ�̓_���[�W���󂯂Ȃ�

	// �_���[�W�A�j���[�V����
	m_bInvincible = true;	// ���G���ԊJ�n
	m_nHp--;				// HP����

	// HP��0�ɂȂ�����
	if (m_nHp <= 0)
	{
		SetState(E_State::STATE_DEAD);	// ���S��Ԃɐݒ�
	}
}

/* ========================================
	���G���ԍX�V�֐�
	-------------------------------------
	���e�F���G���Ԃ̍X�V����
======================================== */
void ObjectEnemy::InvincibleUpdate()
{	
	// �J�E���g���Z
	m_fInvFlashCnt += DELTA_TIME;
	m_fInvCnt += DELTA_TIME;

	// ��莞�Ԃ��Ƃɓ_��
	if (m_fInvFlashCnt >= INV_FLASH_INTERVAL)
	{
		m_fInvFlashCnt = 0.0f;
		bool bVisible = m_pCompModelAnime->GetIsVisible();
		m_pCompModelAnime->SetIsVisible(!bVisible);			// ���f���̕\���؂�ւ�
	}
	// ���G���ԏI��
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible = false;
		m_fInvCnt = 0.0f;
		m_fInvFlashCnt = 0.0f;
		m_pCompModelAnime->SetIsVisible(true);	// ���f���̕\��ON
	}
}

/* ========================================
	�Q�b�^�[(HP)�֐�
	-------------------------------------
	�ߒl�Fint	HP
=========================================== */
int ObjectEnemy::GetHp()
{
	return m_nHp;
}

/* ========================================
	�Q�b�^�[(�ő�HP)�֐�
	-------------------------------------
	�ߒl�Fint	�ő�HP
=========================================== */
int ObjectEnemy::GetMaxHp()
{
	return m_nMaxHp;
}

/* ========================================
	�Q�b�^�[(���G����)�֐�
	-------------------------------------
	�ߒl�Fbool	���G���ԃt���O
=========================================== */
bool ObjectEnemy::GetInvincible()
{
	return m_bInvincible;
}

/* ========================================
	�Z�b�^�[(HP)�֐�
	-------------------------------------
	�����Fint	HP
=========================================== */
void ObjectEnemy::SetHp(int hp)
{
	m_nHp = hp;
}

/* ========================================
	�Z�b�^�[(�ő�HP)�֐�
	-------------------------------------
	�����Fint	�ő�HP
=========================================== */
void ObjectEnemy::SetMaxHp(int maxHp)
{
	m_nMaxHp = maxHp;
}

/* ========================================
	�Z�b�^�[(���G����)�֐�
	-------------------------------------
	�����Fbool	���G���ԃt���O
=========================================== */
void ObjectEnemy::SetInvincible(bool bInvincible)
{
	m_bInvincible = bInvincible;
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemy::OutPutLocalData(std::ofstream& file)
{
	S_SaveDataEnemy data;

	// HP
	data.nMaxHp = m_nMaxHp;

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveDataEnemy));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemy::InputLocalData(std::ifstream& file)
{
	S_SaveDataEnemy data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveDataEnemy));

	// HP
	m_nMaxHp = data.nMaxHp;
}