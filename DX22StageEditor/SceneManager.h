/* ========================================
	DX22Base/
	------------------------------------
	シーン管理用ヘッダ
	------------------------------------
	説明：各シーンの管理を行うクラス
	------------------------------------
	SceneManager.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "SceneBase.h"

// =============== クラス定義 =====================
class SceneManager
{
public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	// シーン切り替え
	template<typename T>
	static void ChangeScene();

	// シーン再読み込み
	static void CallReloadScene();	// シーン再読み込みを呼び出す
	static void ReloadScene();		// シーン再読み込み

	// ゲッター
	static SceneBase* GetScene();	// シーン取得
	static bool GetIsReloadScene();	// シーン再読み込みフラグ取得

private:
	static inline std::unique_ptr<SceneBase> m_pScene = nullptr;	// std::unique_ptrに変更
	static inline bool	m_IsReloadScene = false;					// シーン再読み込み処理呼び出しフラグ
};

// =============== テンプレート関数の実装 =====================


/* ========================================
	シーン変更関数
	-------------------------------------
	内容：シーンを変更する
========================================== */
template<typename T>
inline void SceneManager::ChangeScene()
{
	// シーンが存在していたら終了処理を行う
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();  // 終了処理
		// deleteは不要：unique_ptrが自動でメモリ解放を行う
	}

	// 新しいシーンを生成（unique_ptrが管理する）
	m_pScene = std::make_unique<T>();  // メモリ解放を自動化
	m_pScene->Init();  // 初期化処理

}