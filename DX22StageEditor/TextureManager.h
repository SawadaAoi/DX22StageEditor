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
		DEBUG_CAMERA_ICON,	// デバッグ用カメラアイコン
		DEBUG_LIGHT_ICON,	// デバッグ用ライトアイコン
		DEBUG_PLAYER_START,	// プレイヤースタート位置
		BLOCK_SIMPLE,		// シンプルなブロック
		BLOCK_GRASS,		// 草地
		BLOCK_GRASS_TOP,	// 草地(上)
		BLOCK_GRASS_SIDE,	// 草地(横)
		BLOCK_NOGRASS_SIDE,	// 草地なし(横)
		BLOCK_BRICK,		// レンガブロック
		BLOCK_BRICK_CRACK,	// レンガブロック(亀裂)
		PLAYER_BULLET,		// プレイヤー弾
		WALL_FENCE,			// 壁(フェンス)
		UI_TITLE_LOGO,		// タイトルロゴ
		UI_PLAYER_ICON,		// プレイヤーアイコン
		UI_HP_HEART,		// HPハート
		EFFECT_EXPLOSION,	// 爆発エフェクト
		EFFECT_SMOKE,		// 煙エフェクト
		ITEM_COIN,			// コイン
		BG_STAGE_SELECT,	// ステージセレクト背景
		FADE_IRIS_OUT,		// アイリスアウト
		EFFECT_FIREBALL,	// 火の玉エフェクト
		MAX,
	};

	// 画像データ保存先パス
	const std::unordered_map < TextureManager::E_TEX_KEY, std::string > MODEL_TEX_PATH
	{
		{TEX_KEY::TEST,					"Assets/Texture/Test.png"						},	
		{TEX_KEY::DEBUG_CAMERA_ICON,	"Assets/Texture/Debug/Debug_Camera_Icon.png"	},
		{TEX_KEY::DEBUG_LIGHT_ICON,		"Assets/Texture/Debug/Debug_Light_Icon.png"		},
		{TEX_KEY::DEBUG_PLAYER_START,	"Assets/Texture/Debug/Debug_PlayerStart.png"	},
		{TEX_KEY::BLOCK_SIMPLE,			"Assets/Texture/Block/Block_Simple.png"			},
		{TEX_KEY::BLOCK_GRASS,			"Assets/Texture/Block/Block_Grass.png"			},
		{TEX_KEY::BLOCK_GRASS_TOP,		"Assets/Texture/Block/Block_Grass_Top.png"		},
		{TEX_KEY::BLOCK_GRASS_SIDE,		"Assets/Texture/Block/Block_Grass_Side.png"		},
		{TEX_KEY::BLOCK_NOGRASS_SIDE,	"Assets/Texture/Block/Block_NoGrass_Side.png"	},
		{TEX_KEY::BLOCK_BRICK,			"Assets/Texture/Block/Block_Brick.png"			},
		{TEX_KEY::BLOCK_BRICK_CRACK,	"Assets/Texture/Block/Block_Brick_Crack.png"	},
		{TEX_KEY::PLAYER_BULLET,		"Assets/Texture/Player_Bullet.png"				},
		{TEX_KEY::WALL_FENCE,			"Assets/Texture/Wall_Fence.png"					},
		{TEX_KEY::UI_TITLE_LOGO,		"Assets/Texture/UI/UI_TitleLogo.png"			},
		{TEX_KEY::UI_PLAYER_ICON,		"Assets/Texture/UI/UI_PlayerIcon.png"			},
		{TEX_KEY::UI_HP_HEART,			"Assets/Texture/UI/UI_HpHeart.png"				},
		{TEX_KEY::EFFECT_EXPLOSION,		"Assets/Texture/Effect_Explosion.png"			},
		{TEX_KEY::EFFECT_SMOKE,			"Assets/Texture/Effect_Smoke.png"				},
		{TEX_KEY::ITEM_COIN,			"Assets/Texture/Item_Coin.png"					},
		{TEX_KEY::BG_STAGE_SELECT,		"Assets/Texture/UI/BG_StageSelect.png"			},
		{TEX_KEY::FADE_IRIS_OUT,		"Assets/Texture/UI/Fade_IrisOut.png"			},
		{TEX_KEY::EFFECT_FIREBALL,		"Assets/Texture/Effect_Fireball.png"			},

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

