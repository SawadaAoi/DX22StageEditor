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

// =============== 前方宣言 =======================
class ComponentModelStatic;

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
	ComponentModelStatic* m_pModel;	// モデルコンポーネント

	bool m_bIsGoal;				// true: ゴール / false: 未ゴール
	bool m_bIsSetBasePos;		// 基準座標設定フラグ
	float m_fAnimeTimeCnt;		// アニメーション時間カウント
	Vector3<float> m_vBasePos;	// 基準座標
};

