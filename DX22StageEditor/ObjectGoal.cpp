/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�S�[��)�pcpp
	------------------------------------
	ObjectGoal.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGoal.h"
#include "ComponentBillboard.h"
#include "ComponentCollisionSphere.h"
#include "TextureManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectGoal::ObjectGoal(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_bIsGoal(false)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ObjectGoal::InitLocal()
{
	// TODO: �Ƃ肠�������̃e�N�X�`����ݒ�
	AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::STAGE_GOAL));
	AddComponent<ComponentCollisionSphere>()->SetTrigger(true);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ObjectGoal::UpdateLocal()
{
	// �S�[���������\���ɂ���
	if (m_bIsGoal)
	{
		GetComponent<ComponentBillboard>()->SetActive(false);
	}

}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectGoal::OnCollisionEnter(ObjectBase* pHit)
{
	// �v���C���[�ƐڐG������S�[���t���O�𗧂Ă�
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		m_bIsGoal = true;
	}
}

/* ========================================
	�Q�b�^�[(�S�[���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�S�[���t���O
=========================================== */
bool ObjectGoal::GetIsGoal()
{
	return m_bIsGoal;
}

/* ========================================
	�Z�b�^�[(�S�[���t���O)�֐�
	-------------------------------------
	�����Fbool	�S�[���t���O
=========================================== */
void ObjectGoal::SetIsGoal(bool bIsGoal)
{
	m_bIsGoal = bIsGoal;
}
