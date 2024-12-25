/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ゲーム状態管理)用ヘッダ
	------------------------------------
	説明：ゲームシーンの状態を管理する
		　※ゲーム中、ポーズ中、ゲームオーバー、ゲームクリア
	------------------------------------
	ObjectGameStateManager.h
========================================== */
#pragma once

// ToDo : ポーズ中の処理を追加する

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ObjectPlayer;

// =============== クラス定義 =====================
class ObjectGameStateManager :
    public ObjectBase
{
public:
	// ゲーム状態
	enum E_GameState
	{
		GS_NORMAL,		// ゲーム中
		GS_PAUSE,		// ポーズ中
		GS_GAMEOVER,	// ゲームオーバー
		GS_GAMECLEAR,	// ゲームクリア
	};

	// クリア条件
	enum E_ClearType
	{
		CT_GOAL,		// ゴール
		CT_KILL_ENEMY,	// 敵全滅
	};
public:
	ObjectGameStateManager(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// ゲッター
	E_GameState GetGameState();
	E_ClearType GetClearType();

	// セッター
	void SetGameState(E_GameState eState);
	void SetClearType(E_ClearType eType);

	DEFINE_OBJECT_TYPE(ObjectGameStateManager)	// オブジェクトの種類ID取得関数
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window) override;
	void SetDebugGameState();
	void SetDebugClearType();
#endif // _DEBUG

private:
	void CheckClear();
	bool CheckClearGoal();
	bool CheckClearKillEnemy();
	void CheckGameOver();
private:
	E_GameState m_eGameState;	// ゲーム状態
	E_ClearType m_eClearType;	// クリア条件

	ObjectPlayer* m_pPlayer;	// プレイヤーオブジェクト


};

