/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ線移動)用ヘッダ
	------------------------------------
	説明：線移動する敵キャラクター
	------------------------------------
	ObjectEnemyLinear.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectEnemy.h"

// =============== 前方宣言 =======================
class ComponentEnemyMoveLinear;

// =============== クラス定義 =====================
class ObjectEnemyLinear :
    public ObjectEnemy
{
public:
	// セーブデータ
	struct S_SaveDataEnemyLinear
	{
		int nWayPointNum;	// 移動座標数
		bool bIsReverse;	// 逆順フラグ
	};

public:
	ObjectEnemyLinear(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyLinear)	// オブジェクトの種類ID取得関数
private:
	ComponentEnemyMoveLinear* m_pCompEnemyMoveLinear;

};

