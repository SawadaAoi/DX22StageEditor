/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g(����)�p�w�b�_
	------------------------------------
	�����F���C��������n�ʃI�u�W�F�N�g
	------------------------------------
	ObjectGroundGrass.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectGround.h"

// =============== �N���X��` =====================
class ObjectGroundGrass :
    public ObjectGround
{
public:
	ObjectGroundGrass(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectGroundGrass)	// �I�u�W�F�N�g�̎��ID�擾�֐�
};

