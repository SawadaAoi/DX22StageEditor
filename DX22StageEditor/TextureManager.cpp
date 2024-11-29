/* ========================================
	DX22/Dropin
	------------------------------------
	画像データ管理用cpp
	------------------------------------
	TextureDataManager.cpp
========================================== */


// =============== インクルード ===================
#include "TextureManager.h"



/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
TextureManager::TextureManager()
	: bIsInit(false)
{
	m_apTextureDatas.clear();
	
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：画像データを読み込む
=========================================== */
void TextureManager::Init()
{
	// 既に初期化されていたら終了
	if (bIsInit) return;

	// 画像データを読み込む
	for (std::pair<E_TEX_KEY, std::string> PathMap : MODEL_TEX_PATH)
	{
		E_TEX_KEY eNowKey = PathMap.first;		// 現在のキー
		std::string sNowPath = PathMap.second;	// 現在の保存位置パス

		Texture* pTex = new Texture;				// 画像データ作成
		if (FAILED(pTex->Create(sNowPath.c_str())))	// 画像データ読み込み
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//エラーメッセージの表示
		}

		m_apTextureDatas.insert(std::make_pair(eNowKey, pTex));	// 画像データ配列に追加
	}

	bIsInit = true;
}



/* ========================================
	インスタンス(シングルトン)取得関数
	-------------------------------------
	内容：自クラスのインスタンスを取得
	-------------------------------------
	戻値：自クラスのインスタンス
=========================================== */
TextureManager& TextureManager::GetInstance()
{
	static TextureManager instance;
	return instance;
}

/* ========================================
	画像データ取得関数
	-------------------------------------
	内容：指定された画像データを取得
	-------------------------------------
	引数1：E_TEX_KEY e_GetKey
	-------------------------------------
	戻値：Texture* 画像データ
=========================================== */
Texture* TextureManager::GetTextureData(E_TEX_KEY e_GetKey)
{
	return m_apTextureDatas[e_GetKey].get();
}



/* ========================================
	ゲッター(画像データ配列)関数
	-------------------------------------
	戻値：unordered_map<E_TEX_KEY, shared_ptr<Texture>> 画像データ配列
=========================================== */
std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> TextureManager::GetTextureDatas()
{
	return m_apTextureDatas;
}

/* ========================================
	ゲッター(画像データパス)関数
	-------------------------------------
	戻値：unordered_map<E_TEX_KEY, string> 画像データパス
=========================================== */
std::unordered_map<TextureManager::E_TEX_KEY, std::string> TextureManager::GetTexturePaths()
{
	return MODEL_TEX_PATH;
}
