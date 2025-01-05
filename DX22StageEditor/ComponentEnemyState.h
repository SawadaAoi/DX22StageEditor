/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L������ԊǗ��R���|�[�l���g�pcpp
	------------------------------------
	�����F�G�L�����̏�Ԃ��Ǘ�����R���|�[�l���g
	------------------------------------
	ComponentEnemyState.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentRigidbody;
class ComponentEnemyMoveBase;
class ComponentEnemyAttackBase;
class ComponentModelAnime;

// =============== �N���X��` ===================
class ComponentEnemyState :
    public ComponentBase
{
public:
	// �G�L�������
	enum E_EnemyState
	{
		ES_MOVE,
		ES_ATTACK,
	};

public:
	ComponentEnemyState(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// �Q�b�^�[
	E_EnemyState GetEnemyState();

	// �Z�b�^�[
	void SetEnemyState(E_EnemyState eState);
	void SetMoveComponent(ComponentEnemyMoveBase* pMove);
	void SetAttackComponent(ComponentEnemyAttackBase* pAttack);

	DEFINE_COMPONENT_TYPE
private:
	void MoveAnime();
private:
	ComponentTransform*			m_pCompTransform;
	ComponentRigidbody*			m_pCompRigidbody;
	ComponentEnemyMoveBase*		m_pCompEnemyMove;
	ComponentEnemyAttackBase*	m_pCompEnemyAttack;
	ComponentModelAnime*		m_pCompModelAnime;
	
	E_EnemyState				m_eEnemyState;
};

