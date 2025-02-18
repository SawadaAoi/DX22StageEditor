/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�X�e�[�W���{�e)�pcpp
	------------------------------------
	ObjectStageSampleParent.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectStageSampleParent.h"
#include "ComponentTransform.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectStageSampleParent::ObjectStageSampleParent(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_fRotateSpeed(10.0f)
	, m_bIsRotate(true)
{
	SetTag(E_ObjectTag::StageSampleParent);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectStageSampleParent::UpdateLocal()
{
	if (!m_bIsRotate) return;
	// ��]
	m_pCompTransform->RotateY(m_fRotateSpeed * DELTA_TIME);
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectStageSampleParent::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupStageSample = Item::CreateGroup("StageSample");

	// ��]�t���O
	pGroupStageSample->AddGroupItem(Item::CreateBind("IsRotate", Item::Kind::Bool, &m_bIsRotate));
	// ��]���x
	pGroupStageSample->AddGroupItem(Item::CreateBind("RotateSpeed", Item::Kind::Float, &m_fRotateSpeed));
}
#endif // DEBUG