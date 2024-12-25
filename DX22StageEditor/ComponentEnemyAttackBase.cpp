/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(攻撃)用cpp
	------------------------------------
	ComponentEnemyAttackBase.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentEnemyAttackBase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

#include "ObjectBase.h"

const float DEFAULT_ATK_INTERVAL = 2.0f;


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有者オブジェクト
========================================= */
ComponentEnemyAttackBase::ComponentEnemyAttackBase(ObjectBase* pOwner)
	:ComponentBase(pOwner, OrderEnemyAttack)
	, m_pCompTransform(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fAtkInvTime(DEFAULT_ATK_INTERVAL)
	, m_fAtkInvCnt(0.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ComponentEnemyAttackBase::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();
}


/* ========================================
	ゲッター(攻撃間隔時間)関数
	-------------------------------------
	戻値：float		攻撃間隔時間
=========================================== */
float ComponentEnemyAttackBase::GetAtkIntervalTime()
{
	return m_fAtkInvTime;
}


/* ========================================
	セッター(攻撃間隔時間)関数
	-------------------------------------
	引数：float		攻撃間隔時間
=========================================== */
void ComponentEnemyAttackBase::SetAtkIntervalTime(float fInterval)
{
	m_fAtkInvTime = fInterval;
}
