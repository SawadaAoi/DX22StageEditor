/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ゴール)用cpp
	------------------------------------
	ObjectGoal.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGoal.h"
#include "ComponentBillboard.h"
#include "ComponentCollisionSphere.h"
#include "TextureManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectGoal::ObjectGoal(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_bIsGoal(false)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectGoal::InitLocal()
{
	// TODO: とりあえず仮のテクスチャを設定
	AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::STAGE_GOAL));
	AddComponent<ComponentCollisionSphere>()->SetTrigger(true);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectGoal::UpdateLocal()
{
	// ゴールしたら非表示にする
	if (m_bIsGoal)
	{
		GetComponent<ComponentBillboard>()->SetActive(false);
	}

}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectGoal::OnCollisionEnter(ObjectBase* pHit)
{
	// プレイヤーと接触したらゴールフラグを立てる
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		m_bIsGoal = true;
	}
}

/* ========================================
	ゲッター(ゴールフラグ)関数
	-------------------------------------
	戻値：bool	ゴールフラグ
=========================================== */
bool ObjectGoal::GetIsGoal()
{
	return m_bIsGoal;
}

/* ========================================
	セッター(ゴールフラグ)関数
	-------------------------------------
	引数：bool	ゴールフラグ
=========================================== */
void ObjectGoal::SetIsGoal(bool bIsGoal)
{
	m_bIsGoal = bIsGoal;
}
