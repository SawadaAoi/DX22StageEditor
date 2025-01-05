/* ========================================
	DX22/Dropin
	------------------------------------
	アニメモデルデータ管理用ヘッダ
	------------------------------------
	説明:アニメモデルのボーン、各アニメーションをまとめて読み込んで管理する
	------------------------------------
	AnimeModelManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ModelAnime.h"
#include "Shader.h"
#include <vector>
#include <unordered_map>
#include <memory>

// =============== 定数定義 =======================
#define MODEL_ANIME_MNG_INST	ModelAnimeManager::GetInstance()	// インスタンス取得

#define GET_MODEL_ANIME ModelAnimeManager::GetInstance().GetModelData	// インスタンス取得

#define ANIME_BASE_KEY			ModelAnimeManager::E_ANIME_BASE_KEY		// アニメーションモデル取得キー省略
#define ANIME_KEY_PLAYER		ModelAnimeManager::E_ANIME_KEY_PLAYER // プレイヤーアニメーション種類取得キー省略
#define ANIME_KEY_ENEMY_MOUSE	ModelAnimeManager::E_ANIME_KEY_ENEMY_MOUSE	// 敵アニメーション種類取得キー省略

class ModelAnimeManager
{
public:
	// モデルデータ種類
	enum E_ANIME_BASE_KEY
	{
		AB_PLAYER,			// プレイヤー(猫)
		AB_ENEMY_MOUSE,		// 敵(ネズミ)
		AB_MAX,
	};

	enum E_ANIME_KEY_PLAYER
	{
		PLYR_IDLE,		// 待機
		PLYR_WALK,		// 歩行
		PLYR_SHOT,		// ショット
		PLYR_MAX,		
	};

	enum E_ANIME_KEY_ENEMY_MOUSE
	{
		ENMYM_IDLE,		// 待機
		ENMYM_WALK,		// 歩行
		ENMYM_MAX,
	};


public:
	static ModelAnimeManager& GetInstance();
	void Init();

	// ゲッター
	ModelAnime* GetModelData(E_ANIME_BASE_KEY e_GetKey);

	std::unordered_map <E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> GetModelDatas();
	std::unordered_map <E_ANIME_BASE_KEY, std::string> GetModelPaths();
	std::unordered_map <int, std::string> GetAnimePaths(E_ANIME_BASE_KEY e_GetKey);

private:
	ModelAnimeManager();// コンストラクタ
	void LoadAnimeData(E_ANIME_BASE_KEY e_GetKey, std::unordered_map<int, std::string> dataPath);	// アニメーションデータ読み込み

	std::unordered_map <E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> m_apAnimeModelDatas;	// モデルデータを格納しておく
	bool m_bIsInit;	// 初期化フラグ


};
