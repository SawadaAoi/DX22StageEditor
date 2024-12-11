/* ========================================
	DX22Base/
	------------------------------------
	ライト管理用ヘッダ
	------------------------------------
	説明:ライトを管理するクラス(適宜切り替える)
	------------------------------------
	LightManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectLight.h"	// ライトオブジェクト
#include <array>	// std::array

// =============== 前方宣言 =======================
class SceneBase;
class ObjectBase;

// =============== 定数定義 =====================
#define LIGHT_MNG_INST LightManager::GetInstance()	// instance取得
#define GET_LIGHT LightManager::GetInstance().GetLight()	// ライト取得

const int MAX_LIGHT_NUM = 10;	// ライト最大数

class LightManager
{
public:
	static LightManager& GetInstance();	// インスタンス取得
	void Init(SceneBase* pScene);		// シーン開始時にライト初期化

	void AddLight(ObjectLight* pLight);		// ライト追加
	void RemoveLight(ObjectLight* pLight);	// ライト削除

	// ゲッター
	ObjectLight* GetLight(int num);
	std::vector<ObjectLight*> GetLightList();
	int GetLightNum();

private:
	LightManager();	// コンストラクタ	

	std::vector<ObjectLight*> m_pLightList;	// ライトリスト

	SceneBase* m_pScene;	// シーンクラスのポインタ
};

