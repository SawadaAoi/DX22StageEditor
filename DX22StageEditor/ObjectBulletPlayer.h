/* ========================================
	CatRobotGame/
	------------------------------------
	弾オブジェクト(基底)用ヘッダ
	------------------------------------
	説明：弾オブジェクトの基底クラス
		　向いている方向に移動する
	------------------------------------
	ObjectBullet.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBullet.h"


// =============== クラス定義 =====================
class ObjectBulletPlayer :
	public ObjectBullet
{
public:
	ObjectBulletPlayer(SceneBase* pScene);

	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectBulletPlayer)
};

