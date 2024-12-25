/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ)用cpp
	------------------------------------
	ObjectEnemy.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectEnemy.h"

#include "ComponentCollisionSphere.h"
#include "ComponentEnemyState.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "ModelAnimeManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectEnemy::ObjectEnemy(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCompColSphere(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompGroundRaycast(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_pCompEnemyState(nullptr)
	, m_nHp(1)
	, m_nMaxHp(1)
{
	SetTag(E_ObjectTag::Enemy);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================== */
void ObjectEnemy::InitLocal()
{
	m_pCompColSphere		= AddComponent<ComponentCollisionSphere>();
	m_pCompColSphere->SetRadius(0.7f);
	m_pCompColSphere->SetTrigger(true);

	m_pCompRigidbody		= AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetGroundDrag(0.9f);

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pCompGroundRaycast->SetRayLength(0.2f);
	m_pCompModelAnime = AddComponent<ComponentModelAnime>();
	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_ENEMY_G1));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_ENEMY::ENMY_WALK, true, 1.0f);

	m_pCompEnemyState = AddComponent<ComponentEnemyState>();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================== */
void ObjectEnemy::UpdateLocal()
{	
	// 地面に設置している場合　かつ　ジャンプ中でない場合
	if (m_pCompGroundRaycast->GetHitFlg()
		&& m_pCompRigidbody->GetVelocity().y <= 0.0f)
	{
		// 地面に設置している位置にプレイヤーを設定
		Vector3<float> vPos = m_pCompTransform->GetWorldPosition();
		vPos.y = m_pCompGroundRaycast->GetHitPos().y + 0.5f;
		m_pCompTransform->SetLocalPosition(vPos);

	}
}

/* ========================================
	衝突判定(継続時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(継続時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectEnemy::OnCollisionStay(ObjectBase* pHit)
{
	if (pHit->GetTag() == E_ObjectTag::PlayerBullet)	// 衝突相手がプレイヤーの場合
	{
		Damage();	// ダメージ処理
	}
}

/* ========================================
	ダメージ関数
	-------------------------------------
	内容：敵キャラのダメージ処理
======================================== */
void ObjectEnemy::Damage()
{
	if (m_nHp <= 0)		return;	// HPが0以下の場合はダメージを受けない

	m_nHp--;	// HPを減らす

	// HPが0になったら
	if (m_nHp <= 0)
	{
		SetState(E_State::STATE_DEAD);	// 死亡状態に設定
	}
}