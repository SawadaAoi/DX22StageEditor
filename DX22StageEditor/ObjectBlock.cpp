/* ========================================
	DX22Base/
	------------------------------------
	�u���b�N�pcpp
	------------------------------------
	ObjectBlock.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"
#include "ComponentGeometry.h"
#include "ComponentCollisionOBB.h"
#include "TextureManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
=========================================== */
ObjectBlock::ObjectBlock(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGeometry(nullptr)
	, m_pCompColObb(nullptr)
{

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
=========================================== */
void ObjectBlock::InitLocal()
{
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGeometry->SetShapeType(ComponentGeometry::TYPE_BOX);
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_SIMPLE));

	m_pCompColObb = AddComponent<ComponentCollisionOBB>();

}
