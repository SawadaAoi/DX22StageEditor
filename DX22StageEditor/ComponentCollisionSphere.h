/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(球体)用ヘッダ
	------------------------------------
	説明：球体の衝突判定を行うコンポーネント
	------------------------------------
	ComponentCollisionSphere.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentCollisionBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionAABB;

// =============== クラス定義 =====================
class ComponentCollisionSphere :
    public ComponentCollisionBase
{
public:
	ComponentCollisionSphere(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// 衝突判定
	bool CheckCollision(ComponentCollisionBase* otherCol) override;

	// ゲッター
	float GetRadius();

	// セッター
	void SetRadius(float fRadius);

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	bool CheckCollisionSphere(ComponentCollisionSphere* otherCol);	// 球同士の衝突判定
private:
	float m_fRadius;			// 半径
};

