/* ========================================
	CatRobotGame/
	------------------------------------
�@�@�I�u�W�F�N�g(�X�e�[�W���{�e)�ph
	------------------------------------
	�����F�X�e�[�W�Z���N�g��ʂŕ\������
		�@�e�X�e�[�W���{�̐e�I�u�W�F�N�g
	------------------------------------
	ObjectStageSampleParent.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �N���X�̒�` ===================
class ObjectStageSampleParent :
    public ObjectBase
{
public:
	ObjectStageSampleParent(SceneBase* pScene);
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectStageSampleParent)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	float	m_fRotateSpeed;
	bool	m_bIsRotate;
};

