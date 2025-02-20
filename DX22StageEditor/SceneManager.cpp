/* ========================================
	DX22Base/
	------------------------------------
	シーン管理用cpp
	------------------------------------
	SceneManager.cpp
========================================== */

// =============== インクルード =====================
#include "SceneManager.h"
#include "SceneBase.h"

#include "RegisterAllScene.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
void SceneManager::Init()
{
#ifndef _DEBUG
	ChangeScene("SceneStageSave_1", 0.0f);
#endif
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：削除時
=========================================== */
void SceneManager::Uninit()
{
	m_pScene->Uninit();	// 終了処理
}


/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void SceneManager::Update()
{
	m_pScene->Update();	// 更新処理
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void SceneManager::Draw()
{
	m_pScene->Draw();	// 描画処理

	// シーン変更処理が指示されていたら
	if (m_isSceneChange)	CommitSceneChange();
}
/* ========================================
	シーン変更予約関数
	-------------------------------------
	内容：シーンを変更を予約する
	-------------------------------------
	引数1：シーン名
	引数2：フェード時間
=========================================== */
void SceneManager::ChangeScene(const std::string& sSceneName, float fFadeTime)
{
	if (m_isSceneChange) return;	// シーン変更中は処理しない

	m_sNextSceneName = sSceneName;	// 次のシーン名を設定
	m_isSceneChange = true;			// シーン再読み込みフラグを立てる

	// シーンが存在していない場合は即座に変更
	if (!m_pScene)
	{
		InitScene();
		return;
	}

}

/* ========================================
	シーン再読み込み予約関数
	-------------------------------------
	内容：シーン再読み込みを予約する
	-------------------------------------
	引数1：フェード時間
=========================================== */
void SceneManager::ReloadScene(float fFadeTime)
{
	if (m_isSceneChange) return;	// シーン変更中は処理しない

	m_sNextSceneName = m_pScene->GetSceneName();	// 現在のシーン名を再設定
	m_isSceneChange = true;						// シーン再読み込みフラグを立てる
}


/* ========================================
	シーン変更実行関数
	-------------------------------------
	内容：シーン変更を実行する
=========================================== */
void SceneManager::CommitSceneChange()
{
	// シーンが存在していたら終了処理を行う
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();  // 終了処理
	}

	// 新しいシーンを生成
	m_pScene = m_SceneMap[m_sNextSceneName]();

	m_pScene->Init();  // 初期化処理

	m_isSceneChange = false;	// シーン再読み込みフラグを下げる

}


/* ========================================
	シーン初期化関数
	-------------------------------------
	内容：シーン初期化
=========================================== */
void SceneManager::InitScene()
{
	// シーンが存在していない場合は即座に変更
	CommitSceneChange();

}



/* ========================================
	シーン登録関数
	-------------------------------------
	内容：シーンを登録する
=========================================== */
void SceneManager::RegisterScene(const std::string& sSceneName, CreateSceneFunc func)
{
	m_SceneMap[sSceneName] = func;
}

/* ========================================
	ゲッター(シーン)関数
	-------------------------------------
	戻値：SceneBase*		シーン
=========================================== */
SceneBase* SceneManager::GetScene()
{
	return m_pScene.get();
}

/* ========================================
	ゲッター(シーン名リスト)関数
	-------------------------------------
	戻値：std::vector<std::string>		シーン名リスト
=========================================== */
std::vector<std::string> SceneManager::GetSceneNameList()
{
	std::vector<std::string> sSceneNameList;
	for (auto& scene : m_SceneMap)
	{
		sSceneNameList.push_back(scene.first);
	}
	return sSceneNameList;
}

