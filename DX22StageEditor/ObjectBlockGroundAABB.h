/* ========================================
	DX22Base/
	------------------------------------
	AABB�u���b�N�I�u�W�F�N�g(�n�ʔ���L��)�p�w�b�_
	------------------------------------
	�����F��]���Ȃ��A�n�ʔ���L��̃u���b�N
	------------------------------------
	ObjectBlockGroundAABB.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"

// =============== �N���X��` ===================
class ObjectBlockGroundAABB :
    public ObjectBlock
{
public:
	ObjectBlockGroundAABB(SceneBase* pScene);
	void InitLocal() override;
	DEFINE_OBJECT_TYPE(ObjectBlockGroundAABB)
};

