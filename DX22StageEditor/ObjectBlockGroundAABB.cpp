/* ========================================
	CatRobotGame/
	------------------------------------
	AABB�u���b�N�I�u�W�F�N�g(�n�ʔ���L��)�pcpp
	------------------------------------
	ObjectBlockGroundAABB.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBlockGroundAABB.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGroundBox.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================== */
ObjectBlockGroundAABB::ObjectBlockGroundAABB(SceneBase* pScene)
	: ObjectBlock(pScene)
{
	SetTag(E_ObjectTag::Ground);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================== */
void ObjectBlockGroundAABB::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// �n�ʔ���
	m_eColType = E_COL_TYPE::COL_AABB;	// �Փ˔���`��
	ObjectBlock::InitLocal();			// �e�N���X�̏������֐����Ă�
}