/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト(スポットイト)用cpp
	------------------------------------
	ObjectLightSpot.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectLightSpot.h"
#include "MathUtils.h"

// =============== 定数定義 =======================
const float DEFAULT_RANGE = 3.0f;	// デフォルトの範囲
const float DEFAULT_ANGLE = MathUtils::ToRadian(30.0f);	// デフォルトの角度


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ObjectLightSpot::ObjectLightSpot(SceneBase* pScene)
	: ObjectLight(pScene)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：オブジェクトの初期化
========================================== */
void ObjectLightSpot::InitLocal()
{
	// 親クラスの初期化関数を呼ぶ
	ObjectLight::InitLocal();

	// ライト種類を設定
	m_pCompLightBase->SetLightType(ComponentLightBase::E_LightType::SPOT);
	// デフォルトの範囲を設定
	m_pCompLightBase->SetRange(DEFAULT_RANGE);
	// デフォルトの角度を設定
	m_pCompLightBase->SetAngle(DEFAULT_ANGLE);
}
