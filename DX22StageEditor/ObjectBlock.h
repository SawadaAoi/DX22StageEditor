/* ========================================
	DX22Base/
	------------------------------------
	�u���b�N�p�w�b�_
	------------------------------------
	�����F�V���v���ȃu���b�N�I�u�W�F�N�g
	------------------------------------
	ObjectBlock.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentGeometry;
class ComponentCollisionOBB;

// =============== �N���X��` ===================
class ObjectBlock
	: public ObjectBase
{
public:
	struct S_SaveData
	{
		int nTextureID;	// �e�N�X�`��ID
		bool bUseTex;	// �e�N�X�`���g�p�t���O
	};
public:
	ObjectBlock(SceneBase* pScene);

	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectBlock)	// �I�u�W�F�N�g�̎��ID�擾�֐�
	
private:
	ComponentGeometry*		m_pCompGeometry;
	ComponentCollisionOBB*	m_pCompColObb;
};

