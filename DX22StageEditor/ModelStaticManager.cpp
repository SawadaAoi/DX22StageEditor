/* ========================================
	DX22/Dropin
	------------------------------------
	モデルデータ管理用cpp
	------------------------------------
	ModelDataManager.cpp
========================================== */

// =============== インクルード ===================
#include "ModelStaticManager.h"


// =============== 定数定義 =======================
// モデルデータ保存先パス


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
========================================== */
ModelStaticManager::ModelStaticManager()
	: bIsInit(false)
{
	m_apModelDatas.clear();
}


/* ========================================
	インスタンス取得関数
	-------------------------------------
	内容：インスタンスを取得する
	-------------------------------------
	戻値：インスタンス
========================================== */
ModelStaticManager& ModelStaticManager::GetInstance()
{
	static ModelStaticManager instance;
	return instance;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：モデルデータを読み込む
========================================== */
void ModelStaticManager::Init()
{
	// 既に初期化されていたら終了
	if (bIsInit) return;

	// モデルデータを読み込む
	for (std::pair<E_MODEL_KEY, std::string> PathMap : MODEL_FBX_PATH)
	{
		E_MODEL_KEY eNowKey = PathMap.first;	// 現在のキー
		std::string sNowPath = PathMap.second;	// 現在の保存位置パス

		ModelStatic* pModel = new ModelStatic();
		if (!pModel->Load(sNowPath.c_str(), MODEL_FBX_SCALE.at(eNowKey), ModelStatic::E_Flip::XFlip))	// 3Dモデル読み込み
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//エラーメッセージの表示
		}

		m_apModelDatas.insert(std::make_pair(eNowKey, pModel));	// モデルデータ配列に追加
	}

	bIsInit = true;
}


/* ========================================
	モデルデータ取得関数
	-------------------------------------
	内容：キーを元に該当のモデルデータを渡す
	-------------------------------------
	引数1：E_MODEL_KEY	取得したいモデルデータのキー
	-------------------------------------
	戻値：ModelStatic*	モデルデータ
=========================================== */
ModelStatic* ModelStaticManager::GetModelData(E_MODEL_KEY e_GetKey)
{
	return m_apModelDatas.at(e_GetKey).get();
}

/* ========================================
	モデルデータキー取得関数
	-------------------------------------
	内容：モデルデータからキーを取得
	-------------------------------------
	引数1：ModelStatic*	pModelData	モデルデータ
	-------------------------------------
	戻値：E_MODEL_KEY	モデルデータのキー
=========================================== */
ModelStaticManager::E_MODEL_KEY ModelStaticManager::GetModelKey(ModelStatic* pModelData)
{
	for (std::pair<E_MODEL_KEY, std::shared_ptr<ModelStatic>> ModelData : m_apModelDatas)
	{
		if (ModelData.second.get() == pModelData)
		{
			return ModelData.first;
		}
	}

	return E_MODEL_KEY::MAX;
}

/* ========================================
	ゲッター(静的モデル配列)関数
	-------------------------------------
	戻値：std::unordered_map<E_MODEL_KEY, std::shared_ptr<ModelStatic>>	モデルデータ配列
=========================================== */
std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> ModelStaticManager::GetModelDatas()
{
	return m_apModelDatas;
}

/* ========================================
	ゲッター(モデルデータパス)関数
	-------------------------------------
	戻値：std::unordered_map<E_MODEL_KEY, std::string>	モデルデータパス
=========================================== */
std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> ModelStaticManager::GetModelPaths()
{
	return MODEL_FBX_PATH;
}
