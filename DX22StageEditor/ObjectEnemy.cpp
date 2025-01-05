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

#include "ComponentEnemyMoveLinear.h"

// =============== 定数定義 =======================
const int		MAX_HP				= 1;	// 最大HP
const float		INVINCIBLE_TIME		= 1.0f;	// 無敵時間
const float		INV_FLASH_INTERVAL	= 0.1f;	// 無敵点滅間隔
// レイ
const float				RAY_LENGTH = 0.2f;								// レイの長さ
const Vector3<float>	RAY_OFFSET = Vector3<float>(0.0f, -0.4f, 0.0f);	// レイの開始位置
// リジッドボディ
const float				GROUND_DRAG = 0.9f;	// 地面摩擦

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
	, m_nHp(MAX_HP)
	, m_nMaxHp(MAX_HP)
	, m_bInvincible(false)
	, m_fInvCnt(0.0f)
	, m_fInvFlashCnt(0.0f)
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
	m_pCompColSphere = AddComponent<ComponentCollisionSphere>();
	m_pCompColSphere->SetRadius(1.0f);
	m_pCompColSphere->SetTrigger(true);

	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompRigidbody->SetGroundDrag(GROUND_DRAG);

	m_pCompGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pCompGroundRaycast->SetStartPosOffset(RAY_OFFSET);
	m_pCompGroundRaycast->SetRayLength(RAY_LENGTH);

	m_pCompModelAnime = AddComponent<ComponentModelAnime>();
	m_pCompModelAnime->SetPosOffset(Vector3<float>(0.0f, -0.5f, 0.0f));
	m_pCompModelAnime->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_ENEMY_MOUSE));
	m_pCompModelAnime->PlayAnime(ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE, true, 1.0f);

	m_pCompEnemyState = AddComponent<ComponentEnemyState>();

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================== */
void ObjectEnemy::UpdateLocal()
{	
	CheckGround();	// 地面判定

	// ダメージ後の無敵時間処理
	if (m_bInvincible)	InvincibleUpdate();
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
	地面判定関数
	-------------------------------------
	内容：地面判定処理
======================================== */
void ObjectEnemy::CheckGround()
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
	ダメージ関数
	-------------------------------------
	内容：敵キャラのダメージ処理
======================================== */
void ObjectEnemy::Damage()
{
	if (m_bInvincible)	return;	// 無敵時間中はダメージを受けない
	if (m_nHp <= 0)		return;	// HPが0以下の場合はダメージを受けない

	// ダメージアニメーション
	m_bInvincible = true;	// 無敵時間開始
	m_nHp--;				// HP減少

	// HPが0になったら
	if (m_nHp <= 0)
	{
		SetState(E_State::STATE_DEAD);	// 死亡状態に設定
	}
}

/* ========================================
	無敵時間更新関数
	-------------------------------------
	内容：無敵時間の更新処理
======================================== */
void ObjectEnemy::InvincibleUpdate()
{	
	// カウント加算
	m_fInvFlashCnt += DELTA_TIME;
	m_fInvCnt += DELTA_TIME;

	// 一定時間ごとに点滅
	if (m_fInvFlashCnt >= INV_FLASH_INTERVAL)
	{
		m_fInvFlashCnt = 0.0f;
		bool bVisible = m_pCompModelAnime->GetIsVisible();
		m_pCompModelAnime->SetIsVisible(!bVisible);			// モデルの表示切り替え
	}
	// 無敵時間終了
	if (m_fInvCnt >= INVINCIBLE_TIME)
	{
		m_bInvincible = false;
		m_fInvCnt = 0.0f;
		m_fInvFlashCnt = 0.0f;
		m_pCompModelAnime->SetIsVisible(true);	// モデルの表示ON
	}
}

/* ========================================
	ゲッター(HP)関数
	-------------------------------------
	戻値：int	HP
=========================================== */
int ObjectEnemy::GetHp()
{
	return m_nHp;
}

/* ========================================
	ゲッター(最大HP)関数
	-------------------------------------
	戻値：int	最大HP
=========================================== */
int ObjectEnemy::GetMaxHp()
{
	return m_nMaxHp;
}

/* ========================================
	ゲッター(無敵時間)関数
	-------------------------------------
	戻値：bool	無敵時間フラグ
=========================================== */
bool ObjectEnemy::GetInvincible()
{
	return m_bInvincible;
}

/* ========================================
	セッター(HP)関数
	-------------------------------------
	引数：int	HP
=========================================== */
void ObjectEnemy::SetHp(int hp)
{
	m_nHp = hp;
}

/* ========================================
	セッター(最大HP)関数
	-------------------------------------
	引数：int	最大HP
=========================================== */
void ObjectEnemy::SetMaxHp(int maxHp)
{
	m_nMaxHp = maxHp;
}

/* ========================================
	セッター(無敵時間)関数
	-------------------------------------
	引数：bool	無敵時間フラグ
=========================================== */
void ObjectEnemy::SetInvincible(bool bInvincible)
{
	m_bInvincible = bInvincible;
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemy::OutPutLocalData(std::ofstream& file)
{
	S_SaveDataEnemy data;

	// HP
	data.nMaxHp = m_nMaxHp;

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveDataEnemy));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemy::InputLocalData(std::ifstream& file)
{
	S_SaveDataEnemy data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveDataEnemy));

	// HP
	m_nMaxHp = data.nMaxHp;
}