/* ========================================
	DX22Base/
	------------------------------------
	アプリ動作用cpp
	------------------------------------
	AppController.cpp
========================================== */

// =============== インクルード ===================
#include "AppController.h"
#include "DirectXManager.h"		// DirectX管理			
#include "Input.h"
#include "ShaderManager.h"

#include "SceneManager.h"
#include "GridLine.h"
#include "CameraManager.h"
#include "TimeManager.h"
#include "ModelStaticManager.h"
#include "ModelAnimeManager.h"
#include "TextureManager.h"
#include "ObjectTypeRegistry.h"	// オブジェクトタイプ登録(ファイル読み込み)

#include "WindowAPI.h"

#include "DebugMenu.h"

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
HRESULT AppController::Init()
{
	HRESULT hr;

	// DirectX初期化
	if (FAILED(hr = DirectXManager::InitDirectX()))
	{
		return hr;
	}

	TimeManager::Init();	// 時間管理初期化

	SHADER_INST.Init();				// シェーダー読み込み
	MODEL_STATIC_MNG_INST.Init();	// 静的モデル管理初期化
	MODEL_ANIME_MNG_INST.Init();	// アニメーションモデル管理初期化
	TEXTURE_MNG_INST.Init();		// テクスチャ管理初期化

	SceneManager::RegisterAllScene();				// シーン登録
	ObjectTypeRegistry::RegisterAllObjectTypes();	// オブジェクトタイプ登録
#ifdef _DEBUG
	DebugUI::Menu::Init();
	DebugUI::InitDebugWindow();
#endif

	SceneManager::Init();	// シーン管理初期化

	GridLine::Init();	// グリッド線初期化


	return hr;
}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理を行う
=========================================== */
void AppController::Uninit()
{
	SceneManager::Uninit();	// シーン管理削除
#ifdef _DEBUG
	DebugUI::Menu::Uninit();
#endif

	DirectXManager::UninitDirectX();	// DirectX終了
}

/* ========================================
	更新処理関数
	-------------------------------------
	内容：更新を行う
	-------------------------------------
	引数1：前回のフレームからの経過時間
=========================================== */
void AppController::Update(float tick)
{

	TimeManager::Update(tick);	// 時間管理更新
	SceneManager::Update();


}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画を行う
=========================================== */
void AppController::Draw()
{
	DirectXManager::BeginDrawDirectX();	// ビュークリア
	GridLine::Draw();		// グリッド線描画
	SceneManager::Draw();	// シーン描画

#ifdef _DEBUG
	DebugUI::Menu::Draw();
#endif

	DirectXManager::EndDrawDirectX();	// バックバッファをフロントバッファにコピー

}


