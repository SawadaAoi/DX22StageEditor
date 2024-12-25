/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ攻撃コンポーネント(突進)用cpp
	------------------------------------
	ComponentEnemyAttackCharge.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentEnemyAttackCharge.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ShapeSphere.h"
#include "ColorVec3.h"
#include <format>

// =============== 定数 =======================
const float LIMIT_DIST_SQ			= 0.1f * 0.1f;	// 到達判定距離
const float DEFAULT_CHARGE_DIST		= 5.0f;			// 突進距離
const float DEFAULT_CHARGE_SPEED	= 10.0f;		// 突進速度

// 目的座標描画
const Vector3<float> TARGET_COLOR			= ColorVec3::BLUE;
const Vector3<float> TARGET_SPHERE_SCALE	= { 0.2f, 0.2f, 0.2f };

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有者オブジェクト
======================================== */
ComponentEnemyAttackCharge::ComponentEnemyAttackCharge(ObjectBase* pOwner)
	:ComponentEnemyAttackBase(pOwner)
	, m_fChargeDist(DEFAULT_CHARGE_DIST)
	, m_fChargeSpeed(DEFAULT_CHARGE_SPEED)
	, m_eChargeState(CS_IDLE)
	, m_pTarget(nullptr)
	, m_vChargeEndPos(Vector3<float>::Zero())
#ifdef _DEBUG
	, m_bDispChargeEndSphere(true)
#else
	, m_bDispChargeEndSphere(false)
#endif // DEBUG
	, m_pChargeEndSphere(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ComponentEnemyAttackCharge::Init()
{
	ComponentEnemyAttackBase::Init();

	m_pTarget = m_pOwnerObj->GetOwnerScene()->FindSceneObject("Player");

	// 突進終了座標用スフィア生成
	m_pChargeEndSphere = std::make_unique<ShapeSphere>();
	m_pChargeEndSphere->SetPosition(Vector3<float>::Zero());
	m_pChargeEndSphere->SetScale(TARGET_SPHERE_SCALE);
	m_pChargeEndSphere->SetColor(TARGET_COLOR);
	m_pChargeEndSphere->SetDrawMode(ShapeSphere::E_DrawMode::DRAW_NORMAL);

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ComponentEnemyAttackCharge::Update()
{
	if (!m_pTarget)
	{
		m_pTarget = m_pOwnerObj->GetOwnerScene()->FindSceneObject("Player");
		return;
	}

	switch (m_eChargeState)
	{
	case E_ChargeState::CS_IDLE:
		Idle();
		break;
	case E_ChargeState::CS_ATTACK:
		Attack();
		break;
	}
#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyAttackCharge"))
	{
		WIN_OBJ_INFO["EnemyAttackCharge"]["ChargeState"].SetText(
			(m_eChargeState == E_ChargeState::CS_IDLE) ? "Idle" : "Attack");
	}
#endif // _DEBUG

}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================= */
void ComponentEnemyAttackCharge::Draw()
{
	if (m_bDispChargeEndSphere)
	{
		m_pChargeEndSphere->SetPosition(m_vChargeEndPos);
		m_pChargeEndSphere->Draw();
	}
}

/* ========================================
	待機関数
	-------------------------------------
	内容：突進攻撃の待機処理
========================================= */
void ComponentEnemyAttackCharge::Idle()
{
	// プレイヤー座標を取得
	Vector3<float> vTargetPos = m_pTarget->GetTransform()->GetWorldPosition();

	// 向きを変更
	Vector3<float> vLookPos = vTargetPos;
	vLookPos.y				= m_pCompTransform->GetWorldPosition().y;	// 高さは変更しない
	m_pCompTransform->LookAt(vLookPos);

	m_fAtkInvCnt += DELTA_TIME;
	if (m_fAtkInvCnt >= m_fAtkInvTime)
	{
		m_fAtkInvCnt	= 0.0f;			// カウントリセット

		// 突進終了座標を設定
		Vector3<float> vDir = vTargetPos - m_pCompTransform->GetWorldPosition();
		vDir.y = 0.0f;	// 高さは変更しない
		m_vChargeEndPos		= m_pCompTransform->GetWorldPosition() + (vDir.GetNormalize() * m_fChargeDist);

		m_eChargeState	= CS_ATTACK;	// 攻撃状態に遷移
	}
}

/* ========================================
	突進関数
	-------------------------------------
	内容：突進攻撃処理
========================================= */
void ComponentEnemyAttackCharge::Attack()
{
	// 目的座標までのベクトルを求める
	Vector3<float> vDir = m_vChargeEndPos - m_pCompTransform->GetWorldPosition();

	// 移動
	m_pCompRigidbody->SetVelocity(vDir.GetNormalize() * m_fChargeSpeed);

	// 突進終了座標に到達したら
	if (vDir.LengthSq() <= LIMIT_DIST_SQ)
	{
		m_pCompRigidbody->SetVelocity(Vector3<float>::Zero());	// 移動停止
		m_eChargeState = CS_IDLE;	// 待機状態に遷移
		return;
	}

}

/* ========================================
	ゲッター(突進状態)関数
	-------------------------------------
	戻値：E_ChargeState		突進状態
========================================== */
ComponentEnemyAttackCharge::E_ChargeState ComponentEnemyAttackCharge::GetChargeState()
{
	return m_eChargeState;
}

/* ========================================
	ゲッター(突進距離)関数
	-------------------------------------
	戻値：float		突進距離
========================================== */
float ComponentEnemyAttackCharge::GetChargeDist()
{
	return m_fChargeDist;
}

/* ========================================
	ゲッター(突進速度)関数
	-------------------------------------
	戻値：float		突進速度
========================================== */
float ComponentEnemyAttackCharge::GetChargeSpeed()
{
	return m_fChargeSpeed;
}

/* ========================================
	セッター(突進状態)関数
	-------------------------------------
	引数：E_ChargeState		突進状態
========================================== */
void ComponentEnemyAttackCharge::SetChargeState(E_ChargeState eState)
{
	m_eChargeState = eState;
}

/* ========================================
	セッター(突進距離)関数
	-------------------------------------
	引数：float		突進距離
========================================== */
void ComponentEnemyAttackCharge::SetChargeDist(float fDist)
{
	m_fChargeDist = fDist;
}

/* ========================================
	セッター(突進速度)関数
	-------------------------------------
	引数：float		突進速度
========================================== */
void ComponentEnemyAttackCharge::SetChargeSpeed(float fSpeed)
{
	m_fChargeSpeed = fSpeed;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentEnemyAttackCharge::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* EnemyAttackCharge = Item::CreateGroup("EnemyAttackCharge");

	EnemyAttackCharge->AddGroupItem(Item::CreateBind("AtkInvTime", Item::Kind::Float, &m_fAtkInvTime));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("AtkInvCnt", Item::Kind::Float, &m_fAtkInvCnt));
	EnemyAttackCharge->AddGroupItem(Item::CreateValue("ChargeState", Item::Kind::Text));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeDist", Item::Kind::Float, &m_fChargeDist));
	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeSpeed", Item::Kind::Float, &m_fChargeSpeed));

	EnemyAttackCharge->AddGroupItem(Item::CreateBind("ChargeEndPos", Item::Kind::Vector, &m_vChargeEndPos));

	window.AddItem(EnemyAttackCharge);

	WIN_OBJ_INFO["EnemyAttackCharge"]["ChargeState"].SetText(
		(m_eChargeState == E_ChargeState::CS_IDLE) ? "Idle" : "Attack");

}

#endif // _DEBUG