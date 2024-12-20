/* ========================================
	CatRobotGame/
	------------------------------------
	�e�I�u�W�F�N�g(���)�p�w�b�_
	------------------------------------
	�����F�e�I�u�W�F�N�g�̊��N���X
		�@�����Ă�������Ɉړ�����
	------------------------------------
	ObjectBullet.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBullet.h"


// =============== �N���X��` =====================
class ObjectBulletPlayer :
	public ObjectBullet
{
public:
	ObjectBulletPlayer(SceneBase* pScene);

	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectBulletPlayer)
};

