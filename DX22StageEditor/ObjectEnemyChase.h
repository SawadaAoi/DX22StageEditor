/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ追跡移動)用ヘッダ
	------------------------------------
	説明：追跡移動する敵キャラクター
	------------------------------------
	ObjectEnemyChase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectEnemy.h"

// =============== 前方宣言 =======================
class ComponentEnemyMoveChase;

// =============== クラス定義 =====================
class ObjectEnemyChase :
    public ObjectEnemy
{
public:
	// セーブデータ
	struct S_SaveDataEnemyChase
	{
		float fMoveSpeed;		// 移動速度
		float fChaseStartDist;	// 追跡開始距離
		float fLimitDistSq;		// 追跡限界距離
	};
public:
	ObjectEnemyChase(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyChase)	// オブジェクトの種類ID取得関数
private:
	ComponentEnemyMoveChase* m_pCompEnemyMoveChase;
};

