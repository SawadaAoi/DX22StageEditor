/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(基底)用ヘッダ
	------------------------------------
	説明：敵キャラの移動処理の基底クラス
	------------------------------------
	ComponentEnemyMoveBase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== クラス定義 =====================
class ComponentEnemyMoveBase :
    public ComponentBase
{
public:
	ComponentEnemyMoveBase(ObjectBase* pOwner);
	void Init() override;

	// ゲッター
	float GetMoveSpeed();

	// セッター
	void SetMoveSpeed(float fSpeed);

	DEFINE_COMPONENT_TYPE
protected:
	ComponentTransform* m_pCompTransform;
	ComponentRigidbody* m_pCompRigidbody;

	float			m_fMoveSpeed;
};

