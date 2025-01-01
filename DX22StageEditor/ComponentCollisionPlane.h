#pragma once
#include "ComponentCollisionBase.h"
class ComponentCollisionPlane :
    public ComponentCollisionBase
{
public:
	ComponentCollisionPlane(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// 衝突判定
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionAABB(ComponentCollisionAABB* otherCol);		// AABBとAABB

	// ゲッター
	Vector3<float> GetMin();
	Vector3<float> GetMax();

	// セッター
	void SetMin(Vector3<float> vMin);
	void SetMax(Vector3<float> vMax);

private:
	Vector3<float> m_vMin;		// 最小座標(左、下、手前)
	Vector3<float> m_vMax;		// 最大座標(右、上、奥)
};

