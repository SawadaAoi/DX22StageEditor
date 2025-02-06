/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(移動時煙エフェクト)用ヘッダ
	------------------------------------
	説明：移動時の煙エフェクトを管理する
	------------------------------------
	ComponentSmokeEffect.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"

// =============== 前方宣言 =======================
class SceneBase;

// =============== クラス定義 =====================
class ComponentSmokeEffect :
    public ComponentBase
{
public:
	ComponentSmokeEffect(ObjectBase* pOwner);
	void Init() override;
	void Update() override;

	// セッター
	void SetMoveDir(Vector3<float> vMoveDir);
	void SetCreatePosDist(Vector3<float> vCreatePosDist);
	void SetCreateInterval(float fCreateInterval);
	void SetLifeTime(float fLifeTime);
	void SetScale(float fScaleMin, float fScaleMax);
	void SetPosAdjust(float fPosAdjustMin, float fPosAdjustMax);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void CreateSmoke();
	void DeleteSmoke();
private:
	SceneBase* m_pScene;

	std::vector<std::pair<ObjectBase*, float>> m_Smokes;
	Vector3<float> m_vMoveDir;	// プレイヤーの移動方向

	float m_fCreateInterval;	// 生成間隔
	float m_fCreateItvTimeCnt;	// 生成間隔時間カウント

	Vector3<float> m_vCreatePosDist;	// 生成位置の調整
	float m_fLifeTime;					// 煙の寿命

	float m_fScaleMin;	// 煙の最小スケール
	float m_fScaleMax;	// 煙の最大スケール

	float m_fPosAdjustMin;	// 位置調整の最小値
	float m_fPosAdjustMax;	// 位置調整の最大値

};

