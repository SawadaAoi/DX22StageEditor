/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(AABB)用cpp
	------------------------------------
	ComponentCollisionAABB.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentCollisionAABB.h"
#include "ShapeBox.h"					// デバッグ用図形(ボックス)
#include "ComponentCollisionSphere.h"	// 球コリジョンとの衝突判定
#include "ComponentCollisionOBB.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentCollisionAABB::ComponentCollisionAABB(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
	, m_vMin(Vector3<float>(-0.5f, -0.5f, -0.5f))
	, m_vMax(Vector3<float>(0.5f, 0.5f, 0.5f))
{
	m_eColType = COL_AABB;	// コリジョンの種類設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
void ComponentCollisionAABB::Init()
{
	m_pShape = std::make_unique<ShapeBox>();						// コリジョンの形状(デバッグ表示用)
	m_pShape->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);	// ワイヤーフレーム表示
	m_pShape->SetColor(WIRE_COLOR_NORMAL);							// ワイヤーフレームの色設定
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新を行う
=========================================== */
void ComponentCollisionAABB::Update()
{
	// 親クラスの更新処理(トランスフォームの参照)を先に行う
	ComponentCollisionBase::Update();

	// 最小、最大座標更新(当たり判定の位置を更新する)
	m_vMin = m_vPosition - m_vScale * 0.5f;
	m_vMax = m_vPosition + m_vScale * 0.5f;

	// デバッグ用図形の更新
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	// AABBは常にワールド軸に平行なので回転は考慮しない

	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// 色を元に戻す

}

/* ========================================
	衝突確認分岐関数
	-------------------------------------
	内容：衝突相手の衝突判定の形状によって
		　処理を分岐させる
	-------------------------------------
	引数1：相手オブジェクトコリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionAABB::CheckCollision(ComponentCollisionBase* otherCol)
{
	bool bResult = false;
	// 衝突相手の形状を確認
	switch (otherCol->GetColType())
	{
	// ボックスコリジョン(軸平行ボックス)
	case COL_AABB:		
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// ボックスコリジョン(任意軸ボックス)
	case COL_OBB:		
		bResult = CheckCollisionAABBToOBB(this, static_cast<ComponentCollisionOBB*>(otherCol));
		break;
	// 球コリジョン
	case COL_SPHERE:	
		bResult = CheckCollisionAABBToSphere(this, static_cast<ComponentCollisionSphere*>(otherCol));
		break;

	default:
		return false;
		break;
	}

	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// 衝突していたら色を変える

	return bResult;
}

/* ========================================
	AABB衝突判定関数
	-------------------------------------
	内容：AABBと衝突しているかを確認する
	-------------------------------------
	引数1：他オブジェクトAABBコリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionAABB::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	// 以下の条件が満たされていたら衝突している
		// 自身の右面＞相手の左面 && 自身の左面＜相手の右面
	if ((this->GetMax().x >= otherCol->GetMin().x && this->GetMin().x <= otherCol->GetMax().x) &&
		// 自身の上面＞相手の下面 && 自身の下面＜相手の上面
		(this->GetMax().y >= otherCol->GetMin().y && this->GetMin().y <= otherCol->GetMax().y) &&	
		// 自身の奥面＞相手の手前面 && 自身の手前面＜相手の奥面
		(this->GetMax().z >= otherCol->GetMin().z && this->GetMin().z <= otherCol->GetMax().z))		
	{
		return true;	
	}

	return false;	// 衝突していない
}



/* ========================================
	ゲッター(最小座標(左、下、手前)関数
	-------------------------------------
	戻値：最小座標　Vector3<float>
=========================================== */
Vector3<float> ComponentCollisionAABB::GetMin()
{
	return m_vMin;
}


/* ========================================
	ゲッター(最大座標(右、上、奥)関数
	-------------------------------------
	戻値：最大座標　Vector3<float>
=========================================== */
Vector3<float> ComponentCollisionAABB::GetMax()
{
	return m_vMax;
}


/* ========================================
	セッター(最小座標(左、下、手前))関数
	-------------------------------------
	引数：最小座標　Vector3<float>
=========================================== */
void ComponentCollisionAABB::SetMin(Vector3<float> vMin)
{
	m_vMin = vMin;
}

/* ========================================
	セッター(最大座標(右、上、奥))関数
	-------------------------------------
	引数：最大座標　Vector3<float>
=========================================== */
void ComponentCollisionAABB::SetMax(Vector3<float> vMax)
{
	m_vMax = vMax;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentCollisionAABB::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColAABB = Item::CreateGroup("CollisionAABB");

	ComponentCollisionBase::DebugColBase(pGroupColAABB, "CollisionAABB");

	pGroupColAABB->AddGroupItem(Item::CreateBind("MinPos", Item::Kind::Vector, &m_vMin));
	pGroupColAABB->AddGroupItem(Item::CreateBind("MaxPos", Item::Kind::Vector, &m_vMax));

	window.AddItem(pGroupColAABB);
}

#endif // _DEBUG
