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
#include "ObjectBase.h"

// =============== 前方宣言 =====================
class ComponentRigidbody;
class ComponentCollisionSphere;

// =============== クラス定義 =====================
class ObjectBullet :
    public ObjectBase
{
public:
	ObjectBullet(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	// ゲッター
	float GetDeleteTime();
	float GetMoveSpeed();

	// セッター
	void SetDeleteTime(float fLifeTime);
	void SetMoveSpeed(float fMoveSpeed);

	DEFINE_OBJECT_TYPE(ObjectBullet)
protected:
	ComponentRigidbody* m_pCompRigidbody;
	ComponentCollisionSphere* m_pCompCollisionSphere;

	float m_fDelTime;		// 最大生存時間
	float m_fMoveSpeed;		// 移動速度

	float m_fDelTimeCnt;	// 経過時間
};

