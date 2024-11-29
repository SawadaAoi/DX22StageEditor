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
	{ANIME_BASE_KEY::AB_PLAYER, "Assets/Model/Player/Player_Base2.fbx" },	// プレイヤー
	{ANIME_BASE_KEY::AB_ENEMY_G1, "Assets/Model/Enemy/Enemy_G1/Enemy_G1_Base.fbx" },	// 敵G1
	{ANIME_BASE_KEY::AB_ENEMY_Y2, "Assets/Model/Enemy/Enemy_Y2/Enemy_Y2_Base.fbx" },	// 敵Y2
	{ANIME_BASE_KEY::AB_ENEMY_R3, "Assets/Model/Enemy/Enemy_R3/Enemy_R3_Base.fbx" },	// 敵R3
};

// 各モデルデータ大きさ指定
const std::unordered_map < ANIME_BASE_KEY, float > BASE_MODEL_FBX_SCALE
{
	{ANIME_BASE_KEY::AB_PLAYER,	0.7f},
	{ANIME_BASE_KEY::AB_ENEMY_G1,	1.0f},
	{ANIME_BASE_KEY::AB_ENEMY_Y2,	1.0f},
	{ANIME_BASE_KEY::AB_ENEMY_R3,	1.0f},
};

// プレイヤーアニメーションデータ保存先パス
const std::unordered_map < int, std::string > ANIME_DATA_PATH_PLAYER
{
	{ANIME_KEY_PLAYER::PLYR_IDLE		, "Assets/Model/Player/Player_Idle.fbx" },	// 待機
	{ANIME_KEY_PLAYER::PLYR_WALK		, "Assets/Model/Player/Player_Walk.fbx" },	// 歩行
	{ANIME_KEY_PLAYER::PLYR_SHOT		, "Assets/Model/Player/Player_Shot.fbx" },	// 射撃
	{ANIME_KEY_PLAYER::PLYR_WALK_SHOT	, "Assets/Model/Player/Player_Walk_Shot.fbx" },	// 歩行射撃
	{ANIME_KEY_PLAYER::PLYR_GAMECLEAR	, "Assets/Model/Player/Player_GameClear.fbx" },	// ゲームクリア
};

// 敵キャラアニメーションデータ保存先パス
const std::unordered_map < int, std::string > ANIME_DATA_ENEMY_PATH
{
	{ANIME_KEY_ENEMY::ENMY_IDLE		, "Assets/Model/Enemy/Enemy_Idle.fbx" },	// 待機
	{ANIME_KEY_ENEMY::ENMY_WALK		, "Assets/Model/Enemy/Enemy_Walk.fbx" },	// 歩行
	{ANIME_KEY_ENEMY::ENMY_NOT_ANIME	, "Assets/Model/Enemy/Enemy_NotAnime.fbx" },	// アニメーション無し(滑走状態)
	{ANIME_KEY_ENEMY::ENMY_VIBRATION	, "Assets/Model/Enemy/Enemy_Vibration.fbx" },	// 振動(停止状態)

};

// アニメーションデータマップ紐づけ
// モデルデータキーと各アニメーションマップを紐づける
const std::unordered_map < ANIME_BASE_KEY, std::unordered_map < int, std::string > > ANIME_DATA_MAP_PATH
{
	{ANIME_BASE_KEY::AB_PLAYER, ANIME_DATA_PATH_PLAYER},
	{ANIME_BASE_KEY::AB_ENEMY_G1, ANIME_DATA_ENEMY_PATH},
	{ANIME_BASE_KEY::AB_ENEMY_Y2, ANIME_DATA_ENEMY_PATH},
	{ANIME_BASE_KEY::AB_ENEMY_R3, ANIME_DATA_ENEMY_PATH},
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
		m_apAnimeModelDatas.at(e_GetKey)->AddAnimation(PathMap.second.c_str());

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


