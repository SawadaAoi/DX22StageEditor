/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g(����)�pcpp
	------------------------------------
	ObjectGroundGrass.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGroundGrass.h"
#include "ComponentGeometry.h"
#include "TextureManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
========================================== */
ObjectGroundGrass::ObjectGroundGrass(SceneBase* pScene)
	: ObjectGround(pScene)
{

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectGroundGrass::InitLocal()
{
	ObjectGround::InitLocal();

	// �e�N�X�`���ݒ�
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::GROUND_GRASS));
}
