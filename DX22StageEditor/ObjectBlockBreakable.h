/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト(破壊可能ブロック)用ヘッダ
	------------------------------------
	説明：弾に当たると壊れるブロック
		　AABB、地面判定有り
	------------------------------------
	ObjectBlockBreakable.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBlock.h"

// =============== クラス定義 ===================
class ObjectBlockBreakable :
    public ObjectBlock
{
public:
	ObjectBlockBreakable(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectBlockBreakable)
private:
	int m_nHp;		// HP
	int m_nHpOld;	// HP(前回)

	bool m_bIsBreak;	// 壊れたかどうか
};

