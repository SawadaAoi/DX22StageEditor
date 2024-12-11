/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�X�|�b�g���C�g)�p�w�b�_
	------------------------------------
	�����F���C�g�I�u�W�F�N�g�̏���
	------------------------------------
	ObjectLightSpot.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectLight.h"

// =============== �N���X��` =====================
class ObjectLightSpot :
    public ObjectLight
{
public:
	ObjectLightSpot(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightSpot)
};

