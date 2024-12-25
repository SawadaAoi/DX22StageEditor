/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ゲーム状態管理)用cpp
	------------------------------------
	ObjectGameStateManager.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGameStateManager.h"
#include "SceneBase.h"
#include "ObjectBase.h"
#include "ObjectGoal.h"
#include "ObjectPlayer.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectGameStateManager::ObjectGameStateManager(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_eGameState(E_GameState::GS_NORMAL)
	, m_eClearType(E_ClearType::CT_GOAL)
	, m_pPlayer(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================== */
void ObjectGameStateManager::InitLocal()
{
	// プレイヤーオブジェクトを取得
	std::vector<ObjectBase*> pPlayerList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Player);
	m_pPlayer = static_cast<ObjectPlayer*>(pPlayerList[0]);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================== */
void ObjectGameStateManager::UpdateLocal()
{
	if (!m_pPlayer)
	{
		// プレイヤーオブジェクトを取得
		std::vector<ObjectBase*> pPlayerList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Player);
		m_pPlayer = static_cast<ObjectPlayer*>(pPlayerList[0]);
		return;
	}

	// ゲーム状態が通常以外の場合はチェックしない
	if (m_eGameState == E_GameState::GS_NORMAL)
	{
		CheckClear();
		CheckGameOver();
	}

#ifdef _DEBUG
	if (CHECK_DISP_OBJ("GameStateMng"))
	{
		SetDebugGameState();
		SetDebugClearType();
	}

#endif // _DEBUG


}

/* ========================================
	ゲームクリアチェック関数
	-------------------------------------
	内容：ゲームクリア条件をチェック
======================================== */
void ObjectGameStateManager::CheckClear()
{
	bool bClear = false;	// クリアフラグ

	// クリア条件によってチェック関数を変更
	switch (m_eClearType)
	{
	case E_ClearType::CT_GOAL:			bClear = CheckClearGoal();		break;
	case E_ClearType::CT_KILL_ENEMY:	bClear = CheckClearKillEnemy();	break;
	}

	// クリア条件を満たしていたらゲームクリア
	if (bClear)
		m_eGameState = E_GameState::GS_GAMECLEAR;
}

/* ========================================
	ゴールクリアチェック関数
	-------------------------------------
	内容：ゴールオブジェクトのクリア状態をチェック
	-------------------------------------
	戻り値：true = クリア / false = 未クリア
======================================== */
bool ObjectGameStateManager::CheckClearGoal()
{
	std::vector<ObjectBase*> pGoalList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Goal);

	if (pGoalList.size() == 0) return false;

	// ゴールオブジェクトが全てゴールしているかチェック
	for (auto pGoal : pGoalList)
	{
		// ゴールオブジェクトにキャスト
		ObjectGoal* pGoalObj = static_cast<ObjectGoal*>(pGoal);

		// 一つでもゴールしていないオブジェクトがあればfalse
		if (!pGoalObj->GetIsGoal()) return false;
	}

	// 全てゴールしていればtrue
	return true;
}

/* ========================================
	敵全滅クリアチェック関数
	-------------------------------------
	内容：敵オブジェクトが全滅しているかチェック
	-------------------------------------
	戻り値：true = クリア / false = 未クリア
======================================== */
bool ObjectGameStateManager::CheckClearKillEnemy()
{
	std::vector<ObjectBase*> pEnemyList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Enemy);

	if (pEnemyList.size() == 0) return true;
	else return false;
}

/* ========================================
	ゲームオーバーチェック関数
	-------------------------------------
	内容：ゲームオーバー条件をチェック
======================================== */
void ObjectGameStateManager::CheckGameOver()
{
	// プレイヤーのHPが0以下になったらゲームオーバー
	if (m_pPlayer->GetHp() <= 0)
	{
		m_eGameState = E_GameState::GS_GAMEOVER;
	}
}


/* ========================================
	ゲッター(ゲーム状態)関数
	-------------------------------------
	戻値：E_GameState	ゲーム状態
=========================================== */
ObjectGameStateManager::E_GameState ObjectGameStateManager::GetGameState()
{
	return m_eGameState;
}

/* ========================================
	ゲッター(クリア条件)関数
	-------------------------------------
	戻値：E_ClearType	クリア条件
=========================================== */
ObjectGameStateManager::E_ClearType ObjectGameStateManager::GetClearType()
{
	return m_eClearType;
}

/* ========================================
	セッター(ゲーム状態)関数
	-------------------------------------
	引数1：E_GameState	ゲーム状態
=========================================== */
void ObjectGameStateManager::SetGameState(E_GameState eState)
{
	m_eGameState = eState;
}

/* ========================================
	セッター(クリア条件)関数
	-------------------------------------
	引数1：E_ClearType	クリア条件
=========================================== */
void ObjectGameStateManager::SetClearType(E_ClearType eType)
{
	m_eClearType = eType;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectGameStateManager::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGameStateMng = Item::CreateGroup("GameStateMng");

	pGroupGameStateMng->AddGroupItem(Item::CreateValue("GameState", Item::Kind::Text));
	pGroupGameStateMng->AddGroupItem(Item::CreateValue("ClearType", Item::Kind::Text));

	window.AddItem(pGroupGameStateMng);
	
	SetDebugGameState();
	SetDebugClearType();
}

/* ========================================
	デバッグゲーム状態セット関数
	-------------------------------------
	内容：デバッグ用のゲーム状態を設定
======================================== */
void ObjectGameStateManager::SetDebugGameState()
{
	using namespace DebugUI;

	Window& window = WIN_OBJ_INFO;

	// ゲーム状態
	switch (m_eGameState)
	{
	case E_GameState::GS_NORMAL:	window["GameStateMng"]["GameState"].SetText("Normal");	break;
	case E_GameState::GS_PAUSE:		window["GameStateMng"]["GameState"].SetText("Pause");		break;
	case E_GameState::GS_GAMEOVER:	window["GameStateMng"]["GameState"].SetText("GameOver");	break;
	case E_GameState::GS_GAMECLEAR:	window["GameStateMng"]["GameState"].SetText("GameClear");	break;
	}
}

/* ========================================
	デバッグクリア条件セット関数
	-------------------------------------
	内容：デバッグ用のクリア条件を設定
======================================== */
void ObjectGameStateManager::SetDebugClearType()
{
	using namespace DebugUI;

	Window& window = WIN_OBJ_INFO;

	// クリア条件
	switch (m_eClearType)
	{
	case E_ClearType::CT_GOAL:			window["GameStateMng"]["ClearType"].SetText("Goal");		break;
	case E_ClearType::CT_KILL_ENEMY:	window["GameStateMng"]["ClearType"].SetText("KillEnemy");	break;
	}
}
#endif // _DEBUG