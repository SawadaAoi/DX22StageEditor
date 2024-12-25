/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L����)�p�w�b�_
	------------------------------------
	�����F�G�L�����N�^�[�̊��N���X
	------------------------------------
	ObjectEnemy.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentCollisionSphere;
class ComponentRigidbody;
class ComponentGroundRaycast;
class ComponentModelAnime;
class ComponentEnemyState;

// =============== �N���X��` =====================
class ObjectEnemy :
    public ObjectBase
{
public:
	ObjectEnemy(SceneBase* pScene);
	void InitLocal();
	void UpdateLocal();

	void OnCollisionStay(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectEnemy)	// �I�u�W�F�N�g�̎��ID�擾�֐�

private:
	void Damage();
private:
	ComponentCollisionSphere*	m_pCompColSphere;		// �Փ˔���p�R���|�[�l���g
	ComponentRigidbody*			m_pCompRigidbody;		// �������Z�p�R���|�[�l���g
	ComponentGroundRaycast*		m_pCompGroundRaycast;	// �n�ʔ���p�R���|�[�l���g
	ComponentModelAnime*		m_pCompModelAnime;		// ���f���A�j���[�V�����p�R���|�[�l���g
	ComponentEnemyState*		m_pCompEnemyState;		// �G�L������ԊǗ��R���|�[�l���g

	// HP
	int m_nHp;
	int m_nMaxHp;
};

