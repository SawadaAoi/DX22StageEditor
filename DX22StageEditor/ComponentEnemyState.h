/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ状態管理コンポーネント用cpp
	------------------------------------
	説明：敵キャラの状態を管理するコンポーネント
	------------------------------------
	ComponentEnemyState.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentRigidbody;
class ComponentEnemyMoveBase;
class ComponentEnemyAttackBase;
class ComponentModelAnime;

// =============== クラス定義 ===================
class ComponentEnemyState :
    public ComponentBase
{
public:
	// 敵キャラ状態
	enum E_EnemyState
	{
		ES_MOVE,
		ES_ATTACK,
	};

public:
	ComponentEnemyState(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// ゲッター
	E_EnemyState GetEnemyState();

	// セッター
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

