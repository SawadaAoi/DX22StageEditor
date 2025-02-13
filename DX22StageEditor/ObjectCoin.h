/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(コイン)用ヘッダ
	------------------------------------
	説明：プレイヤーが取得するコイン
		　重力有り、ビルボードアニメーション有り
		　すり抜け衝突判定
	------------------------------------
	ObjectCoin.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentBillboardAnime;
class ComponentRigidbody;
class ComponentCollisionSphere;
class ComponentGroundRaycast;

// =============== クラス ===================
class ObjectCoin :
    public ObjectBase
{
public:
	ObjectCoin(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectCoin)
private:
	ComponentBillboardAnime*	m_pCompBillboardAnime;		// ビルボードアニメーション
	ComponentRigidbody*			m_pCompRigidbody;			// 剛体
	ComponentCollisionSphere*	m_pCompCollisionSphere;		// 衝突判定
	ComponentGroundRaycast*		m_pCompGroundRaycast;		// 地面判定
};

