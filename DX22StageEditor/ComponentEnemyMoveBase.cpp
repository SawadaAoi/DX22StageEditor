/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(基底)用cpp
	------------------------------------
	ComponentEnemyMoveBase.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentEnemyMoveBase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

#include "ObjectBase.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentEnemyMoveBase::ComponentEnemyMoveBase(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyMove)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(1.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================= */
void ComponentEnemyMoveBase::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}


/* ========================================
	ゲッター(移動速度)関数
	-------------------------------------
	戻値：移動速度
========================================== */
float ComponentEnemyMoveBase::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	セッター(移動速度)関数
	-------------------------------------
	引数1：移動速度
========================================== */
void ComponentEnemyMoveBase::SetMoveSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}