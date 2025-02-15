/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(コイン)用cpp
	------------------------------------
	ObjectCoin.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectCoin.h"

#include "ComponentBillboardAnime.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionSphere.h"
#include "ComponentGroundRaycast.h"
#include "ComponentTransform.h"

#include "TextureManager.h"

// =============== 定数 =======================
const int			ANIME_FRAME_MAX = 18;	// アニメーションフレーム数
const Vector2<int>	ANIME_ROW_COL	= Vector2<int>(6, 3);	// アニメーション行列数
const float			ANIME_SPEED		= 0.5f;	// アニメーション速度

const float			COIN_SCALE		= 0.6f;	// コインの拡大率

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectCoin::ObjectCoin(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompBillboardAnime(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompCollisionSphere(nullptr)
	, m_pCompGroundRaycast(nullptr)
{
	SetTag(E_ObjectTag::Coin);
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：オブジェクトの初期化
=========================================== */
void ObjectCoin::InitLocal()
{
	m_pCompBillboardAnime	= AddComponent<ComponentBillboardAnime>();
	m_pCompRigidbody		= AddComponent<ComponentRigidbody>();
	m_pCompCollisionSphere	= AddComponent<ComponentCollisionSphere>();
	m_pCompGroundRaycast	= AddComponent<ComponentGroundRaycast>();

	// テクスチャ設定
	m_pCompBillboardAnime->SetTexture(GET_TEXTURE_DATA(TEX_KEY::ITEM_COIN));
	m_pCompBillboardAnime->Play(ANIME_FRAME_MAX, ANIME_ROW_COL, ANIME_SPEED, true);

	// 衝突判定設定
	m_pCompCollisionSphere->SetTrigger(true);
	m_pCompCollisionSphere->SetDynamic(false);

	// 大きさ設定
	m_pCompTransform->Scale(Vector3<float>(COIN_SCALE, COIN_SCALE, COIN_SCALE));
}

/* ========================================
	更新関数
	-------------------------------------
	内容：オブジェクトの更新処理
=========================================== */
void ObjectCoin::UpdateLocal()
{
	// 地面判定
	// 地面に設置している場合
	if (m_pCompGroundRaycast->GetHitFlg())
	{
		// 地面に設置している位置に高さを合わせる
		Vector3<float> vPos = m_pCompTransform->GetPosition();
		float fAdjustHeight = m_pCompGroundRaycast->GetHitPos().y + (m_pCompTransform->GetScale().y * 0.5f) - vPos.y;
		m_pCompTransform->TranslateY(fAdjustHeight);
	}
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectCoin::OnCollisionEnter(ObjectBase* pHit)
{
	// プレイヤーと接触したら
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		Destroy();	// コイン削除
	}
}
