/* ========================================
	DX22Base/
	------------------------------------
	ブロック用cpp
	------------------------------------
	ObjectBlock.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBlock.h"
#include "ComponentGeometry.h"
#include "ComponentCollisionOBB.h"
#include "TextureManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
=========================================== */
ObjectBlock::ObjectBlock(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGeometry(nullptr)
	, m_pCompColObb(nullptr)
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
void ObjectBlock::InitLocal()
{
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGeometry->SetShapeType(ComponentGeometry::TYPE_BOX);
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_SIMPLE));

	m_pCompColObb = AddComponent<ComponentCollisionOBB>();

}
