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
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// ボックスコリジョン(任意軸ボックス)
	case COL_OBB:
		bResult = CheckCollisionOBB(static_cast<ComponentCollisionOBB*>(otherCol));
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

	bool bResult = false;	// 衝突結果

	// 2つの球の中心間の距離(処理速度向上のため、距離の2乗を使用)
	float fDistanceSq = (m_vPosition - otherCol->GetPosition()).LengthSq();	

	// オブジェクトの半径の和(2乗)
	float fRadiusSumSq = (m_fRadius + otherCol->GetRadius()) * (m_fRadius + otherCol->GetRadius());

	if (fDistanceSq < fRadiusSumSq)
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	T_MTV tMtv = T_MTV();
	tMtv.bIsTrigger = otherCol->GetTrigger();
	tMtv.bIsCol = bResult;

	if (bResult)
	{
		tMtv.vAxis = (otherCol->GetPosition() - m_vPosition).GetNormalize();
		tMtv.fOverlap = otherCol->GetRadius() + m_fRadius - std::sqrt(fDistanceSq);
	}
	else
	{
		tMtv.fOverlap = 0.0f;
	}

	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());


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
bool ComponentCollisionSphere::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	bool bResult	= false;	// 衝突結果
	T_MTV tMtv		= T_MTV();	// MTV構造体

	Vector3<float> vClosestPoint;	// AABB上で球に最も近い点

	// 各軸での最も近い点を求める
	vClosestPoint.x = std::clamp(this->GetPosition().x, otherCol->GetMin().x, otherCol->GetMax().x);
	vClosestPoint.y = std::clamp(this->GetPosition().y, otherCol->GetMin().y, otherCol->GetMax().y);
	vClosestPoint.z = std::clamp(this->GetPosition().z, otherCol->GetMin().z, otherCol->GetMax().z);

	// AABB上の最も近い点から球の中心へのベクトル
	Vector3<float> vSphCenToCloAABB = vClosestPoint - this->GetPosition();

	float fDistanceSq = vSphCenToCloAABB.LengthSq();						// 距離の2乗
	float fRadiusSq = this->GetRadius() * this->GetRadius();	// 球の半径の2乗

	// 球の半径よりも距離が近い場合は衝突している
	if (fDistanceSq < fRadiusSq)
	{
		tMtv.vAxis		= vSphCenToCloAABB.GetNormalize();				// 衝突方向	
		tMtv.fOverlap	= this->GetRadius() - std::sqrt(fDistanceSq);	// 重なり量

		bResult = true;
	}
	else
	{
		tMtv.fOverlap = 0.0f;	// 重なり量

		bResult = false;
	}

	tMtv.bIsCol		= bResult;					// 衝突判定結果
	tMtv.bIsTrigger = otherCol->GetTrigger();	// トリガー判定(すり抜け判定)

	// 衝突相手のMTVを設定
	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
}

/* ========================================
	OBB衝突判定関数
	-------------------------------------
	内容：OBBと衝突しているかを確認する
	-------------------------------------
	引数1：他オブジェクトOBBコリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionSphere::CheckCollisionOBB(ComponentCollisionOBB* otherCol)
{
	bool bResult	= false;	// 衝突結果
	T_MTV tMtv		= T_MTV();	// MTV構造体

	// OBBの構造体作成
	ComponentCollisionOBB::T_OBB tObb = otherCol->CreateOBB(otherCol);
	// 球の中心からOBBの中心までのベクトルを計算
	Vector3<float> vDis = this->GetPosition() - otherCol->GetPosition();
	// OBB上の最も近い点を求める
	Vector3<float> vClosestOBB = otherCol->GetPosition();	// OBBの中心を初期値とする


	// OBBの各ローカル軸に対して、球の中心からOBBの中心までのベクトルを射影
	for (int i = 0; i < 3; i++)
	{
		// OBBのローカル軸を取得
		Vector3<float> vAxis = tObb.vAxis[i];

		// 球の中心からOBBの中心までのベクトルをOBBのローカル軸に射影
		float fDis = vDis.Dot(vAxis);

		// OBBのローカル軸に射影したベクトルをOBBのローカル軸に戻す
		// OBBの表面上の座標に変換し、OBBの中心座標に加算
		// この処理でOBB表面上の最も近い点を求める
		vClosestOBB += vAxis * std::clamp(fDis, -tObb.fExtent[i], tObb.fExtent[i]);

	}

	Vector3<float> vSphCenToCloOBB = vClosestOBB - this->GetPosition();	// 球の中心からOBB上の最も近い点までのベクトル
	// 最も近い点と球の中心との距離を求める(計算量を減らすために2乗距離で比較)
	float fDistanceSq = vSphCenToCloOBB.LengthSq();
	float fRadiusSq = this->GetRadius() * this->GetRadius();	// 球の半径の2乗

	// 球の半径よりも距離が近い場合は衝突している
	if (fDistanceSq < fRadiusSq)
	{
		tMtv.vAxis		= vSphCenToCloOBB.GetNormalize();				// 衝突方向
		tMtv.fOverlap	= this->GetRadius() - std::sqrt(fDistanceSq);	// 重なり量

		bResult = true;
	}
	else
	{
		tMtv.fOverlap = 0.0f;	// 重なり量

		bResult = false;
	}

	tMtv.bIsCol		= bResult;					// 衝突判定結果
	tMtv.bIsTrigger = otherCol->GetTrigger();	// トリガー判定(すり抜け判定)

	// 衝突相手のMTVを設定
	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
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