/* ========================================
	CatRobotGame/
	------------------------------------
	リスポーン用cpp
	------------------------------------
	ObjectRespawn.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectRespawn.h"
#include "ObjectPlayer.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "ComponentTransform.h"
#include "ComponentCollisionAABB.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
=========================================== */
ObjectRespawn::ObjectRespawn(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_RespawnType(E_RespawnType::RT_RELOAD_SCENE)
	, m_RespawnPosition(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_pCompColAABB(nullptr)
	, m_IsReloadScene(false)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void ObjectRespawn::InitLocal()
{
	// 当たり判定設定
	m_pCompColAABB = AddComponent<ComponentCollisionAABB>();
	m_pCompColAABB->SetTrigger(true);
	m_pCompColAABB->SetDynamic(false);

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新
=========================================== */
void ObjectRespawn::UpdateLocal()
{
	// シーンを再読み込み
	if (m_IsReloadScene)	SceneManager::ReloadScene();
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectRespawn::OnCollisionEnter(ObjectBase* pHit)
{
	// プレイヤーと当たった場合
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		// リスポーン種類によって処理を変える
		switch (m_RespawnType)
		{
		// シーンを再読み込み
		case E_RespawnType::RT_RELOAD_SCENE:
			// 当たり判定処理内でシーン再読み込み行うと、自身の所有シーンが破棄されるため、
			// フラグを立ててUpdate処理で再読み込みする
			m_IsReloadScene = true;
			break;
		// 決まった位置からリスポーン
		case E_RespawnType::RT_POSITION:	
			pHit->GetTransform()->SetPosition(m_RespawnPosition);
			break;
		}
	}
}


/* ========================================
	ゲッター(リスポーン種類)関数
	-------------------------------------
	戻値：リスポーン種類
=========================================== */
ObjectRespawn::E_RespawnType ObjectRespawn::GetRespawnType() const
{
	return m_RespawnType;
}


/* ========================================
	ゲッター(リスポーン位置)関数
	-------------------------------------
	戻値：リスポーン位置
=========================================== */
Vector3<float> ObjectRespawn::GetRespawnPosition() const
{
	return m_RespawnPosition;
}

/* ========================================
	セッター(リスポーン種類)関数
	-------------------------------------
	引数：リスポーン種類
=========================================== */
void ObjectRespawn::SetRespawnType(E_RespawnType type)
{
	m_RespawnType = type;
}

/* ========================================
	セッター(リスポーン位置)関数
	-------------------------------------
	引数：リスポーン位置
=========================================== */
void ObjectRespawn::SetRespawnPosition(const Vector3<float>& pos)
{
	m_RespawnPosition = pos;
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectRespawn::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// リスポーン種類
	data.eRespawnType = m_RespawnType;
	// リスポーン位置
	data.vRespawnPosition = m_RespawnPosition;

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectRespawn::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// リスポーン種類
	m_RespawnType = data.eRespawnType;
	// リスポーン位置
	m_RespawnPosition = data.vRespawnPosition;
}




#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectRespawn::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupRespawn = Item::CreateGroup("Respawn");

	// リスポーン種類
	pGroupRespawn->AddGroupItem(Item::CreateList("RespawnType", [this](const void* arg)
	{
		int nNum = WIN_OBJ_INFO["Respawn"]["RespawnType"].GetInt();
		m_RespawnType = (E_RespawnType)nNum;

	}, false, true));

	// リスポーン位置
	pGroupRespawn->AddGroupItem(Item::CreateBind("RespawnPosition", Item::Kind::Vector, &m_RespawnPosition));

	window.AddItem(pGroupRespawn);

	// リスポーン種類リスト追加
	WIN_OBJ_INFO["Respawn"]["RespawnType"].AddListItem("ReloadScene");
	WIN_OBJ_INFO["Respawn"]["RespawnType"].AddListItem("Position");
	WIN_OBJ_INFO["Respawn"]["RespawnType"].SetListNo((int)m_RespawnType);

}

#endif // _DEBUG