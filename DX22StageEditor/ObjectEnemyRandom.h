/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラランダム移動)用ヘッダ
	------------------------------------
	説明：ランダム移動する敵キャラクター
	------------------------------------
	ObjectEnemyRandom.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectEnemy.h"

// =============== 前方宣言 =======================
class ComponentEnemyMoveRandom;

// =============== クラス定義 =====================
class ObjectEnemyRandom :
    public ObjectEnemy
{
public:
	// セーブデータ
	struct S_SaveDataEnemyRandom
	{
		float fMoveSpeed;				// 移動速度
		float fTargetSwitchTime;		// ターゲット切り替え時間
		Vector3<float> vMoveLimitCenter;	// 移動範囲中心座標
		Vector3<float> vMoveLimitRF;	// 移動範囲(右上)
		Vector3<float> vMoveLimitLF;	// 移動範囲(左上)
		Vector3<float> vMoveLimitLB;	// 移動範囲(左下)
		Vector3<float> vMoveLimitRB;	// 移動範囲(右下)
	};
public:
	ObjectEnemyRandom(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectEnemyRandom)	// オブジェクトの種類ID取得関数
private:
	ComponentEnemyMoveRandom* m_pCompEnemyMoveRandom;
};

