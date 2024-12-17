/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(球体)用cpp
	------------------------------------
	ComponentCollisionSphere.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentCollisionSphere.h"
#include "ShapeSphere.h"				// デバッグ用図形(球)
#include "ComponentCollisionAABB.h"	// AABBコリジョンとの衝突判定
#include "ComponentCollisionOBB.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentCollisionSphere::ComponentCollisionSphere(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
	, m_fRadius(0.0f)
{
	m_eColType = COL_SPHERE;	// コリジョンの種類設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================== */
void ComponentCollisionSphere::Init()
{
	m_pShape = std::make_unique<ShapeSphere>();						// コリジョンの形状(デバッグ表示用)
	m_pShape->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);	// ワイヤーフレーム表示
	m_pShape->SetColor(WIRE_COLOR_NORMAL);							// ワイヤーフレームの色設定
}


/* ========================================
	更新関数
	-------------------------------------
	内容：更新を行う
========================================== */
void ComponentCollisionSphere::Update()
{
	// 親クラスの更新処理(トランスフォームの参照)を先に行う
	ComponentCollisionBase::Update();

	// デバッグ用図形の更新
	m_pShape->SetPosition(m_vPosition);

	// 所有オブジェクトのトランスフォームを参照する場合はスケールの最大値を半径として使用
	if (m_bRefOwnerTransform)
	{
		m_fRadius = m_vScale.GetMax() / 2;
	}

	m_fRadius += 0.003f;	// 少し余裕を持たせる(メッシュに重ならないように)

	// 表示用球のスケールを設定
	m_pShape->SetScale({ m_fRadius * 2, m_fRadius * 2, m_fRadius * 2 });

	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// 色を元に戻す


}


/* ========================================
	衝突確認分岐関数
	-------------------------------------
	内容：衝突相手の衝突判定の形状によって
		　処理を分岐させる
	-------------------------------------
	引数1：相手オブジェクトの当たり判定
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionSphere::CheckCollision(ComponentCollisionBase* otherCol)
{
	// 衝突判定結果
	bool bResult = false;	

	// 衝突相手の形状を確認
	switch (otherCol->GetColType())
	{
	// 球コリジョン
	case COL_SPHERE:	
		bResult = CheckCollisionSphere(static_cast<ComponentCollisionSphere*>(otherCol));
		break;
	// ボックスコリジョン(軸平行ボックス)
	case COL_AABB:		
		bResult = CheckCollisionAABBToSphere(static_cast<ComponentCollisionAABB*>(otherCol), this);
		break;
	// ボックスコリジョン(任意軸ボックス)
	case COL_OBB:
		bResult = CheckCollisionOBBToSphere(static_cast<ComponentCollisionOBB*>(otherCol), this);
		break;
	default:
		bResult = false;
		break;
	}

	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// 衝突していたら色を変える

	return bResult;
}

/* ========================================
	球衝突判定関数
	-------------------------------------
	内容：球同士の衝突判定を行う
	-------------------------------------
	引数1：相手オブジェクト球コリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionSphere::CheckCollisionSphere(ComponentCollisionSphere* otherCol)
{
	// 2つの球の中心間の距離(処理速度向上のため、距離の2乗を使用)
	float fDistanceSq = (m_vPosition - otherCol->GetPosition()).LengthSq();	

	// オブジェクトの半径の和(2乗)
	float fRadiusSumSq = (m_fRadius + otherCol->GetRadius()) * (m_fRadius + otherCol->GetRadius());

	return fDistanceSq < fRadiusSumSq;
}


/* ========================================
	ゲッター(半径)関数
	-------------------------------------
	戻値：半径　float
=========================================== */
float ComponentCollisionSphere::GetRadius()
{
	return m_fRadius;
}

/* ========================================
	セッター(半径)関数
	-------------------------------------
	引数1：半径　float
=========================================== */
void ComponentCollisionSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentCollisionSphere::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColSphere = Item::CreateGroup("CollisionSphere");

	ComponentCollisionBase::DebugColBase(pGroupColSphere, "CollisionSphere");

	pGroupColSphere->AddGroupItem(Item::CreateBind("Radius", Item::Kind::Float, &m_fRadius));

	window.AddItem(pGroupColSphere);
}

#endif // _DEBUG