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
	// �Z�[�u�f�[�^
	struct S_SaveDataEnemy
	{
		int nHp;
		int nMaxHp;
	};
public:
	ObjectEnemy(SceneBase* pScene);
	void InitLocal();
	void UpdateLocal();

	void OnCollisionStay(ObjectBase* pHit) override;

	// �Q�b�^�[
	int GetHp();
	int GetMaxHp();
	bool GetInvincible();

	// �Z�b�^�[
	void SetHp(int hp);
	void SetMaxHp(int maxHp);
	void SetInvincible(bool bInvincible);

	// �f�[�^�ۑ�
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemy)	// �I�u�W�F�N�g�̎��ID�擾�֐�
private:
	void CheckGround();
	void Damage();
	void InvincibleUpdate();
protected:
	ComponentCollisionSphere*	m_pCompColSphere;		// �Փ˔���p�R���|�[�l���g
	ComponentRigidbody*			m_pCompRigidbody;		// �������Z�p�R���|�[�l���g
	ComponentGroundRaycast*		m_pCompGroundRaycast;	// �n�ʔ���p�R���|�[�l���g
	ComponentModelAnime*		m_pCompModelAnime;		// ���f���A�j���[�V�����p�R���|�[�l���g
	ComponentEnemyState*		m_pCompEnemyState;		// �G�L������ԊǗ��R���|�[�l���g

	// HP
	int m_nHp;
	int m_nMaxHp;

	// ���G����
	bool	m_bInvincible;	// ���G���ԃt���O
	float	m_fInvCnt;		// ���G���ԃJ�E���g
	float	m_fInvFlashCnt;	// ���G���ԓ_�ŃJ�E���g

};

