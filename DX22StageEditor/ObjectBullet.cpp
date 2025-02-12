/* ========================================
	CatRobotGame/
	------------------------------------
	弾オブジェクト(基底)用cpp
	------------------------------------
	ObjectBullet.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectBullet.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionSphere.h"
#include "ComponentTransform.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
=========================================== */
ObjectBullet::ObjectBullet(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompRigidbody(nullptr)
	, m_pCompCollisionSphere(nullptr)
	, m_fDelTime(5.0f)
	, m_fMoveSpeed(10.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void ObjectBullet::InitLocal()
{
	m_pCompRigidbody = AddComponent<ComponentRigidbody>();
	m_pCompCollisionSphere = AddComponent<ComponentCollisionSphere>();

	m_pCompRigidbody->SetUseGravity(false);		// 重力無効
	m_pCompCollisionSphere->SetTrigger(true);	// 衝突はすり抜け

	Destroy(m_fDelTime);	// 一定時間後に削除
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectBullet::UpdateLocal()
{
	// 向いている方向に移動
	m_pCompRigidbody->SetVelocity(m_pCompTransform->GetForwardVector() * m_fMoveSpeed);
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数1：衝突相手オブジェクト
=========================================== */
void ObjectBullet::OnCollisionEnter(ObjectBase* pHit)
{
	Destroy();
}

/* ========================================
	ゲッター(削除時間)関数
	-------------------------------------
	戻値：float 削除時間
=========================================== */
float ObjectBullet::GetDeleteTime()
{
	return m_fDelTime;
}

/* ========================================
	ゲッター(移動速度)関数
	-------------------------------------
	戻値：float 移動速度
=========================================== */
float ObjectBullet::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	セッター(削除時間)関数
	-------------------------------------
	引数1：float 削除時間
=========================================== */
void ObjectBullet::SetDeleteTime(float fLifeTime)
{
	m_fDelTime = fLifeTime;
	Destroy(m_fDelTime);	// 削除時間を再設定する(初期化時に設定されている値を上書き)
}

/* ========================================
	セッター(移動速度)関数
	-------------------------------------
	引数1：float 移動速度
=========================================== */
void ObjectBullet::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}
