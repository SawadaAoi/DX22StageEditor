/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(プレイヤー)用cpp
	------------------------------------
	ObjectPlayer.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectPlayer.h"
// コンポーネント
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionOBB.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"

// =============== 定数定義 =======================
const int	MAX_HP = 5;	// プレイヤーの最大HP
const float INVINCIBLE_TIME = 2.0f;	// 無敵時間
const float INV_FLASH_INTERVAL = 0.1f;	// 無敵点滅間隔

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectPlayer::ObjectPlayer(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGroundRaycast(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_pCompPlayerController(nullptr)
	, m_nMaxHp(MAX_HP)
	, m_nHp(MAX_HP)
	, m_bInvincible(false)
	, m_fInvCnt(0.0f)
	, m_fInvFlashCnt(0.0f)

{
	SetTag(E_ObjectTag::Player);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectPlayer::InitLocal()
{
	m_pCompTransform =GetComponent<ComponentTransform>();

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pCompGroundRaycast->SetRayLength(0.2f);

	AddComponent<ComponentCollisionOBB>();
	
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetUseGravity(true);
	m_pCompRigidbody->SetGroundDrag(0.9f);

	m_pCompPlayerController = AddComponent<ComponentPlayerController>();

	m_pCompModelAnime = AddComponent<ComponentModelAnime>();

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectPlayer::UpdateLocal()
{
	// 接地判定
	CheckGround();

	// ダメージ後の無敵時間処理
	if (m_bInvincible)	InvincibleUpdate();
		

}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectPlayer::OnCollisionEnter(ObjectBase* pHit)
{
	// 敵キャラと接触したら
	if (pHit->GetTag() == E_ObjectTag::Enemy)
	{
		Damage();
	}
}

/* ========================================
	接地判定関数
	-------------------------------------
	内容：地面に設置しているか判定
========================================= */
void ObjectPlayer::CheckGround()
{
	// 地面に設置している場合　かつ　ジャンプ中でない場合
	if (m_pCompGroundRaycast->GetHitFlg() 
		&& m_pCompRigidbody->GetVelocity().y <= 0.0f)
	{
		// 地面に設置している位置にプレイヤーを設定
		Vector3<float> vPos = m_pCompTransform->GetWorldPosition();
		vPos.y = m_pCompGroundRaycast->GetHitPos().y + 0.5f;
		m_pCompTransform->SetLocalPosition(vPos);

		m_pCompPlayerController->SetUseJump(true);
	}
	else
	{
		m_pCompPlayerController->SetUseJump(false);
	}
}

/* ========================================
	ダメージ関数
	-------------------------------------
	内容：ダメージ処理
========================================= */
void ObjectPlayer::Damage()
{
	// ダメージアニメーション

	m_bInvincible = true;	// 無敵時間開始
	m_nHp--;				// HP減少

	if (m_nHp <= 0)
	{
		// 死亡
	}
}

/* ========================================
	無敵時間更新関数
	-------------------------------------
	内容：無敵時間処理
========================================= */
void ObjectPlayer::InvincibleUpdate()
{
	// カウント加算
	m_fInvFlashCnt	+= DELTA_TIME;
	m_fInvCnt		+= DELTA_TIME;

	// 一定時間ごとに点滅
	if (m_fInvFlashCnt >= INV_FLASH_INTERVAL)
	{
		m_fInvFlashCnt	= 0.0f;
		bool bVisible	= m_pCompModelAnime->GetIsVisible();
		m_pCompModelAnime->SetIsVisible(!bVisible);			// モデルの表示切り替え
	}
	// 無敵時間終了
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible	= false;
		m_fInvCnt		= 0.0f;
		m_fInvFlashCnt	= 0.0f;
		m_pCompModelAnime->SetIsVisible(true);	// モデルの表示ON
	}
}

/* ========================================
	ゲッター(HP)関数
	-------------------------------------
	戻値：int	HP
=========================================== */
int ObjectPlayer::GetHp()
{
	return m_nHp;
}

/* ========================================
	ゲッター(最大HP)関数
	-------------------------------------
	戻値：int	最大HP
=========================================== */
int ObjectPlayer::GetMaxHp()
{
	return m_nMaxHp;
}

/* ========================================
	ゲッター(無敵時間)関数
	-------------------------------------
	戻値：bool	無敵時間フラグ
=========================================== */
bool ObjectPlayer::GetInvincible()
{
	return m_bInvincible;
}

/* ========================================
	セッター(HP)関数
	-------------------------------------
	引数：int	HP
=========================================== */
void ObjectPlayer::SetHp(int hp)
{
	m_nHp = hp;
}

/* ========================================
	セッター(最大HP)関数
	-------------------------------------
	引数：int	最大HP
=========================================== */
void ObjectPlayer::SetMaxHp(int maxHp)
{
	m_nMaxHp = maxHp;
}

/* ========================================
	セッター(無敵時間)関数
	-------------------------------------
	引数：bool	無敵時間フラグ
=========================================== */
void ObjectPlayer::SetInvincible(bool bInvincible)
{
	m_bInvincible = bInvincible;
}

