/* ========================================
	CatRobotGame/
	------------------------------------
	OBB�u���b�N�I�u�W�F�N�g(�n�ʔ���L��)�pcpp
	------------------------------------
	ObjectBlockGroundOBB.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBlockGroundOBB.h"
#include "ComponentGroundBox.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================== */
ObjectBlockGroundOBB::ObjectBlockGroundOBB(SceneBase* pScene)
	: ObjectBlock(pScene)
{
	SetTag(E_ObjectTag::Ground);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================== */
void ObjectBlockGroundOBB::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// �n�ʔ���
	m_eColType = E_COL_TYPE::COL_OBB;	// �Փ˔���`��
	ObjectBlock::InitLocal();			// �e�N���X�̏������֐����Ă�
}