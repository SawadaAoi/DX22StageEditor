/* ========================================
	DX22Base/
	------------------------------------
	OBB�u���b�N�I�u�W�F�N�g(�n�ʔ���L��)�p�w�b�_
	------------------------------------
	�����F��]����A�n�ʔ���L��̃u���b�N
	------------------------------------
	ObjectBlockGroundOBB.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"

// =============== �N���X��` ===================
class ObjectBlockGroundOBB :
    public ObjectBlock
{
public:
	ObjectBlockGroundOBB(SceneBase* pScene);
	void InitLocal() override;
	DEFINE_OBJECT_TYPE(ObjectBlockGroundOBB)
};

