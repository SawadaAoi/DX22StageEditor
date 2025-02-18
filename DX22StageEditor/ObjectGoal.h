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
	// セーブデータ
	struct S_SaveData
	{
		Vector3<float> vBasePos;	// 座標
	};
public:
	ObjectGoal(SceneBase* pScene);

	void InitLocal();
	void UpdateLocal();

	void OnCollisionEnter(ObjectBase* pHit) override;

	// ゲッター
	bool GetIsGoal();

	// セッター
	void SetIsGoal(bool bIsGoal);	// データ保存

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectGoal)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	ComponentModelStatic* m_pModel;	// モデルコンポーネント

	bool			m_bIsGoal;			// true: ゴール / false: 未ゴール
	float			m_fAnimeTimeCnt;	// アニメーション時間カウント
	Vector3<float>	m_vBasePos;			// 基準座標

	bool m_bOnFloatAnime;				// 上下アニメーションフラグ
};

