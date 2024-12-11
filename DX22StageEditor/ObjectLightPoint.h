/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�|�C���g���C�g)�p�w�b�_
	------------------------------------
	�����F���C�g�I�u�W�F�N�g�̏���
	------------------------------------
	ObjectLightPoint.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectLight.h"

// =============== �N���X��` =====================
class ObjectLightPoint :
    public ObjectLight
{
public:
	ObjectLightPoint(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightPoint)
};

