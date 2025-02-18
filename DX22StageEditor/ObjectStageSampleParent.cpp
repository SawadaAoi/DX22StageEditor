/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ステージ見本親)用cpp
	------------------------------------
	ObjectStageSampleParent.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectStageSampleParent.h"
#include "ComponentTransform.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectStageSampleParent::ObjectStageSampleParent(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_fRotateSpeed(10.0f)
	, m_bIsRotate(true)
{
	SetTag(E_ObjectTag::StageSampleParent);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectStageSampleParent::UpdateLocal()
{
	if (!m_bIsRotate) return;
	// 回転
	m_pCompTransform->RotateY(m_fRotateSpeed * DELTA_TIME);
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectStageSampleParent::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupStageSample = Item::CreateGroup("StageSample");

	// 回転フラグ
	pGroupStageSample->AddGroupItem(Item::CreateBind("IsRotate", Item::Kind::Bool, &m_bIsRotate));
	// 回転速度
	pGroupStageSample->AddGroupItem(Item::CreateBind("RotateSpeed", Item::Kind::Float, &m_fRotateSpeed));
}
#endif // DEBUG