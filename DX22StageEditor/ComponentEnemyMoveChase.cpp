/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(追跡)用cpp
	------------------------------------
	ComponentEnemyMoveChase.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentEnemyMoveChase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ShapeLine.h"
#include "ColorVec3.h"

// =============== 定数定義 =======================
const float DEFAULT_CHASE_START_DIST	= 10.0f;		// 追跡開始距離
const float LIMIT_DIST_SQ_TO_PLYR		= 1.0f * 1.0f;	// 移動先に到達する距離の2乗
const float LIMIT_DIST_SQ_TO_START		= 0.5f * 0.5f;	// 移動先に到達する距離の2乗

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================= */
ComponentEnemyMoveChase::ComponentEnemyMoveChase(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_pTargetObj(nullptr)
	, m_fLimitDistSq(LIMIT_DIST_SQ_TO_PLYR)
	, m_fChaseStartDist(DEFAULT_CHASE_START_DIST)
#ifdef _DEBUG
	, m_bDispDistLine(true)
#else
	, m_bDispDistLine(false)
#endif // _DEBUG
	, m_pDistLine(nullptr)
	, m_vStartPos(Vector3<float>::Zero())
	, m_qStartRot()
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ComponentEnemyMoveChase::Init()
{
	ComponentEnemyMoveBase::Init();

	m_pTargetObj = m_pOwnerObj->GetOwnerScene()->GetSceneObjectTag(E_ObjectTag::Player);
	m_pDistLine = std::make_unique<ShapeLine>(1);

	m_vStartPos = m_pCompTransform->GetPosition();
	m_qStartRot = m_pCompTransform->GetRotation();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ComponentEnemyMoveChase::Update()
{
#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveChase"))
	{
		WIN_OBJ_INFO["EnemyMoveChase"]["TargetObj"].SetText(m_pTargetObj ? m_pTargetObj->GetName() : "None");
		float fDist = m_pTargetObj ? 
			(m_pTargetObj->GetTransform()->GetPosition() - m_pCompTransform->GetPosition()).Length() : 0.0f;
		WIN_OBJ_INFO["EnemyMoveChase"]["DistanceToPlayer"].SetText(std::to_string(fDist));
	}
#endif // _DEBUG

	// プレイヤーがいない場合は何もしない
	if (!m_pTargetObj)
	{
		m_pTargetObj = m_pOwnerObj->GetOwnerScene()->GetSceneObjectTag(E_ObjectTag::Player);
		return;
	}

	// 移動処理
	Move();

	// 距離ライン描画
	if (m_bDispDistLine)
		DrawPlayerDistLine();
	
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================= */
void ComponentEnemyMoveChase::Draw()
{
	if (m_bDispDistLine)
	{
		m_pDistLine->Draw();
	}
}


/* ========================================
	移動関数
	-------------------------------------
	内容：移動処理
========================================= */
void ComponentEnemyMoveChase::Move()
{
	// 追跡対象との距離
	float fDistSq = m_pTargetObj->GetTransform()->GetPosition().DistanceSq(m_pCompTransform->GetPosition());

	
	// 追跡開始距離より近づいたら追跡開始
	if(fDistSq < (m_fChaseStartDist * m_fChaseStartDist))
	{
		ChaseTarget();
	}
	// 追跡開始距離より遠い場合は初期座標に向かって移動
	else
	{
		BackToStartPos();
	}

}

/* ========================================
	対象オブジェクト追跡移動関数
	-------------------------------------
	内容：対象オブジェクトを追跡する
========================================= */
void ComponentEnemyMoveChase::ChaseTarget()
{
	// プレイヤーの座標を取得
	Vector3<float> vTargetPos = m_pTargetObj->GetTransform()->GetPosition();

	// プレイヤーの方向
	Vector3<float> vDirTarget = vTargetPos - m_pCompTransform->GetPosition();
	vDirTarget.y = 0.0f;

	// 追跡対象との距離が追跡限界距離より遠い場合は追跡
	if (vDirTarget.LengthSq() > m_fLimitDistSq)
	{
		// 追跡対象の方向へ速度を設定(Y軸は考慮しない)
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = vDirTarget.GetNormalize().x * m_fMoveSpeed;
		vVelocity.z = vDirTarget.GetNormalize().z * m_fMoveSpeed;

		m_pCompRigidbody->SetVelocity(vVelocity);

	}
	else
	{
		// 移動停止
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = 0.0f;
		vVelocity.z = 0.0f;
		m_pCompRigidbody->SetVelocity(vVelocity);
	}

	// 追跡対象の方向を向く
	Vector3<float> vLook = vTargetPos;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	移動開始座標帰還関数
	-------------------------------------
	内容：移動開始座標に戻る
========================================= */
void ComponentEnemyMoveChase::BackToStartPos()
{
	// 移動開始座標の方向
	Vector3<float> vDirStart = m_vStartPos - m_pCompTransform->GetPosition();
	vDirStart.y = 0.0f;

	//
	if (vDirStart.LengthSq() > LIMIT_DIST_SQ_TO_START)
	{
		// 移動開始座標方向へ速度を設定(Y軸は考慮しない)
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = vDirStart.GetNormalize().x * m_fMoveSpeed;
		vVelocity.z = vDirStart.GetNormalize().z * m_fMoveSpeed;

		m_pCompRigidbody->SetVelocity(vVelocity);

		// 移動開始座標の方向を向く
		Vector3<float> vLook = m_vStartPos;
		vLook.y = m_pCompTransform->GetPosition().y;
		m_pCompTransform->LookAt(vLook);
	}
	else
	{
		// 移動停止
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = 0.0f;
		vVelocity.z = 0.0f;
		m_pCompRigidbody->SetVelocity(vVelocity);

		// 初期方向を向く
		m_pCompTransform->SetLocalRotation(m_qStartRot);
	}
}

/* ========================================
	距離ライン更新関数
	-------------------------------------
	内容：プレイヤーとの距離ラインを描画
========================================= */
void ComponentEnemyMoveChase::DrawPlayerDistLine()
{
	// プレイヤーの座標を取得
	Vector3<float> vTargetPos = m_pTargetObj->GetComponent<ComponentTransform>()->GetPosition();

	// 距離ライン描画(ターゲットの方向へ、追跡開始距離分の距離)
	Vector3<float> vStart = m_pCompTransform->GetPosition();				// 自分の位置
	Vector3<float> vDir = vTargetPos - m_pCompTransform->GetPosition();	// プレイヤーの方向
	vDir.Normalize();															// 正規化
	Vector3<float> vEnd = vStart + (vDir * m_fChaseStartDist);					// 追跡開始距離分の位置

	// 高さは0.5fに固定
	vStart.y = 0.5f;
	vEnd.y	 = 0.5f;

	m_pDistLine->UpdateLine(1, vStart, vEnd, ColorVec3::BLUE);
}

/* ========================================
	ゲッター(追跡開始距離)関数
	-------------------------------------
	戻り値：float	追跡開始距離
=========================================== */
float ComponentEnemyMoveChase::GetChaseStartDist() const
{
	return m_fChaseStartDist;
}

/* ========================================
	ゲッター(距離ライン表示フラグ)関数
	-------------------------------------
	戻り値：bool	距離ライン表示フラグ
=========================================== */
bool ComponentEnemyMoveChase::GetDispDistLine() const
{
	return m_bDispDistLine;
}

/* ========================================
	ゲッター(追跡限界距離)関数
	-------------------------------------
	戻り値：float	追跡限界距離
=========================================== */
float ComponentEnemyMoveChase::GetLimitDistSq() const
{
	return m_fLimitDistSq;
}


/* ========================================
	セッター(追跡開始距離)関数
	-------------------------------------
	引数：float	追跡開始距離
=========================================== */
void ComponentEnemyMoveChase::SetChaseStartDist(float fDist)
{
	m_fChaseStartDist = fDist;
}

/* ========================================
	セッター(距離ライン表示フラグ)関数
	-------------------------------------
	引数：bool	距離ライン表示フラグ
=========================================== */
void ComponentEnemyMoveChase::SetDispDistLine(bool bDisp)
{
	m_bDispDistLine = bDisp;
}

/* ========================================
	セッター(追跡限界距離)関数
	-------------------------------------
	引数：float	追跡限界距離
=========================================== */
void ComponentEnemyMoveChase::SetLimitDistSq(float fDist)
{
	m_fLimitDistSq = fDist;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveChase::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pEnemyMoveChaser = Item::CreateGroup("EnemyMoveChase");

	pEnemyMoveChaser->AddGroupItem(Item::CreateBind("MoveSpeed",			Item::Kind::Float, &m_fMoveSpeed));
	pEnemyMoveChaser->AddGroupItem(Item::CreateValue("TargetObj",			Item::Kind::Text));
	pEnemyMoveChaser->AddGroupItem(Item::CreateValue("DistanceToPlayer",	Item::Kind::Text));
	pEnemyMoveChaser->AddGroupItem(Item::CreateBind("ChaseStartDist",		Item::Kind::Float, &m_fChaseStartDist));

	window.AddItem(pEnemyMoveChaser);
}

#endif // _DEBUG