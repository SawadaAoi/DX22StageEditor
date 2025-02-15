/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g�pcpp
	------------------------------------
	ObjectGround.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGround.h"
#include "ComponentGeometry.h"
#include "ComponentGround.h"
#include "TextureManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
========================================== */
ObjectGround::ObjectGround(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGeometry(nullptr)
	, m_pCompGround(nullptr)
{
	m_eTag = E_ObjectTag::Ground;
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectGround::InitLocal()
{
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGround = AddComponent<ComponentGround>();

	m_pCompGeometry->SetShapeType(ComponentGeometry::TYPE_PLANE);
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
========================================== */
void ObjectGround::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// �e�N�X�`��ID
	data.nTextureID = TEXTURE_MNG_INST.GetTextureKey(m_pCompGeometry->GetTexture());
	// �e�N�X�`���g�p�t���O
	data.bUseTex = m_pCompGeometry->GetIsTex();
	// �e�N�X�`���X�P�[��
	data.vUvScale = m_pCompGeometry->GetUvScale();
	// �e�N�X�`���I�t�Z�b�g
	data.vUvOffset = m_pCompGeometry->GetUvOffset();

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C�����烍�[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
========================================== */
void ObjectGround::InputLocalData(std::ifstream& file)
{
	S_SaveData data;
	file.read((char*)&data, sizeof(S_SaveData));

	// �e�N�X�`���ݒ�
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA((TextureManager::E_TEX_KEY)data.nTextureID));
	// �e�N�X�`���g�p�t���O
	m_pCompGeometry->SetIsTex(data.bUseTex);
	// �e�N�X�`���X�P�[��
	m_pCompGeometry->SetUvScale(data.vUvScale);
	// �e�N�X�`���I�t�Z�b�g
	m_pCompGeometry->SetUvOffset(data.vUvOffset);
}

