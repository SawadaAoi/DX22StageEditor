/* ========================================
	CatRobotGame/
	------------------------------------
	�ǃI�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F��(�v���C���[�������Ȃ���)
	------------------------------------
	ObjectWall.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"
#include <vector>

// =============== �O���錾 =======================
class ComponentCollisionOBB;
class ComponentGeometry;		// ������

// =============== �N���X��` =====================
class ObjectWall :
	public ObjectBase
{
public:
	ObjectWall(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectWall)
private:
	ComponentCollisionOBB*	m_pCompColOBB;
	ComponentGeometry*		m_pCompGeometry;

};

