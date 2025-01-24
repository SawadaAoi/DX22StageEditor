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
	for (int i = 0; i < 6; i++)
		m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_SIMPLE), i);

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
	for (int i = 0; i < 6; i++)
		data.nTextureID[i] = TEXTURE_MNG_INST.GetTextureKey(m_pCompGeometry->GetTexture(i));

	// �e�N�X�`���g�p�t���O
	data.bUseTex	= m_pCompGeometry->GetIsTex();
	// �Փ˔���`��
	data.eColType	= m_eColType;

	// �e�N�X�`���X�P�[��
	for (int i = 0; i < 3; i++)
		data.vUvScale[i] = m_pCompGeometry->GetUvScale(i);
	// �e�N�X�`���I�t�Z�b�g
	for (int i = 0; i < 3; i++)
		data.vUvOffset[i] = m_pCompGeometry->GetUvOffset(i);

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
	for (int i = 0; i < 6; i++)
		m_pCompGeometry->SetTexture(GET_TEXTURE_DATA((TextureManager::E_TEX_KEY)data.nTextureID[i]), i);
	// �e�N�X�`���g�p�t���O
	m_pCompGeometry->SetIsTex(data.bUseTex);
	// �Փ˔���`��
	m_eColType = data.eColType;
	// �e�N�X�`���X�P�[��
	for (int i = 0; i < 3; i++)
		m_pCompGeometry->SetUvScale(data.vUvScale[i], i);
	// �e�N�X�`���I�t�Z�b�g
	for (int i = 0; i < 3; i++)
		m_pCompGeometry->SetUvOffset(data.vUvOffset[i], i);
}
