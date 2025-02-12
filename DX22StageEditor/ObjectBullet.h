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
#include "ObjectBase.h"

// =============== �O���錾 =====================
class ComponentRigidbody;
class ComponentCollisionSphere;

// =============== �N���X��` =====================
class ObjectBullet :
	public ObjectBase
{
public:
	ObjectBullet(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	// �Q�b�^�[
	float GetDeleteTime();
	float GetMoveSpeed();

	// �Z�b�^�[
	void SetDeleteTime(float fLifeTime);
	void SetMoveSpeed(float fMoveSpeed);

	DEFINE_OBJECT_TYPE(ObjectBullet)
protected:
	ComponentRigidbody* m_pCompRigidbody;
	ComponentCollisionSphere* m_pCompCollisionSphere;

	float m_fDelTime;		// �ő吶������
	float m_fMoveSpeed;		// �ړ����x
};

