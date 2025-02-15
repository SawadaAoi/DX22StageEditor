/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F���C��������n�ʃI�u�W�F�N�g
	------------------------------------
	ObjectGround.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentGeometry;
class ComponentGround;


// =============== �N���X��` =====================
class ObjectGround :
    public ObjectBase
{
public:
	struct S_SaveData
	{
		int nTextureID;	// �e�N�X�`��ID
		bool bUseTex;	// �e�N�X�`���g�p�t���O
		Vector2<float> vUvScale;		// �e�N�X�`���X�P�[��
		Vector2<float> vUvOffset;	// �e�N�X�`���I�t�Z�b�g
	};
public:
	ObjectGround(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectGround)	// �I�u�W�F�N�g�̎��ID�擾�֐�

protected:
	ComponentGeometry* m_pCompGeometry;	// �`��
	ComponentGround* m_pCompGround;		// �n�ʃR���|�[�l���g
};

