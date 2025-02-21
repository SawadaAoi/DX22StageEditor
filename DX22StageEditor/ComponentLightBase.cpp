/* ========================================
	DX22Base/
	------------------------------------
	ライト基本コンポーネント用cpp
	------------------------------------
	ComponentLightBase.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentLightBase.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ShapeLine.h"
#include "ShapeSphere.h"
#include "ColorVec3.h"
#include "MathUtils.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentLightBase::ComponentLightBase(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderLightBase)
	, m_eLightType(E_LightType::NONE)
	, m_vColor(Vector3<float>(1.0f, 1.0f, 1.0f))
	, m_fRange(0.0f)
	, m_fAngle(0.0f)
	, m_bDispLine(false)
	, m_pDirLine(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：コンポーネントの初期化
========================================== */
void ComponentLightBase::Init()
{
	Vector3<float> vStart = m_pOwnerObj->GetTransform()->GetPosition();
	Vector3<float> vEnd = vStart + m_pOwnerObj->GetTransform()->GetForwardVector();
	m_pDirLine = std::make_unique<ShapeLine>(vStart, vEnd, ColorVec3::RED);

	m_pSphere = std::make_unique<ShapeSphere>();
	m_pSphere->SetPosition(vStart);
	m_pSphere->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
	m_pSphere->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentLightBase::Update()
{
	m_pDirLine->SetPos(m_pOwnerObj->GetTransform()->GetPosition());
	m_pDirLine->SetRotation(m_pOwnerObj->GetTransform()->GetRotation());

	switch (m_eLightType)
	{
	case ComponentLightBase::POINT:
		m_pSphere->SetPosition(m_pOwnerObj->GetTransform()->GetPosition());
		m_pSphere->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
		break;
	case ComponentLightBase::SPOT:
		m_pDirLine->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
		break;
	}


#ifdef _DEBUG
	// LightBase情報を表示しているかどうか
	if (&WIN_OBJ_INFO["LightBase"] != DebugUI::Item::dummy 
		&& WIN_OBJ_INFO["ObjectName"].GetText() == m_pOwnerObj->GetName())
	{
		DirectX::XMFLOAT4 color = WIN_OBJ_INFO["LightBase"]["Color"].GetColor();
		m_vColor			= Vector3<float>(color.x, color.y, color.z);
	}
	
#endif // _DEBUG

}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================== */
void ComponentLightBase::Draw()
{
	if (!m_bDispLine) return;
	
	if (m_eLightType == E_LightType::POINT)
	{
 		m_pSphere->Draw();
	}
	else
	{
		m_pDirLine->Draw();
	}

}

/* ========================================
	ゲッター(ライト種類)関数
	-------------------------------------
	戻値：E_LightType	ライト種類
=========================================== */
ComponentLightBase::E_LightType ComponentLightBase::GetLightType() const
{
	return m_eLightType;
}

/* ========================================
	ゲッター(ディフューズカラー)関数
	-------------------------------------
	戻値：Vector3<float>	ディフューズカラー
=========================================== */
Vector3<float> ComponentLightBase::GetColor() const
{
	return m_vColor;
}

/* ========================================
	ゲッター(ライト有効範囲)関数
	-------------------------------------
	戻値：float	ライト有効範囲
=========================================== */
float ComponentLightBase::GetRange() const
{
	return m_fRange;
}

/* ========================================
	ゲッター(スポットライト角度)関数
	-------------------------------------
	戻値：float	スポットライト角度
=========================================== */
float ComponentLightBase::GetAngle() const
{
	return m_fAngle;
}

/* ========================================
	セッター(ライト種類)関数
	-------------------------------------
	引数1：type	ライト種類
=========================================== */
void ComponentLightBase::SetLightType(E_LightType type)
{
	m_eLightType = type;
}

/* ========================================
	セッター(ディフューズカラー)関数
	-------------------------------------
	引数1：color	ディフューズカラー
=========================================== */
void ComponentLightBase::SetColor(const Vector3<float>& color)
{
	m_vColor = color;
}

/* ========================================
	セッター(ライト有効範囲)関数
	-------------------------------------
	引数1：range	ライト有効範囲
=========================================== */
void ComponentLightBase::SetRange(float range)
{
	m_fRange = range;
}

/* ========================================
	セッター(スポットライト角度)関数
	-------------------------------------
	引数1：angle	スポットライト角度
=========================================== */
void ComponentLightBase::SetAngle(float angle)
{
	m_fAngle = angle;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentLightBase::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupLight = Item::CreateGroup("LightBase");
	pGroupLight->AddGroupItem(Item::CreateValue("LightType", Item::Kind::Text));

	pGroupLight->AddGroupItem(Item::CreateValue("Color",	Item::Kind::Color));
	pGroupLight->AddGroupItem(Item::CreateBind("DispLine",	Item::Kind::Bool, &m_bDispLine));
	pGroupLight->AddGroupItem(Item::CreateBind("Range",		Item::Kind::Float, &m_fRange));
	pGroupLight->AddGroupItem(Item::CreateCallBack("Angle", Item::Kind::Float, [this](bool isWrite, void* arg) 
	{
		float* pAngle = static_cast<float*>(arg);
		if (isWrite)
			m_fAngle = MathUtils::ToRadian(*pAngle);
		else
			*pAngle = MathUtils::ToDegree(m_fAngle);

	}));

	window.AddItem(pGroupLight);

	// 初期値設定
	// 色
	WIN_OBJ_INFO["LightBase"]["Color"].SetColor(DirectX::XMFLOAT4(m_vColor.x, m_vColor.y, m_vColor.z, 1.0f));

	// ライト種類
	std::string sLightType;
	switch (m_eLightType)
	{
	case ComponentLightBase::NONE:			sLightType = "NONE";		break;
	case ComponentLightBase::DIRECTIONAL:	sLightType = "DIRECTIONAL"; break;
	case ComponentLightBase::POINT:			sLightType = "POINT";		break;
	case ComponentLightBase::SPOT:			sLightType = "SPOT";		break;
	}
	WIN_OBJ_INFO["LightBase"]["LightType"].SetText(sLightType.c_str());
}

#endif // _DEBUG
