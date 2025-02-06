/* ========================================
	DX22/Dropin
	------------------------------------
	画像データ管理用ヘッダ
	------------------------------------
	説明:画像データをまとめて読み込んで管理する
	------------------------------------
	TextureManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Texture.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

// =============== 定数定義 =======================
#define TEX_KEY				TextureManager::E_TEX_KEY	// キー省略
#define GET_TEXTURE_DATA	TextureManager::GetInstance().GetTextureData	// 文章省略
#define TEXTURE_MNG_INST TextureManager::GetInstance()	// インスタンス取得

class TextureManager
{
public:
	// モデルデータ種類
	enum E_TEX_KEY
	{
		TEST,				// テスト画像
		TEST2,				// 
		CAMERA_ICON,		// デバッグ用カメラアイコン
		LIGHT_ICON,			// デバッグ用ライトアイコン
		BLOCK_SIMPLE,		// シンプルなブロック
		GROUND_GRASS,		// 草地
		PLAYER_HP,			// プレイヤーHP
		PLAYER_BULLET,		// プレイヤー弾
		STAGE_GOAL,			// ゴール
		WALL_FENCE,			// 壁(フェンス)
		PLAYER_STARTPOS,	// プレイヤースタート位置
		TITLE_LOGO,			// タイトルロゴ
		EFFECT_EXPLOSION,	// 爆発エフェクト
		EFFECT_SMOKE,		// 煙エフェクト
		MAX,
	};

	// 画像データ保存先パス
	const std::unordered_map < TextureManager::E_TEX_KEY, std::string > MODEL_TEX_PATH
	{
		{TEX_KEY::TEST,				"Assets/Texture/Test.png"				},	// テスト画像
		{TEX_KEY::TEST2,			"Assets/Texture/Star.png"				},
		{TEX_KEY::CAMERA_ICON,		"Assets/Texture/Debug_Camera_Icon.png"	},
		{TEX_KEY::LIGHT_ICON,		"Assets/Texture/Debug_Light_Icon.png"	},
		{TEX_KEY::BLOCK_SIMPLE,		"Assets/Texture/Block_Simple.png"		},
		{TEX_KEY::GROUND_GRASS,		"Assets/Texture/Ground_Grass.png"		},
		{TEX_KEY::PLAYER_HP,		"Assets/Texture/Player_Hp.png"			},
		{TEX_KEY::PLAYER_BULLET,	"Assets/Texture/Player_Bullet.png"		},
		{TEX_KEY::STAGE_GOAL,		"Assets/Texture/Stage_Goal.png"			},
		{TEX_KEY::WALL_FENCE,		"Assets/Texture/Wall_Fence.png"			},
		{TEX_KEY::PLAYER_STARTPOS,	"Assets/Texture/Player_StartPos.png"	},
		{TEX_KEY::TITLE_LOGO,		"Assets/Texture/Title_Logo.png"			},
		{TEX_KEY::EFFECT_EXPLOSION, "Assets/Texture/Effect_Explosion.png"	},
		{TEX_KEY::EFFECT_SMOKE,		"Assets/Texture/Effect_Smoke.png"		},

	};

public:
	static TextureManager& GetInstance();
	void Init();

	// ゲッター
	Texture* GetTextureData(E_TEX_KEY e_GetKey);
	E_TEX_KEY GetTextureKey(Texture* pTex);

	std::unordered_map <E_TEX_KEY, std::shared_ptr<Texture>> GetTextureDatas();
	std::unordered_map <E_TEX_KEY, std::string> GetTexturePaths();
private:
	TextureManager();
private:
	std::unordered_map <E_TEX_KEY, std::shared_ptr<Texture>> m_apTextureDatas;	// モデルデータを格納しておく
	bool bIsInit;

};

