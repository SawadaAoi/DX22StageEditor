/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ゴール)用ヘッダ
	------------------------------------
	説明：ステージのゴール
		　プレイヤーが接触するとクリア
	------------------------------------
	ObjectGoal.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== クラス定義 =====================
class ObjectGoal :
    public ObjectBase
{
public:
	ObjectGoal(SceneBase* pScene);

	void InitLocal();
	void UpdateLocal();

	void OnCollisionEnter(ObjectBase* pHit) override;

	// ゲッター
	bool GetIsGoal();

	// セッター
	void SetIsGoal(bool bIsGoal);

	DEFINE_OBJECT_TYPE(ObjectGoal)
private:
	bool m_bIsGoal;		// true: ゴール / false: 未ゴール
};

