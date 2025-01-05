/* ========================================
	DX22/Dropin
	------------------------------------
	アニメモデルデータ管理用cpp
	------------------------------------
	AnimeModelManager.cpp
========================================== */

// =============== インクルード ===================
#include "ModelAnimeManager.h"

// =============== 定数定義 =======================


// モデルデータ保存先パス
const std::unordered_map < ANIME_BASE_KEY, std::string > BASE_MODEL_FBX_PATH
{
	{ANIME_BASE_KEY::AB_PLAYER,			"Assets/Model/Anime/Player/Player_Base.fbx" },			// プレイヤー
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	"Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Base.fbx" },	// 敵Mouse
};

// 各モデルデータ大きさ指定
const std::unordered_map < ANIME_BASE_KEY, float > BASE_MODEL_FBX_SCALE
{
	{ANIME_BASE_KEY::AB_PLAYER,			0.4f},
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	0.4f},
};

// プレイヤー(猫)アニメーションデータ保存先パス
const std::unordered_map < int, std::string > ANIME_DATA_PLAYER_CAT_PATH
{
	{ANIME_KEY_PLAYER::PLYR_IDLE		, "Assets/Model/Anime/Player/Player_Idle.fbx" },	// 待機
	{ANIME_KEY_PLAYER::PLYR_WALK		, "Assets/Model/Anime/Player/Player_Walk.fbx" },	// 歩行
	{ANIME_KEY_PLAYER::PLYR_SHOT		, "Assets/Model/Anime/Player/Player_Shot.fbx" },	// ショット
};


// 敵キャラ(ネズミ)アニメーションデータ保存先パス
const std::unordered_map < int, std::string > ANIME_DATA_ENEMY_MOUSE_PATH
{
	{ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE		, "Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Idle.fbx" },	// 待機
	{ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK		, "Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Walk.fbx" },	// 歩行
	
};


// アニメーションデータマップ紐づけ
// モデルデータキーと各アニメーションマップを紐づける
const std::unordered_map < ANIME_BASE_KEY, std::unordered_map < int, std::string > > ANIME_DATA_MAP_PATH
{
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	ANIME_DATA_ENEMY_MOUSE_PATH},
	{ANIME_BASE_KEY::AB_PLAYER,		ANIME_DATA_PLAYER_CAT_PATH},
};




/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
========================================== */
ModelAnimeManager::ModelAnimeManager()
	: m_bIsInit(false)
{
	m_apAnimeModelDatas.clear();
	
}

/* ========================================
	インスタンス取得関数
	-------------------------------------
	内容：インスタンスを取得する
	-------------------------------------
	戻値：インスタンス
========================================== */
ModelAnimeManager& ModelAnimeManager::GetInstance()
{
	static ModelAnimeManager instance;
	return instance;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
========================================== */
void ModelAnimeManager::Init()
{
	// 既に初期化されていたら終了
	if (m_bIsInit) return;

	// モデルデータを読み込む
	for (std::pair<ANIME_BASE_KEY, std::string> PathMap : BASE_MODEL_FBX_PATH)
	{
		ANIME_BASE_KEY eNowKey = PathMap.first;	// 現在のキー
		std::string sNowPath = PathMap.second;	// 現在の保存位置パス

		ModelAnime* pModel = new ModelAnime();
		if (!pModel->Load(sNowPath.c_str(), BASE_MODEL_FBX_SCALE.at(eNowKey), ModelAnime::E_Flip::XFlip))	// 3Dモデル読み込み
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//エラーメッセージの表示
		}

		m_apAnimeModelDatas.insert(std::make_pair(eNowKey, pModel));	// モデルデータ配列に追加
	}

	// アニメーションオブジェクト種類分
	for (int i = 0; i < E_ANIME_BASE_KEY::AB_MAX; i++)
	{
		// アニメーションデータマップ取得
		std::unordered_map < int, std::string > dataPath = ANIME_DATA_MAP_PATH.at(static_cast<ANIME_BASE_KEY>(i));
		// アニメーションデータ読み込み
		LoadAnimeData(static_cast<ANIME_BASE_KEY>(i), dataPath);
	}



	m_bIsInit = true;
}


/* ========================================
	アニメーションデータ読み込み
	-------------------------------------
	引数1：モデルデータのキー
	引数2：アニメーションデータのパスマップ
	-------------------------------------
	内容：渡されたモデル種類キーに合わせて
		　アニメーションデータを読み込む
========================================== */
void ModelAnimeManager::LoadAnimeData(E_ANIME_BASE_KEY e_GetKey, std::unordered_map<int, std::string> dataPath)
{
	for(std::pair<int, std::string> PathMap : dataPath)
	{
		if (m_apAnimeModelDatas.at(e_GetKey)->AddAnimation(PathMap.second.c_str()) == ModelAnime::ANIME_NONE)
		{
			MessageBox(NULL, PathMap.second.c_str(), "Error", MB_OK);	//エラーメッセージの表示
		}

	}

}


/* ========================================
	モデルデータ取得関数
	-------------------------------------
	内容：キーを元に該当のモデルデータを渡す
		　※ディープコピー
	-------------------------------------
	引数1：キー
	-------------------------------------
	戻値：モデルポインタ
========================================== */
ModelAnime* ModelAnimeManager::GetModelData(E_ANIME_BASE_KEY e_GetKey)
{
	return new ModelAnime(*m_apAnimeModelDatas.at(e_GetKey).get());
	//return m_apAnimeModelDatas.at(e_GetKey).get();
}

/* ========================================
	ゲッター(アニメーションモデル配列)関数
	-------------------------------------
	戻値：std::unordered_map<E_MODEL_KEY, std::shared_ptr<ModelStatic>>	モデルデータ配列
=========================================== */
std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> ModelAnimeManager::GetModelDatas()
{
	return m_apAnimeModelDatas;
}

/* ========================================
	ゲッター(モデルデータパス)関数
	-------------------------------------
	戻値：std::unordered_map<E_MODEL_KEY, std::string>	モデルデータパス
========================================== */
std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> ModelAnimeManager::GetModelPaths()
{
	return BASE_MODEL_FBX_PATH;
}

/* ========================================
	ゲッター(アニメーションデータパス)関数
	-------------------------------------
	引数1：キー
	-------------------------------------
	戻値：std::unordered_map<int, std::string>	アニメーションデータパス
========================================== */
std::unordered_map<int, std::string> ModelAnimeManager::GetAnimePaths(E_ANIME_BASE_KEY e_GetKey)
{
	return ANIME_DATA_MAP_PATH.at(e_GetKey);
}


