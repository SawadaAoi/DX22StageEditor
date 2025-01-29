/* ========================================
	CatRobotGame/
	------------------------------------
	弾オブジェクト(基底)用cpp
	------------------------------------
	ObjectBullet.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectBulletPlayer.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "TextureManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
=========================================== */
ObjectBulletPlayer::ObjectBulletPlayer(SceneBase* pScene)
	: ObjectBullet(pScene)
{
	SetTag(E_ObjectTag::PlayerBullet);
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void ObjectBulletPlayer::InitLocal()
{
	ObjectBullet::InitLocal();
	AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::PLAYER_BULLET));

	m_pCompTransform->SetScale({ 0.5f, 0.5f, 0.5f });
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数1：衝突相手オブジェクト
=========================================== */
void ObjectBulletPlayer::OnCollisionEnter(ObjectBase* pHit)
{
	if (pHit->GetTag() == E_ObjectTag::Player
		|| pHit->GetTag() == E_ObjectTag::PlayerBullet)
	{
		return;
	}

	if (pHit->GetTag() == E_ObjectTag::Enemy)
	{
		pHit->OnCollisionStay(this);
	}
	
	SetState(E_State::STATE_DEAD);
}

