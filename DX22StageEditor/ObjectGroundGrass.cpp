/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト(草原)用cpp
	------------------------------------
	ObjectGroundGrass.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGroundGrass.h"
#include "ComponentGeometry.h"
#include "TextureManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
========================================== */
ObjectGroundGrass::ObjectGroundGrass(SceneBase* pScene)
	: ObjectGround(pScene)
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectGroundGrass::InitLocal()
{
	ObjectGround::InitLocal();

	// テクスチャ設定
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::GROUND_GRASS));
}
