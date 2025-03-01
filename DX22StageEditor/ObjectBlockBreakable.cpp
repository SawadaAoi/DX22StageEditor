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

/* ========================================
	コピー関数(個別処理)
	-------------------------------------
	内容：オブジェクト個別のコピー処理
	-------------------------------------
	引数1：コピーされたオブジェクト
=========================================== */
void ObjectBlockBreakable::CopyLocal(ObjectBase* pObject)
{
	__super::CopyLocal(pObject);	// 親クラスのコピー関数を呼ぶ

	ObjectBlockBreakable* pObj = dynamic_cast<ObjectBlockBreakable*>(pObject);

	// HP
	pObj->m_nHp = m_nHp;
}


/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBlockBreakable::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// HP
	data.nHp = m_nHp;

	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBlockBreakable::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	file.read((char*)&data, sizeof(S_SaveData));

	// HP
	m_nHp = data.nHp;
}

/* ========================================
	セッター(HP)関数
	-------------------------------------
	引数：int HP
=========================================== */
void ObjectBlockBreakable::SetHp(int nHp)
{
	m_nHp = nHp;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectBlockBreakable::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupBlockBreakable = Item::CreateGroup("BlockBreakable");
	// HP
	pGroupBlockBreakable->AddGroupItem(Item::CreateBind("HP", Item::Kind::Int, &m_nHp));

	window.AddItem(pGroupBlockBreakable);
}
#endif // _DEBUG