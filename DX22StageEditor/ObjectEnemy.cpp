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
	, m_nHp(1)
	, m_nMaxHp(1)
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
	m_pCompColSphere		= AddComponent<ComponentCollisionSphere>();
	m_pCompColSphere->SetRadius(0.7f);
	m_pCompColSphere->SetTrigger(true);

	m_pCompRigidbody		= AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetGroundDrag(0.9f);

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pCompGroundRaycast->SetRayLength(0.2f);
	m_pCompModelAnime = AddComponent<ComponentModelAnime>();
	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_ENEMY_G1));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_ENEMY::ENMY_WALK, true, 1.0f);

	m_pCompEnemyState = AddComponent<ComponentEnemyState>();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================== */
void ObjectEnemy::UpdateLocal()
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
	�_���[�W�֐�
	-------------------------------------
	���e�F�G�L�����̃_���[�W����
======================================== */
void ObjectEnemy::Damage()
{
	if (m_nHp <= 0)		return;	// HP��0�ȉ��̏ꍇ�̓_���[�W���󂯂Ȃ�

	m_nHp--;	// HP�����炷

	// HP��0�ɂȂ�����
	if (m_nHp <= 0)
	{
		SetState(E_State::STATE_DEAD);	// ���S��Ԃɐݒ�
	}
}