/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(破壊可能ブロック)用cpp
	------------------------------------
	ObjectBlockBreakable.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBlockBreakable.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGeometry.h"
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"

#include "ObjectExplosion.h"

#include "SceneBase.h"
#include "TextureManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================== */
ObjectBlockBreakable::ObjectBlockBreakable(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_nHp(1)
	, m_nHpOld(0)
{
	SetTag(E_ObjectTag::Ground);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================== */
void ObjectBlockBreakable::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// 地面判定
	m_eColType = E_COL_TYPE::COL_AABB;	// 衝突判定形状
	ObjectBlock::InitLocal();			// 親クラスの初期化関数を呼ぶ

	m_pCompGeometry->SetTextureAll(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_BRICK));
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理を行う
========================================== */
void ObjectBlockBreakable::UpdateLocal()
{
	if (m_nHp != m_nHpOld)
	{
		// HPが1の時、テクスチャを亀裂状態に変更
		if (m_nHp == 1)
			m_pCompGeometry->SetTextureAll(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_BRICK_CRACK));

		m_nHpOld = m_nHp;	// HPを更新
	}
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数1：衝突相手オブジェクト
========================================== */
void ObjectBlockBreakable::OnCollisionEnter(ObjectBase* pHit)
{
	// 弾に当たった時
	if (pHit->GetTag() == E_ObjectTag::PlayerBullet)
	{
		m_nHp--;

		if (m_nHp <= 0)
		{
			Destroy();	// 自身を削除

			// 爆発エフェクト生成(仮)
			ObjectBase* pExplosion = m_pOwnerScene->AddSceneObject<ObjectExplosion>("Explosion_" + m_sName);
			pExplosion->GetTransform()->SetPosition(m_pCompTransform->GetPosition());
		}
	}
}
