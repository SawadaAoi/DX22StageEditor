/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(落ちる床)用ヘッダ
	------------------------------------
	説明：プレイヤーが乗ると一定時間後に落下する床オブジェクト
	------------------------------------
	ObjectDropFloor.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBlock.h"

// =============== 前方宣言 =======================
class ComponentRigidbody;

// =============== クラス定義 =====================
class ObjectDropFloor :
    public ObjectBlock
{
public:
	ObjectDropFloor(SceneBase* pScene);	
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectDropFloor)
private:
	ComponentRigidbody* m_pCompRigidBody;	// 剛体コンポーネント
	bool	m_bIsDropSwitch;				// 落下スイッチ
	float	m_fDropWaitTime;				// 落下開始時間
	bool	m_bIsRespawn;					// リスポーンフラグ
	float	m_fRespawnTime;					// リスポーン時間

	Vector3<float> m_vDefaultPos;			// 初期位置

};

