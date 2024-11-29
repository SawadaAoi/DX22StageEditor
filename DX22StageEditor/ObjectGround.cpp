/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト用cpp
	------------------------------------
	ObjectGround.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGround.h"
#include "ComponentTransform.h"
#include "ComponentGeometry.h"
#include "ColorVec3.h"
#include "ComponentGround.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
========================================== */
ObjectGround::ObjectGround(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pTransform(nullptr)
	, m_pGeometry(nullptr)
{
	m_eTag = E_ObjectTag::Ground;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectGround::InitLocal()
{
	m_pTransform =GetComponent<ComponentTransform>();
	m_pGeometry = AddComponent<ComponentGeometry>();
	AddComponent<ComponentGround>();

	m_pGeometry->SetShapeType(ComponentGeometry::TYPE_PLANE);
	//m_pTransform->SetLocalScale(Vector3(5.0f, 0.0f, 5.0f));
	//m_pTransform->RotateZ(10.0f);	// 回転

}


/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
========================================== */
void ObjectGround::UninitLocal()
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ObjectGround::UpdateLocal()
{



}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================== */
void ObjectGround::DrawLocal()
{
}
