/* ========================================
	CatRobotGame/
	------------------------------------
	�e�I�u�W�F�N�g(���)�pcpp
	------------------------------------
	ObjectBullet.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectBullet.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionSphere.h"
#include "ComponentTransform.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
=========================================== */
ObjectBullet::ObjectBullet(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompRigidbody(nullptr)
	, m_pCompCollisionSphere(nullptr)
	, m_fDelTime(5.0f)
	, m_fMoveSpeed(10.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void ObjectBullet::InitLocal()
{
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompCollisionSphere = AddComponent<ComponentCollisionSphere>();

	m_pCompRigidbody->SetUseGravity(false);		// �d�͖���
	m_pCompCollisionSphere->SetTrigger(true);	// �Փ˂͂��蔲��

	Destroy(m_fDelTime);	// ��莞�Ԍ�ɍ폜
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectBullet::UpdateLocal()
{
	// �����Ă�������Ɉړ�
	m_pCompRigidbody->SetVelocity(m_pCompTransform->GetForwardVector() * m_fMoveSpeed);
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	����1�F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectBullet::OnCollisionEnter(ObjectBase* pHit)
{
	Destroy();
}

/* ========================================
	�Q�b�^�[(�폜����)�֐�
	-------------------------------------
	�ߒl�Ffloat �폜����
=========================================== */
float ObjectBullet::GetDeleteTime()
{
	return m_fDelTime;
}

/* ========================================
	�Q�b�^�[(�ړ����x)�֐�
	-------------------------------------
	�ߒl�Ffloat �ړ����x
=========================================== */
float ObjectBullet::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	�Z�b�^�[(�폜����)�֐�
	-------------------------------------
	����1�Ffloat �폜����
=========================================== */
void ObjectBullet::SetDeleteTime(float fLifeTime)
{
	m_fDelTime = fLifeTime;
	Destroy(m_fDelTime);	// �폜���Ԃ��Đݒ肷��(���������ɐݒ肳��Ă���l���㏑��)
}

/* ========================================
	�Z�b�^�[(�ړ����x)�֐�
	-------------------------------------
	����1�Ffloat �ړ����x
=========================================== */
void ObjectBullet::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}
