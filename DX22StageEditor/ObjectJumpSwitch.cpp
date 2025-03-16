/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ジャンプバネスイッチ)用cpp
	------------------------------------
	ObjectJumpSwitch.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectJumpSwitch.h"
#include "ObjectJumpPad.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionAABB.h"
#include "ComponentPlayerController.h"

// =============== 定数定義 =======================
const float POWER_RATE = 1.0f;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================= */
ObjectJumpSwitch::ObjectJumpSwitch(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pJumpPad(nullptr)
	, m_pCompColAABB(nullptr)
	, m_fObjJumpPower(0.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================= */
void ObjectJumpSwitch::InitLocal()
{
	m_pCompColAABB = AddComponent<ComponentCollisionAABB>();
	m_pCompColAABB->SetTrigger(true);

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理を行う
========================================= */
void ObjectJumpSwitch::UpdateLocal()
{
	// ジャンプパッドの取得
	if (!m_pJumpPad)
		m_pJumpPad = static_cast<ObjectJumpPad*>(m_pParentObj->GetParentObject());

	// ジャンプパッドの上に配置
	float fOffset = m_pParentObj->GetTransform()->GetScale().y * 0.5f;
	Vector3<float> vPos = m_pParentObj->GetTransform()->GetPosition();
	vPos.y += fOffset;

	GetTransform()->SetPosition(vPos);
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectJumpSwitch::OnCollisionEnter(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	m_fObjJumpPower = pHit->GetComponent<ComponentPlayerController>()->GetJumpPower();
}

/* ========================================
	衝突判定(継続時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(継続時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectJumpSwitch::OnCollisionStay(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	// ジャンプ力の変更(ジャンプバネの伸縮率に応じて変化)
	float fJumpBoost = m_fObjJumpPower * (POWER_RATE * (1.0f - m_pJumpPad->GetStretchRate()));
	pHit->GetComponent<ComponentPlayerController>()->SetJumpPower(m_fObjJumpPower + fJumpBoost);
	// ジャンプパッドオブジェクトの状態を縮むに変更
	m_pJumpPad->SetSpringState(ObjectJumpPad::E_SpringState::Shrink);
}

/* ========================================
	衝突判定(終了時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(終了時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectJumpSwitch::OnCollisionExit(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	// ジャンプ力を元に戻す
	pHit->GetComponent<ComponentPlayerController>()->SetJumpPower(m_fObjJumpPower);
	// ジャンプパッドオブジェクトの状態を伸びるに変更
	m_pJumpPad->SetSpringState(ObjectJumpPad::E_SpringState::Stretch);
}
