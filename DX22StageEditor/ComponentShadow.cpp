/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(丸影)用cpp
	------------------------------------
	ComponentShadow.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentShadow.h"
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include "ShadowManager.h"
#include "MathUtils.h"

// =============== 定数定義 =======================
const float DEFAULT_RANGE = 0.6f;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentShadow::ComponentShadow(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderShadow)
	, m_fCircleSize(DEFAULT_RANGE)
	, m_vPosOffset({0.0f,0.0f,0.0f})
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ComponentShadow::Init()
{
	SHADOW_MNG_INST.AddShadow(this);
}


/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void ComponentShadow::Uninit()
{
	SHADOW_MNG_INST.RemoveShadow(this);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentShadow::Update()
{
	// オブジェクトの大きさによって、影の大きさを変える
	m_fCircleSize = DEFAULT_RANGE * m_pOwnerObj->GetTransform()->GetScale().x;
}

/* ========================================
	ゲッター(座標)関数
	-------------------------------------
	戻値：vVector3<float> 座標
=========================================== */
Vector3<float> ComponentShadow::GetPosition() const
{
	return  m_pOwnerObj->GetTransform()->GetPosition() + m_vPosOffset;
}

/* ========================================
	ゲッター(円サイズ)関数
	-------------------------------------
	戻値：float 円の大きさ
=========================================== */
float ComponentShadow::GetCircleSize() const
{
	return m_fCircleSize;
}

/* ========================================
	セッター(円サイズ)関数
	-------------------------------------
	引数：float 円の大きさ
=========================================== */
void ComponentShadow::SetCircleSize(float range)
{
	m_fCircleSize = range;
}

/* ========================================
	セッター(基準座標調整値)関数
	-------------------------------------
	引数：Vector3<float> 調整値
=========================================== */
void ComponentShadow::SetPosOffset(Vector3<float> vOffset)
{
	m_vPosOffset = vOffset;
}

