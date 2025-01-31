/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(爆発エフェクト)用ヘッダ
	------------------------------------
	説明：敵キャラが破壊された際の爆発エフェクト
	------------------------------------
	ObjectExplosion.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentBillboardAnime;

// =============== クラス定義 =====================
class ObjectExplosion :
    public ObjectBase
{
public:
    ObjectExplosion(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// ゲッター
	ComponentBillboardAnime* GetCompBillboardAnime();

	DEFINE_OBJECT_TYPE(ObjectExplosion)
private:
	ComponentBillboardAnime* m_pCompBillboardAnime;
};

