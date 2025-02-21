/* ========================================
	CatRobotGame/
	------------------------------------
	丸影管理用ヘッダ
	------------------------------------
	説明:オブジェクトの下に表示する丸影を管理する
	------------------------------------
	ShadowManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentShadow.h"
#include <array>	// std::array
#include <vector>	// std::vector

// =============== 前方宣言 =======================
class SceneBase;
class ObjectBase;

// =============== 定数定義 =====================
#define SHADOW_MNG_INST ShadowManager::GetInstance()	// instance取得
#define GET_SHADOW ShadowManager::GetInstance().GetLight()	// ライト取得


// =============== クラス定義 =====================
class ShadowManager
{
public:
	static ShadowManager& GetInstance();	// インスタンス取得
	void Init(SceneBase* pScene);		// シーン開始時にライト初期化

	void AddShadow(ComponentShadow* pShadow);		// ライト追加
	void RemoveShadow(ComponentShadow* pShadow);	// ライト削除

	// ゲッター
	ComponentShadow* GetShadow(int num);
	std::vector<ComponentShadow*> GetShadowList();
	int GetShadowNum();

private:
	ShadowManager();	// コンストラクタ	
private:
	std::vector<ComponentShadow*> m_pShadowList;	// 影リスト
	SceneBase* m_pScene;							// シーンクラスのポインタ
};

