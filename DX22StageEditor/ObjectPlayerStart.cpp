/* ========================================
	CatRobotGame/
	------------------------------------
	プレイヤー開始地点用cpp
	------------------------------------
	ObjectPlayerStart.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectPlayerStart.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "ComponentCollisionAABB.h"
#include "TextureManager.h"
#include "ObjectPlayer.h"
#include "SceneBase.h"

// =============== 定数定義 =======================
const std::string PLAYER_OBJECT_NAME = "Player";


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
========================================== */
ObjectPlayerStart::ObjectPlayerStart(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_bIsPlayerStart(false)
{
	// タグ設定
	SetTag(E_ObjectTag::PlayerStart);
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================= */
void ObjectPlayerStart::InitLocal()
{
	// 位置設定
	GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

#ifdef _DEBUG
	// ビルボード設定
	ComponentBillboard* pBillboard = AddComponent<ComponentBillboard>();
	pBillboard->SetTexture(GET_TEXTURE_DATA(TEX_KEY::DEBUG_PLAYER_START));

	// 当たり判定(位置を視認しやすくするため)
	ComponentCollisionAABB* pCollision = AddComponent<ComponentCollisionAABB>();
	pCollision->SetTrigger(true);
#endif // _DEBUG
	
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================= */
void ObjectPlayerStart::UpdateLocal()
{
	// 開始フラグが立っている場合はプレイヤーを生成
	if (m_bIsPlayerStart)
	{
		ObjectBase* pPlayer = m_pOwnerScene->AddSceneObject<ObjectPlayer>(PLAYER_OBJECT_NAME);
		pPlayer->GetTransform()->SetPosition(GetTransform()->GetLocalPosition());			// 自身の位置にプレイヤーを生成

		SetState(E_State::STATE_DEAD);	// 自身を削除
	}
}

/* ========================================
	ゲッター(プレイヤー開始フラグ)関数
	-------------------------------------
	戻値：bool	プレイヤー開始フラグ
=========================================== */
bool ObjectPlayerStart::GetIsPlayerStart() const
{
	return m_bIsPlayerStart;
}

/* ========================================
	セッター(プレイヤー開始フラグ)関数
	-------------------------------------
	引数1：bool	プレイヤー開始フラグ
=========================================== */
void ObjectPlayerStart::SetIsPlayerStart(bool bIsPlayerStart)
{
	m_bIsPlayerStart = bIsPlayerStart;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectPlayerStart::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupPlayerStart = Item::CreateGroup("PlayerStart");

	pGroupPlayerStart->AddGroupItem(Item::CreateBind("IsPlayerStart", Item::Kind::Bool, &m_bIsPlayerStart));

	window.AddItem(pGroupPlayerStart);
}

#endif // _DEBUG