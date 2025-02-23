/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�����鏰)�pcpp
	------------------------------------
	ObjectDropFloor.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectDropFloor.h"
#include "ComponentRigidbody.h"
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"

// =============== �萔��` =======================
const Vector3<float>	DEFAULT_SCALE	= { 3.0f, 0.25f, 3.0f };	// �f�t�H���g�X�P�[��
const float				DROP_WAIT_TIME	= 0.5f;						// �����҂�����
const float				RESPAWN_TIME	= 5.0f;						// ���X�|�[������

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================= */
ObjectDropFloor::ObjectDropFloor(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_bIsDropSwitch(false)
	, m_fDropWaitTime(0.0f)
	, m_bIsRespawn(false)
	, m_fRespawnTime(0.0f)
	, m_pCompRigidBody(nullptr)
	, m_vDefaultPos(Vector3<float>(0.0f, 0.0f, 0.0f))
{
	SetTag(E_ObjectTag::Ground);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================= */
void ObjectDropFloor::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// �n�ʔ���
	m_eColType = E_COL_TYPE::COL_AABB;	// �Փ˔���`��

	ObjectBlock::InitLocal();
	m_pCompRigidBody = AddComponent<ComponentRigidbody>();
	m_pCompRigidBody->SetUseGravity(false);

	m_pCompTransform->SetScale(DEFAULT_SCALE);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V�������s��
========================================= */
void ObjectDropFloor::UpdateLocal()
{
	// �����ҋ@����
	if (m_bIsDropSwitch)
	{
		m_fDropWaitTime += DELTA_TIME;
		if (m_fDropWaitTime >= DROP_WAIT_TIME)
		{
			m_pCompRigidBody->SetUseGravity(true);
			m_bIsRespawn = true;
		}
	}

	// ���X�|�[������
	if (m_bIsRespawn)
	{
		m_fRespawnTime += DELTA_TIME;
		if (m_fRespawnTime >= RESPAWN_TIME)
		{
			// �d�͂𖳌���
			m_pCompRigidBody->SetUseGravity(false);
			m_pCompRigidBody->SetVelocity(Vector3<float>(0.0f, 0.0f, 0.0f));
			m_pCompRigidBody->SetAcceleration(Vector3<float>(0.0f, 0.0f, 0.0f));
			// �e�t���O�����Z�b�g
			m_bIsDropSwitch		= false;
			m_bIsRespawn		= false;
			// �҂����Ԃ����Z�b�g
			m_fDropWaitTime		= 0.0f;
			m_fRespawnTime		= 0.0f;
			// �����ʒu�ɖ߂�
			m_pCompTransform->SetPosition(m_vDefaultPos);
		}

	}

}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectDropFloor::OnCollisionEnter(ObjectBase* pHit)
{
	if (pHit->CheckTag(E_ObjectTag::Player))
	{
		m_bIsDropSwitch = true;
		m_vDefaultPos = m_pCompTransform->GetPosition();	// �����ʒu��ۑ�
	}
}
