/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト用cpp
	------------------------------------
	ObjectLight.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectLight.h"
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "LightManager.h"
#include "TextureManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ObjectLight::ObjectLight(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompLightBase(nullptr)
{
	SetTag(E_ObjectTag::Light);
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：オブジェクトの初期化
========================================== */
void ObjectLight::InitLocal()
{
	LIGHT_MNG_INST.AddLight(this);

	// コンポーネント取得
	m_pCompTransform = GetComponent<ComponentTransform>();
	m_pCompLightBase = AddComponent<ComponentLightBase>();

	ComponentBillboard* pCompBillboard = AddComponent<ComponentBillboard>();
	pCompBillboard->SetTexture(GET_TEXTURE_DATA(TEX_KEY::LIGHT_ICON));
}

/* ========================================
	終了関数
	-------------------------------------
	内容：オブジェクトの終了処理
========================================== */
void ObjectLight::UninitLocal()
{
	LIGHT_MNG_INST.RemoveLight(this);
}

/* ========================================
	ゲッター(座標)関数
	-------------------------------------
	戻値：Vector3<float>	ライト位置
=========================================== */
Vector3<float> ObjectLight::GetPosition() const
{
	return m_pCompTransform->GetWorldPosition();
}

/* ========================================
	ゲッター(ライトタイプ)関数
	-------------------------------------
	戻値：ComponentLightBase::E_LightType	ライトタイプ
=========================================== */
ComponentLightBase::E_LightType ObjectLight::GetLightType() const
{
	return m_pCompLightBase->GetLightType();
}

/* ========================================
	ゲッター(ディフューズカラー)関数
	-------------------------------------
	戻値：Vector3<float>	ディフューズカラー
=========================================== */
Vector3<float> ObjectLight::GetColor() const
{
	return m_pCompLightBase->GetColor();
}

/* ========================================
	ゲッター(ライト方向)関数
	-------------------------------------
	戻値：Vector3<float>	ライト方向
=========================================== */
Vector3<float> ObjectLight::GetDirection() const
{
	return m_pCompTransform->GetForwardVector();
}

/* ========================================
	ゲッター(ライト有効範囲)関数
	-------------------------------------
	戻値：float	ライト有効範囲
=========================================== */
float ObjectLight::GetRange() const
{
	return m_pCompLightBase->GetRange();
}

/* ========================================
	ゲッター(スポットライト角度)関数
	-------------------------------------
	戻値：float	スポットライト角度
=========================================== */
float ObjectLight::GetAngle() const
{
	return m_pCompLightBase->GetAngle();
}

/* ========================================
	セッター(ディフューズカラー)関数
	-------------------------------------
	引数1：color	ディフューズカラー
=========================================== */
void ObjectLight::SetColor(const Vector3<float>& color)
{
	m_pCompLightBase->SetColor(color);
}

/* ========================================
	セッター(ライト有効範囲)関数
	-------------------------------------
	引数1：range	ライト有効範囲
=========================================== */
void ObjectLight::SetRange(float range)
{
	m_pCompLightBase->SetRange(range);
}

/* ========================================
	セッター(スポットライト角度)関数
	-------------------------------------
	引数1：angle	スポットライト角度
=========================================== */
void ObjectLight::SetAngle(float angle)
{
	m_pCompLightBase->SetAngle(angle);
}

