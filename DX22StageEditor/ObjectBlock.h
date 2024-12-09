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
	ObjectBlock(SceneBase* pScene);

	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectBlock)	// �I�u�W�F�N�g�̎��ID�擾�֐�
	
private:
	ComponentGeometry*		m_pCompGeometry;
	ComponentCollisionOBB*	m_pCompColObb;
};

