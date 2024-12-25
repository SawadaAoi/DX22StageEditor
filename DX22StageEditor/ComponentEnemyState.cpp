/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント用cpp
	------------------------------------
	ComponentEnemyMove.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBase.h"

#include "ComponentEnemyState.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentEnemyMoveBase.h"
#include "ComponentEnemyAttackBase.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentEnemyState::ComponentEnemyState(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyState)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_eEnemyState(ES_MOVE)
	, m_pCompEnemyMove(nullptr)
	, m_pCompEnemyAttack(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================== */
void ComponentEnemyState::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================== */
void ComponentEnemyState::Update()
{
	if (m_pCompEnemyMove)
	{
		if (m_eEnemyState == E_EnemyState::ES_MOVE)
			m_pCompEnemyMove->SetActive(true);
		else
			m_pCompEnemyMove->SetActive(false);
	}
	if (m_pCompEnemyAttack)
	{
		if (m_eEnemyState == E_EnemyState::ES_ATTACK)
			m_pCompEnemyAttack->SetActive(true);
		else
			m_pCompEnemyAttack->SetActive(false);
	}


}


/* ========================================
	ゲッター(敵キャラ状態)関数
	-------------------------------------
	戻値：敵キャラ状態
=========================================== */
ComponentEnemyState::E_EnemyState ComponentEnemyState::GetEnemyState()
{
	return m_eEnemyState;
}


/* ========================================
	セッター(敵キャラ状態)関数
	-------------------------------------
	引数1：敵キャラ状態
=========================================== */
void ComponentEnemyState::SetEnemyState(E_EnemyState eState)
{
	m_eEnemyState = eState;
}

/* ========================================
	セッター(移動コンポーネント)関数
	-------------------------------------
	引数1：移動コンポーネント
=========================================== */
void ComponentEnemyState::SetMoveComponent(ComponentEnemyMoveBase* pMove)
{
	m_pCompEnemyMove = pMove;
}

/* ========================================
	セッター(攻撃コンポーネント)関数
	-------------------------------------
	引数1：攻撃コンポーネント
=========================================== */
void ComponentEnemyState::SetAttackComponent(ComponentEnemyAttackBase* pAttack)
{
	m_pCompEnemyAttack = pAttack;
}