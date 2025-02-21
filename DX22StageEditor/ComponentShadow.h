/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(丸影)用ヘッダ
	------------------------------------
	説明：オブジェクト(プレイヤーや敵)の
		　下に表示する丸影
	------------------------------------
	ComponentShadow.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"


// =============== クラス定義 =====================
class ComponentShadow :
    public ComponentBase
{
public:
	ComponentShadow(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	// ゲッター
	Vector3<float> GetPosition() const;
	float GetCircleSize() const;

	// セッター
	void SetCircleSize(float range);
	void SetPosOffset(Vector3<float> vOffset);

	DEFINE_COMPONENT_TYPE
private:
	float	m_fCircleSize;			// 丸影の大きさ
	Vector3<float> m_vPosOffset;	// 影の基準座標の調整値
};

