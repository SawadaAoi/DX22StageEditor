/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�����鏰)�p�w�b�_
	------------------------------------
	�����F�v���C���[�����ƈ�莞�Ԍ�ɗ������鏰�I�u�W�F�N�g
	------------------------------------
	ObjectDropFloor.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"

// =============== �O���錾 =======================
class ComponentRigidbody;

// =============== �N���X��` =====================
class ObjectDropFloor :
    public ObjectBlock
{
public:
	ObjectDropFloor(SceneBase* pScene);	
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectDropFloor)
private:
	ComponentRigidbody* m_pCompRigidBody;	// ���̃R���|�[�l���g
	bool	m_bIsDropSwitch;				// �����X�C�b�`
	float	m_fDropWaitTime;				// �����J�n����
	bool	m_bIsRespawn;					// ���X�|�[���t���O
	float	m_fRespawnTime;					// ���X�|�[������

	Vector3<float> m_vDefaultPos;			// �����ʒu

};

