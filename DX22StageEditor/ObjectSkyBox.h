/* ========================================
	CatRobotGame/
	------------------------------------
	�X�J�C�{�b�N�X�p�w�b�_
	------------------------------------
	�����F�w�i�̃X�J�C�{�b�N�X
	------------------------------------
	ObjectSkyBox.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =========================
class ComponentModelStatic;

// =============== �N���X��` =====================
class ObjectSkyBox :
    public ObjectBase
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveDataSkyBox
	{
		int nModelId;
	};
public:
	ObjectSkyBox(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// �f�[�^�ۑ�
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectSkyBox)
private:
	ComponentModelStatic* m_pCompModelStatic;
};

