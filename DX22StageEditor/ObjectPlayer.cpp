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
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ComponentModelAnime.h"
#include "ComponentModelStatic.h"

#include "ModelStaticManager.h"

// =============== 定数定義 =======================
const int	MAX_HP				= 5;	// プレイヤーの最大HP
const float INVINCIBLE_TIME		= 2.0f;	// 無敵時間
const float INV_FLASH_INTERVAL	= 0.1f;	// 無敵点滅間隔
// レイ
const float				RAY_LENGTH	= 0.2f;									// レイの長さ
const Vector3<float>	RAY_OFFSET	= Vector3<float>(0.0f, -0.4f, 0.0f);	// レイの開始位置
// リジッドボディ
const float				GROUND_DRAG = 0.9f;	// 地面摩擦

#define animeOrStatic 0;

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
	, m_pCompModelStatic(nullptr)
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
	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(RAY_OFFSET);
	m_pCompGroundRaycast->SetRayLength(RAY_LENGTH);

	//AddComponent<ComponentCollisionOBB>();
	//AddComponent<ComponentCollisionAABB>();
	AddComponent<ComponentCollisionSphere>();
	
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetUseGravity(true);
	m_pCompRigidbody->SetGroundDrag(GROUND_DRAG);

	m_pCompPlayerController = AddComponent<ComponentPlayerController>();

#if animeOrStatic
	m_pCompModelAnime = AddComponent<ComponentModelAnime>();

	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_PLAYER));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE, true, 1.0f);
#else
	m_pCompModelStatic = AddComponent<ComponentModelStatic>();
	m_pCompModelStatic->SetModel(GET_MODEL_STATIC(MODEL_KEY::PLAYER_CAT));
	//m_pCompModelStatic->SetPosOffset(Vector3<float>(0.0f, 0.3f, 0.0f));
#endif

	SetLightMaterial(1.0f, 0.9f, 0.3f);
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

void ObjectPlayer::GameClear()
{
	m_pCompPlayerController->SetInputEnable(false);			// 操作を無効に
	m_pCompTransform->LookAt(-Vector3<float>::Forward());	// プレイヤーの向きを-Z方向に
	m_pCompRigidbody->SetVelocity(Vector3<float>::Zero());	// 移動速度を0に
}

/* ========================================
	衝突判定(継続時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(継続時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectPlayer::OnCollisionStay(ObjectBase* pHit)
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
	if (m_bInvincible)	return;	// 無敵時間中はダメージを受けない
	if (m_nHp <= 0)		return;	// HPが0以下の場合はダメージを受けない

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
#if animeOrStatic
		m_pCompModelAnime->SetIsVisible(!bVisible);			// モデルの表示切り替え
#else
		m_pCompModelStatic->SetIsVisible(!bVisible);			// モデルの表示切り替え
#endif
	}
	// 無敵時間終了
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible	= false;
		m_fInvCnt		= 0.0f;
		m_fInvFlashCnt	= 0.0f;
#if animeOrStatic
		m_pCompModelAnime->SetIsVisible(true);	// モデルの表示ON
#else
		m_pCompModelStatic->SetIsVisible(true);	// モデルの表示ON
#endif
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

