/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ攻撃コンポーネント(突進)用ヘッダ
	------------------------------------
	説明：敵キャラの突進攻撃処理クラス
	------------------------------------
	ComponentEnemyAttackCharge.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentEnemyAttackBase.h"
#include <memory>

// =============== 前方宣言 =======================
class ObjectBase;
class ShapeSphere;

// =============== クラス定義 ===================
class ComponentEnemyAttackCharge :
    public ComponentEnemyAttackBase
{
public:
	// 突進状態
	enum E_ChargeState
	{
		CS_IDLE,	// 待機
		CS_ATTACK,	// 突進
	};
public:
	ComponentEnemyAttackCharge(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	E_ChargeState GetChargeState();
	float GetChargeDist();
	float GetChargeSpeed();

	// セッター
	void SetChargeState(E_ChargeState eState);
	void SetChargeDist(float fDist);
	void SetChargeSpeed(float fSpeed);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Idle();
	void Attack();
private:
	ObjectBase*		m_pTarget;		// ターゲット
	E_ChargeState	m_eChargeState;	// 突進状態

	float m_fChargeDist;	// 突進距離
	float m_fChargeSpeed;	// 突進速度

	Vector3<float> m_vChargeEndPos;		// 突進終了位置

	bool m_bDispChargeEndSphere;	// 目的座標表示フラグ
	std::unique_ptr<ShapeSphere> m_pChargeEndSphere;	// 目的座標描画用

};

