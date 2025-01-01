/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(AABB)用ヘッダ
	------------------------------------
	説明：ワールドの軸に平行な衝突判定形状
		　※オブジェクトの向きが変わっても
			衝突判定立方体の向きは変わらない
	------------------------------------
	ComponentCollisionAABB.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentCollisionBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionSphere;

// =============== クラス定義 =====================
class ComponentCollisionAABB :
    public ComponentCollisionBase
{
public:
	ComponentCollisionAABB(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// 衝突判定
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionAABB(ComponentCollisionAABB* otherCol);		// AABBとAABB
	bool CheckCollisionOBB(ComponentCollisionOBB* otherCol);		// AABBとOBB
	bool CheckCollisionSphere(ComponentCollisionSphere* otherCol);	// AABBと球

	// ゲッター
	Vector3<float> GetMin();
	Vector3<float> GetMax();

	// セッター
	void SetMin(Vector3<float> vMin);
	void SetMax(Vector3<float> vMax);


	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	Vector3<float> m_vMin;		// 最小座標(左、下、手前)
	Vector3<float> m_vMax;		// 最大座標(右、上、奥)
};

