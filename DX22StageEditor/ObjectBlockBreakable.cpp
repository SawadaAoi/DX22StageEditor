/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�j��\�u���b�N)�pcpp
	------------------------------------
	ObjectBlockBreakable.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBlockBreakable.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGeometry.h"
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"

#include "ObjectExplosion.h"

#include "SceneBase.h"
#include "TextureManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================== */
ObjectBlockBreakable::ObjectBlockBreakable(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_nHp(1)
	, m_nHpOld(0)
{
	SetTag(E_ObjectTag::Ground);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================== */
void ObjectBlockBreakable::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// �n�ʔ���
	m_eColType = E_COL_TYPE::COL_AABB;	// �Փ˔���`��
	ObjectBlock::InitLocal();			// �e�N���X�̏������֐����Ă�

	m_pCompGeometry->SetTextureAll(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_BRICK));
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V�������s��
========================================== */
void ObjectBlockBreakable::UpdateLocal()
{
	if (m_nHp != m_nHpOld)
	{
		// HP��1�̎��A�e�N�X�`�����T���ԂɕύX
		if (m_nHp == 1)
			m_pCompGeometry->SetTextureAll(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_BRICK_CRACK));

		m_nHpOld = m_nHp;	// HP���X�V
	}
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	����1�F�Փˑ���I�u�W�F�N�g
========================================== */
void ObjectBlockBreakable::OnCollisionEnter(ObjectBase* pHit)
{
	// �e�ɓ���������
	if (pHit->GetTag() == E_ObjectTag::PlayerBullet)
	{
		m_nHp--;

		if (m_nHp <= 0)
		{
			Destroy();	// ���g���폜

			// �����G�t�F�N�g����(��)
			ObjectBase* pExplosion = m_pOwnerScene->AddSceneObject<ObjectExplosion>("Explosion_" + m_sName);
			pExplosion->GetTransform()->SetPosition(m_pCompTransform->GetPosition());
		}
	}
}
