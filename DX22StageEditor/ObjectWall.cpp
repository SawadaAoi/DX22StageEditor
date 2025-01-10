/* ========================================
	CatRobotGame/
	------------------------------------
	壁オブジェクト用cpp
	------------------------------------
	ObjectWall.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectWall.h"
#include "ComponentCollisionOBB.h"
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "TextureManager.h"

// =============== 定数定義 =======================
const float WALL_THICKNESS			= 0.01f;	// 壁の厚さ
const float WALL_SCALE				= 5.0f;		// 壁のスケール
const float DEFAULT_WALL_ROTATE_X	= -90.0f;	// 壁の初期回転

const float WALL_LIGHT_DIFFUSE = 1.0f;		// 壁の拡散光
const float WALL_LIGHT_AMBIENT = 1.0f;		// 壁の環境光

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectWall::ObjectWall(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompColOBB(nullptr)
	, m_pCompGeometry(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectWall::InitLocal()
{
	m_pCompColOBB = AddComponent<ComponentCollisionOBB>();
	m_pCompColOBB->SetDynamic(false);	// 静的オブジェクトに設定

	// トランスフォーム
	m_pCompTransform->RotateX(DEFAULT_WALL_ROTATE_X);	// 縦に立てる
	m_pCompTransform->ScaleX(WALL_SCALE);
	m_pCompTransform->ScaleZ(WALL_SCALE);
	m_pCompTransform->ScaleY(WALL_THICKNESS);			// 厚さ(薄くする)
	// 形状
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGeometry->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_PLANE);
	m_pCompGeometry->SetCulling(false);										// 両面描画
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TEX_KEY::WALL_FENCE));

	// ライトの設定(暗くなりすぎないように)
	SetLightMaterial(WALL_LIGHT_DIFFUSE, 0.0f, WALL_LIGHT_AMBIENT);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ObjectWall::UpdateLocal()
{
	// 横幅に合わせて画像をループさせる
	float fScaleX			= m_pCompTransform->GetWorldScale().x;		
	float fAdjustUvScale	= fScaleX / WALL_SCALE;
	m_pCompGeometry->SetUvScale(Vector2<float>(fAdjustUvScale, 1.0f));
}
