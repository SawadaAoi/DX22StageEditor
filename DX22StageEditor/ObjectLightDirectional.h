/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�������C�g)�p�w�b�_
	------------------------------------
	�����F���C�g�I�u�W�F�N�g�̏���
	------------------------------------
	ObjectLightDirectional.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectLight.h"

// =============== �N���X��` =====================
class ObjectLightDirectional :
    public ObjectLight
{
public:
	ObjectLightDirectional(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightDirectional)

};

