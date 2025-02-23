/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(落ちる床)用cpp
	------------------------------------
	ObjectDropFloor.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectDropFloor.h"
#include "ComponentRigidbody.h"
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"

// =============== 定数定義 =======================
const Vector3<float>	DEFAULT_SCALE	= { 3.0f, 0.25f, 3.0f };	// デフォルトスケール
const float				DROP_WAIT_TIME	= 0.5f;						// 落下待ち時間
const float				RESPAWN_TIME	= 5.0f;						// リスポーン時間

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================= */
ObjectDropFloor::ObjectDropFloor(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_bIsDropSwitch(false)
	, m_fDropWaitTime(0.0f)
	, m_bIsRespawn(false)
	, m_fRespawnTime(0.0f)
	, m_pCompRigidBody(nullptr)
	, m_vDefaultPos(Vector3<float>(0.0f, 0.0f, 0.0f))
{
	SetTag(E_ObjectTag::Ground);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================= */
void ObjectDropFloor::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// 地面判定
	m_eColType = E_COL_TYPE::COL_AABB;	// 衝突判定形状

	ObjectBlock::InitLocal();
	m_pCompRigidBody = AddComponent<ComponentRigidbody>();
	m_pCompRigidBody->SetUseGravity(false);

	m_pCompTransform->SetScale(DEFAULT_SCALE);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理を行う
========================================= */
void ObjectDropFloor::UpdateLocal()
{
	// 落下待機処理
	if (m_bIsDropSwitch)
	{
		m_fDropWaitTime += DELTA_TIME;
		if (m_fDropWaitTime >= DROP_WAIT_TIME)
		{
			m_pCompRigidBody->SetUseGravity(true);
			m_bIsRespawn = true;
		}
	}

	// リスポーン処理
	if (m_bIsRespawn)
	{
		m_fRespawnTime += DELTA_TIME;
		if (m_fRespawnTime >= RESPAWN_TIME)
		{
			// 重力を無効化
			m_pCompRigidBody->SetUseGravity(false);
			m_pCompRigidBody->SetVelocity(Vector3<float>(0.0f, 0.0f, 0.0f));
			m_pCompRigidBody->SetAcceleration(Vector3<float>(0.0f, 0.0f, 0.0f));
			// 各フラグをリセット
			m_bIsDropSwitch		= false;
			m_bIsRespawn		= false;
			// 待ち時間をリセット
			m_fDropWaitTime		= 0.0f;
			m_fRespawnTime		= 0.0f;
			// 初期位置に戻す
			m_pCompTransform->SetPosition(m_vDefaultPos);
		}

	}

}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectDropFloor::OnCollisionEnter(ObjectBase* pHit)
{
	if (pHit->CheckTag(E_ObjectTag::Player))
	{
		m_bIsDropSwitch = true;
		m_vDefaultPos = m_pCompTransform->GetPosition();	// 初期位置を保存
	}
}
