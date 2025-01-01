/* ========================================
	DX22/Dropin
	------------------------------------
	モデルデータ管理用ヘッダ
	------------------------------------
	説明:モデルデータをまとめて読み込んで管理する
	------------------------------------
	ModelDataManager.h
========================================== */
#pragma once


// =============== インクルード ===================
#include "ModelStatic.h"
#include "Shader.h"
#include <vector>
#include <unordered_map>
#include <memory>

// =============== 定数定義 =======================
#define MODEL_STATIC_MNG_INST	ModelStaticManager::GetInstance()	// インスタンス取得

#define GET_MODEL_STATIC		ModelStaticManager::GetInstance().GetModelData	// モデル取得
#define MODEL_KEY				ModelStaticManager::E_MODEL_KEY					// モデルキー

class ModelStaticManager
{
public:
	// モデルデータ種類
	enum E_MODEL_KEY
	{
		TEST_MODEL,
		TEST_MODEL2,
		PLAYER_CAT,
		MAX,
	};

	const std::unordered_map < ModelStaticManager::E_MODEL_KEY, std::string > MODEL_FBX_PATH
	{
		{MODEL_KEY::TEST_MODEL, "Assets/Model/Test/TestObject.fbx"},
		{MODEL_KEY::TEST_MODEL2, "Assets/Model/Golem/Golem.fbx"},
		{MODEL_KEY::PLAYER_CAT, "Assets/Model/PlayerCat/Cat.fbx"},
	};

	// 各モデルデータ大きさ指定
	const std::unordered_map < ModelStaticManager::E_MODEL_KEY, float > MODEL_FBX_SCALE
	{
		{MODEL_KEY::TEST_MODEL, 1.0f},
		{MODEL_KEY::TEST_MODEL2, 0.01f},
		{MODEL_KEY::PLAYER_CAT, 0.6f},
	};

public:
	static ModelStaticManager& GetInstance();
	void Init();

	// ゲッター
	ModelStatic* GetModelData(E_MODEL_KEY e_GetKey);

	std::unordered_map <E_MODEL_KEY, std::shared_ptr<ModelStatic>> GetModelDatas();
	std::unordered_map <E_MODEL_KEY, std::string> GetModelPaths();

private:
	ModelStaticManager();
private:
	std::unordered_map <E_MODEL_KEY, std::shared_ptr<ModelStatic>> m_apModelDatas;	// モデルデータを格納しておく
	bool bIsInit;


};


