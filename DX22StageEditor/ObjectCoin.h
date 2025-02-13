/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�R�C��)�p�w�b�_
	------------------------------------
	�����F�v���C���[���擾����R�C��
		�@�d�͗L��A�r���{�[�h�A�j���[�V�����L��
		�@���蔲���Փ˔���
	------------------------------------
	ObjectCoin.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentBillboardAnime;
class ComponentRigidbody;
class ComponentCollisionSphere;
class ComponentGroundRaycast;

// =============== �N���X ===================
class ObjectCoin :
    public ObjectBase
{
public:
	ObjectCoin(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectCoin)
private:
	ComponentBillboardAnime*	m_pCompBillboardAnime;		// �r���{�[�h�A�j���[�V����
	ComponentRigidbody*			m_pCompRigidbody;			// ����
	ComponentCollisionSphere*	m_pCompCollisionSphere;		// �Փ˔���
	ComponentGroundRaycast*		m_pCompGroundRaycast;		// �n�ʔ���
};

