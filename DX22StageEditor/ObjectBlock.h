/* ========================================
	CatRobotGame/
	------------------------------------
	�u���b�N�I�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F�u���b�N�I�u�W�F�N�g�̊��N���X
	------------------------------------
	ObjectBlock.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentGeometry;
class ComponentCollisionBase;

// =============== �N���X��` ===================
class ObjectBlock
	: public ObjectBase
{
public:
	enum E_COL_TYPE
	{
		COL_OBB,
		COL_AABB,
	};

	struct S_SaveData
	{
		int nTextureID[6];				// �e�N�X�`��ID
		bool bUseTex;					// �e�N�X�`���g�p�t���O
		E_COL_TYPE eColType;			// �Փ˔���`��
		Vector2<float> vUvScale[3];		// �e�N�X�`���X�P�[��
		Vector2<float> vUvOffset[3];	// �e�N�X�`���I�t�Z�b�g
	};
public:
	ObjectBlock(SceneBase* pScene);

	void InitLocal() override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectBlock)	// �I�u�W�F�N�g�̎��ID�擾�֐�
	
protected:
	ComponentGeometry*		m_pCompGeometry;
	ComponentCollisionBase*	m_pCompColBase;

	E_COL_TYPE m_eColType;	// �Փ˔���`��
};

