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
#include "ComponentModelAnime.h"
#include "ModelAnimeManager.h"

// =============== 定数定義 =====================
const float	ANIME_SPEED_IDLE = 1.0f;	// 待機アニメーション速度
const float	ANIME_SPEED_WALK = 1.0f;	// 移動アニメーション速度

const float ANIME_BLEND_TIME = 0.2f;	// アニメーションブレンド時間

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
	, m_pCompEnemyMove(nullptr)
	, m_pCompEnemyAttack(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_eEnemyState(ES_MOVE)
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
	m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();
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

	// アニメーションの再生
	MoveAnime();
}


/* ========================================
	アニメーション再生関数
	-------------------------------------
	内容：移動アニメーションの再生
======================================== */
void ComponentEnemyState::MoveAnime()
{
	float fSpeedX = m_pCompRigidbody->GetVelocity().x;
	float fSpeedZ = m_pCompRigidbody->GetVelocity().z;

	// 移動中
	if (fSpeedX != 0.0f || fSpeedZ != 0.0f)
	{
		// 既に再生中でない場合
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK, ANIME_BLEND_TIME, true, ANIME_SPEED_WALK);
	}
	else
	{
		// 既に再生中でない場合
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE, ANIME_BLEND_TIME, true, ANIME_SPEED_IDLE);
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

