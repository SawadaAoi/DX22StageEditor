/* ========================================
	CatRobotGame/
	------------------------------------
	�u���b�N�I�u�W�F�N�g�pcpp
	------------------------------------
	ObjectBlock.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"
#include "ComponentGeometry.h"
#include "ComponentCollisionBase.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionAABB.h"
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
	, m_pCompColBase(nullptr)
	, m_eColType(E_COL_TYPE::COL_OBB)
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

	// �Փ˔���`��ɂ���ăR���|�[�l���g��ǉ�
	if (m_eColType == E_COL_TYPE::COL_AABB)
		m_pCompColBase = AddComponent<ComponentCollisionAABB>();
	else if (m_eColType == E_COL_TYPE::COL_OBB)
		m_pCompColBase = AddComponent<ComponentCollisionOBB>();

}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectBlock::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// �e�N�X�`��ID
	data.nTextureID = TEXTURE_MNG_INST.GetTextureKey(m_pCompGeometry->GetTexture());
	// �e�N�X�`���g�p�t���O
	data.bUseTex	= m_pCompGeometry->GetIsTex();
	// �Փ˔���`��
	data.eColType	= m_eColType;

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectBlock::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// �e�N�X�`���ݒ�
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA((TextureManager::E_TEX_KEY)data.nTextureID));
	// �e�N�X�`���g�p�t���O
	m_pCompGeometry->SetIsTex(data.bUseTex);
	// �Փ˔���`��
	m_eColType = data.eColType;
}
