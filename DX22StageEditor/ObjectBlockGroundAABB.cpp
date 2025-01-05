/* ========================================
	CatRobotGame/
	------------------------------------
	AABBブロックオブジェクト(地面判定有り)用cpp
	------------------------------------
	ObjectBlockGroundAABB.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBlockGroundAABB.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGroundBox.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================== */
ObjectBlockGroundAABB::ObjectBlockGroundAABB(SceneBase* pScene)
	: ObjectBlock(pScene)
{
	SetTag(E_ObjectTag::Ground);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================== */
void ObjectBlockGroundAABB::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// 地面判定
	m_eColType = E_COL_TYPE::COL_AABB;	// 衝突判定形状
	ObjectBlock::InitLocal();			// 親クラスの初期化関数を呼ぶ
}