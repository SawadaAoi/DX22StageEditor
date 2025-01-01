/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(OBB)用cpp
	------------------------------------
	ComponentCollisionOBB.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentCollisionOBB.h"
#include "ShapeBox.h"
#include "ComponentTransform.h"
#include <cmath>	// 絶対値
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ObjectBase.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentCollisionOBB::ComponentCollisionOBB(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
{
	m_eColType = E_ColType::COL_OBB;	// コリジョンの種類設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
void ComponentCollisionOBB::Init()
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
void ComponentCollisionOBB::Update()
{

	// 親クラスの更新処理(トランスフォームの参照)を先に行う
	ComponentCollisionBase::Update();

	// デバッグ用図形の更新
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	m_pShape->SetRotation(m_qRotation);
	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// 色を元に戻す

}

/* ========================================
	衝突確認分岐関数
	-------------------------------------
	内容：衝突相手の衝突判定の形状によって
		　処理を分岐させる
	-------------------------------------
	引数1：衝突相手コリジョン
	-------------------------------------
	戻り値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionOBB::CheckCollision(ComponentCollisionBase* otherCol)
{
	// MTV構造体リストに追加(衝突確認中のオブジェクト用)
	bool bResult = false;
	// 衝突相手の形状を確認
	switch (otherCol->GetColType())
	{
	// ボックスコリジョン(任意軸ボックス)
	case COL_OBB:	
		bResult = CheckCollisionOBB(otherCol);
		break;
	// ボックスコリジョン(軸平行ボックス)
	case COL_AABB:	
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// 球コリジョン
	case COL_SPHERE:	
		bResult = CheckCollisionSphere(static_cast<ComponentCollisionSphere*>(otherCol));
		break;
	default:
		return false;
		break;
	}


	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// 衝突していたら色を変える

	return bResult;
}

/* ========================================
	OBB衝突判定関数
	------------------------------------
	内容：OBBと衝突しているかを確認する
	-------------------------------------
	引数1：相手OBBコリジョン
	※CheckCollisionAABBToOBBで使用するために
	　引数をComponentCollisionBaseにしている
	　(AABBからOBBにcastできないため)
	-------------------------------------
	戻り値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionOBB::CheckCollisionOBB(ComponentCollisionBase* otherCol)
{
	bool bResult = false;	// 衝突結果

	// OBB同士の中心間の距離
	Vector3<float> vDist = (this->GetPosition() - otherCol->GetPosition());

	// OBBの構造体作成
	T_OBB tObb1 = CreateOBB(static_cast<ComponentCollisionBase*>(this));		// 自身のOBB
	T_OBB tObb2 = CreateOBB(otherCol);											// 相手のOBB

	//  2つのOBB同士を分離する軸が存在するか確認する
	if (CheckLocalSeparateAxis(tObb1, tObb2, vDist) ||	// ローカル軸分離判定(自身のXYZ軸が分離軸)
		CheckLocalSeparateAxis(tObb2, tObb1, vDist) ||	// ローカル軸分離判定(相手のXYZ軸が分離軸)
		CheckCrossSeparateAxis(tObb1, tObb2, vDist))	// OBB同士の交差軸分離判定(自身のXYZ軸と相手のXYZ軸の外積が分離軸)
	{
		bResult = false;	// 分離軸が見つかった(衝突していない)
	}
	else
	{
		bResult = true;		// 分離軸が見つからなかった(衝突)

	}

	// MTV構造体リストのトリガー判定(すり抜け判定)を更新
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == otherCol->GetOwnerObject()->GetName())
		{
			mtv.bIsTrigger = otherCol->GetTrigger();
			// 衝突していない場合、重なり量を0にする
			if (!bResult) mtv.fOverlap = 0.0f;

			break;
		}
	}

	return bResult;
}

/* ========================================
	OBB構造体作成関数
	-------------------------------------
	内容：OBB構造体を作成する
	-------------------------------------
	引数1：OBBコリジョン
	-------------------------------------
	戻り値：OBB構造体
=========================================== */
ComponentCollisionOBB::T_OBB ComponentCollisionOBB::CreateOBB(ComponentCollisionBase* Col)
{
	T_OBB tObb;

	// 中心座標
	tObb.vCenter			= Col->GetPosition();
	// ローカル軸
	tObb.vAxis[AXIS_X]		= Col->GetRotation().GetRightVector();
	tObb.vAxis[AXIS_Y]		= Col->GetRotation().GetUpVector();
	tObb.vAxis[AXIS_Z]		= Col->GetRotation().GetForwardVector();
	// 各軸の長さ(ボックスの大きさの半分)
	tObb.fExtent[AXIS_X]	= Col->GetScale().x * 0.5f;
	tObb.fExtent[AXIS_Y]	= Col->GetScale().y * 0.5f;
	tObb.fExtent[AXIS_Z]	= Col->GetScale().z * 0.5f;

	// 衝突相手オブジェクト名
	tObb.sName = Col->GetOwnerObject()->GetName();

	return tObb;
}

/* ========================================
	ローカル分離軸判定関数
	-------------------------------------
	内容：OBB同士のローカル軸分離判定を行う
	-------------------------------------
	引数1：ローカルのXYZ軸が分離軸となるOBB
	引数2：相手のOBB
	引数3：OBB同士の中心間のベクトル
	-------------------------------------
	戻値：true=分離軸が見つかった(衝突していない)
		　false=分離軸が見つからなかった(衝突している)
=========================================== */
bool ComponentCollisionOBB::CheckLocalSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis)
{
	bool bResult = false;	// 衝突結果
	T_MTV tMtv = T_MTV();	// 最小移動ベクトル構造体
	// 相手のOBBのMTV構造体を取得
	if (m_pOwnerObj->GetName() == tObb1.sName)		tMtv = GetMtv(tObb2.sName);	// 自身が分離軸の場合
	else if (m_pOwnerObj->GetName() == tObb2.sName)	tMtv = GetMtv(tObb1.sName);	// 相手が分離軸の場合

	// 自身OBBの各ローカル軸を分離軸として、相手OBBとの分離判定を行う
	for (int i = 0; i < 3; i++)
	{
		// 分離軸を設定
		Vector3<float> vAxis = tObb1.vAxis[i];		

		// 軸方向のOBBの長さ
		float fLength1	= tObb1.fExtent[i];						// 分離軸がローカル軸の場合、他の軸方向は垂直なので、長さをそのまま使う
		float fLength2	= GetProjectionLength(vAxis, tObb2);	// 相手OBBの射影線の長さを取得

		float fDis		= std::abs(vDis.Dot(vAxis));			// 分離軸に中心間の距離を射影(同じ線上で比較を行う)

		float fOverlap	= (fLength1 + fLength2) - fDis;			// 重なり量

		// 重なり量が0未満＝分離軸が見つかった
		if (fOverlap < 0.0f)
		{
			bResult			= true;
		}
		// 重なり量が最小の場合、最小移動ベクトルを更新
		else if (fOverlap < tMtv.fOverlap)
		{
			tMtv.vAxis		= (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// 衝突方向ベクトルの軸(中心間のベクトルの向きによって反転)
			tMtv.fOverlap	= fOverlap;												// 最小移動ベクトルの重なり量

			bResult			= false;
		}

		tMtv.bIsCol = !bResult;	// 衝突結果(bResultがtrue=衝突していないので反転)

		if (bResult) break;		// 分離軸が見つかったらループを抜ける
	}

	// 相手のOBBのMTV構造体を更新
	if		(m_pOwnerObj->GetName() == tObb1.sName)	SetColObjMTV(tMtv, tObb2.sName);	// 自身が分離軸の場合
	else if (m_pOwnerObj->GetName() == tObb2.sName)	SetColObjMTV(tMtv, tObb1.sName);	// 相手が分離軸の場合

	return bResult;	// 分離軸が見つからなかった
}

/* ========================================
	交差軸分離判定関数
	-------------------------------------
	内容：OBB同士の交差軸分離判定を行う
		　OBBのローカルXYZ軸の外積が分離軸となる
		  3軸×3軸の9通りの分離軸を調べる
	-------------------------------------
	引数1：自身のOBB
	引数2：相手のOBB
	引数3：OBB同士の中心間のベクトル
	-------------------------------------
	戻値：true=分離軸が見つかった(衝突していない)
		　false=分離軸が見つからなかった(衝突している)
=========================================== */
bool ComponentCollisionOBB::CheckCrossSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis)
{
	bool bResult	= false;	// 衝突結果
	bool bExitLoop	= false;	// 2重ループを抜けるフラグ

	T_MTV tMtv = T_MTV();		// 最小移動ベクトル構造体
	tMtv = GetMtv(tObb2.sName);	// 相手のOBBのMTV構造体を取得

	// 自身のOBBの3軸分
	for (int i = 0; i < 3; i++)
	{
		// 相手のOBBの3軸分
		for (int j = 0; j < 3; j++)
		{
			// 分離軸を設定
			// 自身のOBBの軸と相手のOBBの軸の外積を取得
			Vector3<float> vAxis = tObb1.vAxis[i].Cross(tObb2.vAxis[j]);

			// 外積が0ベクトルだったらスキップ
			if (vAxis.LengthSq() < 0.0001f)	continue;

			// 外積ベクトルを正規化
			vAxis.Normalize();

			// 分離軸に射影して軸上の長さを取得
			float fLength1 = GetProjectionLength(vAxis, tObb1);	// 自身OBB
			float fLength2 = GetProjectionLength(vAxis, tObb2);	// 相手OBB

			float fDis = std::abs(vDis.Dot(vAxis));	// 分離軸に中心間の距離を射影(同じ線上で比較を行う)

			float fOverlap = (fLength1 + fLength2) - fDis;		// 重なり量

			// 重なり量が0未満＝分離軸が見つかった
			if (fOverlap < 0.0f)
			{
				bResult		= true;
				bExitLoop	= true;
			}
			// 重なり量が最小の場合、最小移動ベクトルを更新
			else if (fOverlap < tMtv.fOverlap)
			{
				// 最後尾のMTV構造体を更新
				tMtv.vAxis		= (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// 衝突方向ベクトルの軸(中心間のベクトルの向きによって反転)
				tMtv.fOverlap	= fOverlap;	// 最小移動ベクトルの重なり量

				bResult			= false;
			}

			tMtv.bIsCol = !bResult;	// 衝突結果(bResultがtrue=衝突していないので反転)

			if (bResult) break;		// 分離軸が見つかったらループを抜ける
		}
		if (bExitLoop) break;
	}

	// 相手のOBBのMTV構造体を更新
	SetColObjMTV(tMtv, tObb2.sName);

	return bResult;	// 分離軸が見つからなかった
}

/* ========================================
	射影線長さ取得関数
	-------------------------------------
	内容：OBBの各軸に対して射影線の長さを取得する
	-------------------------------------
	引数1：射影軸
	引数2：OBB構造体
	-------------------------------------
	戻値：射影線の長さ(軸に投影したOBBの長さ)
=========================================== */
float ComponentCollisionOBB::GetProjectionLength(Vector3<float> vAxis, T_OBB tObb)
{
	float fLength = 0.0f;	// 射影線の長さ

	Vector3<float> vRight	= tObb.vAxis[AXIS_X] * tObb.fExtent[AXIS_X];	// 右軸
	Vector3<float> vUp		= tObb.vAxis[AXIS_Y] * tObb.fExtent[AXIS_Y];	// 上軸
	Vector3<float> vForward = tObb.vAxis[AXIS_Z] * tObb.fExtent[AXIS_Z];	// 前軸

	// 各軸の長さを求め、射影線の長さに加算
	fLength += std::abs(vRight.Dot(vAxis));		// 自身の右方向を分離軸として射影
	fLength += std::abs(vUp.Dot(vAxis));		// 自身の上方向を分離軸として射影
	fLength += std::abs(vForward.Dot(vAxis));	// 自身の前方向を分離軸として射影

	return fLength;
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
bool ComponentCollisionOBB::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	// AABBを親クラスにキャスト
	ComponentCollisionOBB* pAABBAsOBB = reinterpret_cast<ComponentCollisionOBB*>(otherCol);

	// AABBをOBBとして扱うために値を設定
	pAABBAsOBB->SetPosition(otherCol->GetPosition());				// 座標はAABBの座標
	pAABBAsOBB->SetScale(otherCol->GetMax() - otherCol->GetMin());		// 大きさはAABBの最大座標と最小座標の差
	pAABBAsOBB->SetRotation(Quaternion());						// 回転はなし

	return this->CheckCollisionOBB(pAABBAsOBB);
}

/* ========================================
	球衝突判定関数
	-------------------------------------
	内容：球と衝突しているかを確認する
	-------------------------------------
	引数1：他オブジェクト球コリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionOBB::CheckCollisionSphere(ComponentCollisionSphere* otherCol)
{
	bool bResult = false;	// 衝突結果

	// OBBの構造体作成
	ComponentCollisionOBB::T_OBB tObb = CreateOBB(this);
	// 球の中心からOBBの中心までのベクトルを計算
	Vector3<float> vDis = otherCol->GetPosition() - this->GetPosition();
	// OBB上の最も近い点を求める
	Vector3<float> vClosestOBB = this->GetPosition();	// OBBの中心を初期値とする


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

	Vector3<float> vSphCenToCloOBB = otherCol->GetPosition() - vClosestOBB;	// 球の中心からOBB上の最も近い点までのベクトル
	// 最も近い点と球の中心との距離を求める(計算量を減らすために2乗距離で比較)
	float fDistanceSq = vSphCenToCloOBB.LengthSq();
	float fRadiusSq = otherCol->GetRadius() * otherCol->GetRadius();	// 球の半径の2乗

	// 球の半径よりも距離が近い場合は衝突している
	if (fDistanceSq < fRadiusSq)
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	T_MTV tMtv = T_MTV();	// MTV構造体
	tMtv.sName		= otherCol->GetOwnerObject()->GetName();
	tMtv.bIsTrigger = otherCol->GetTrigger();
	tMtv.bIsCol		= bResult;

	if (bResult)
	{
		tMtv.vAxis = vSphCenToCloOBB.GetNormalize();
		tMtv.fOverlap = otherCol->GetRadius() - std::sqrt(fDistanceSq);
	}
	else
	{
		tMtv.fOverlap = 0.0f;
	}

	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
}



#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentCollisionOBB::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColOBB = Item::CreateGroup("CollisionOBB");

	ComponentCollisionBase::DebugColBase(pGroupColOBB, "CollisionOBB");



	window.AddItem(pGroupColOBB);
}

#endif // _DEBUG
