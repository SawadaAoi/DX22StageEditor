/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ攻撃コンポーネント(基底)用ヘッダ
	------------------------------------
	説明：敵キャラの攻撃処理の基底クラス
	------------------------------------
	ComponentEnemyAttackBase.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== クラス定義 ===================
class ComponentEnemyAttackBase :
    public ComponentBase
{
public:
	ComponentEnemyAttackBase(ObjectBase* pOwner);
	void Init() override;

	// ゲッター
	float GetAtkIntervalTime();

	// セッター
	void SetAtkIntervalTime(float fInterval);

	DEFINE_COMPONENT_TYPE
protected:
	ComponentTransform* m_pCompTransform;
	ComponentRigidbody* m_pCompRigidbody;

	float m_fAtkInvTime;	// 攻撃間隔
	float m_fAtkInvCnt;	// 攻撃間隔カウント
};

