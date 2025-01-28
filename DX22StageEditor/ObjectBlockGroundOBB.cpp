/* ========================================
	CatRobotGame/
	------------------------------------
	OBBブロックオブジェクト(地面判定有り)用cpp
	------------------------------------
	ObjectBlockGroundOBB.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBlockGroundOBB.h"
#include "ComponentGroundBox.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================== */
ObjectBlockGroundOBB::ObjectBlockGroundOBB(SceneBase* pScene)
	: ObjectBlock(pScene)
{
	SetTag(E_ObjectTag::Ground);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================== */
void ObjectBlockGroundOBB::InitLocal()
{
	AddComponent<ComponentGroundBox>();	// 地面判定
	m_eColType = E_COL_TYPE::COL_OBB;	// 衝突判定形状
	ObjectBlock::InitLocal();			// 親クラスの初期化関数を呼ぶ
}