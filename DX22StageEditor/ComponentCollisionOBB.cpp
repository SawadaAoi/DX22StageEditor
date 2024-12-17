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
#ifdef _DEBUG
	, m_nSelectMTV(-1)	// 選択中の最小移動ベクトル
#endif // _DEBUG
{
	m_eColType = E_ColType::COL_OBB;	// コリジョンの種類設定
	m_tMtvs.clear();	// MTV構造体リストクリア
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
#ifdef _DEBUG

	// オブジェクト詳細が自身かつ、OBBコンポーネントが表示されている場合
	if (CHECK_DISP_COMP("CollisionOBB"))
	{

		// 現在選択中の最小移動ベクトルを取得
		if (m_tMtvs.size() > 0 && m_nSelectMTV != -1)
		{
			// 最小移動ベクトル構造体の表示
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Axis"].SetVector(m_tMtvs.at(m_nSelectMTV).vAxis.ToXMFLOAT3());
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Overlap"].SetFloat(m_tMtvs.at(m_nSelectMTV).fOverlap);
			WIN_OBJ_INFO["CollisionOBB"]["MTV_IsCol"].SetBool(m_tMtvs.at(m_nSelectMTV).bIsCol);
		}
		else
		{
			// MTV構造体リストが空の場合、初期値を表示
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Axis"].SetVector(Vector3<float>(0.0f, 0.0f, 0.0f).ToXMFLOAT3());
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Overlap"].SetFloat(0);
			WIN_OBJ_INFO["CollisionOBB"]["MTV_IsCol"].SetBool(false);
		}

		WIN_OBJ_INFO["CollisionOBB"]["MTVs"].RemoveListItemAll();	// MTV構造体リストをクリア
	}

#endif // _DEBUG
	// 親クラスの更新処理(トランスフォームの参照)を先に行う
	ComponentCollisionBase::Update();

	// デバッグ用図形の更新
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	m_pShape->SetRotation(m_qRotation);
	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// 色を元に戻す

	// MTV構造体配列クリア
	m_tMtvs.clear();

	// MTV構造体リストに当たり判定を持つオブジェクトを追加
	for (const auto& colObj : m_bColStatesMap)
	{
		// 名前のみ登録
		T_MTV mtv = T_MTV();
		mtv.sName = colObj.first->GetName();
		m_tMtvs.push_back(mtv);

#ifdef _DEBUG
		// オブジェクト詳細が自身かつ、OBBコンポーネントが表示されている場合
		if (CHECK_DISP_COMP("CollisionOBB"))
		{
			DebugUI::Item* list = &WIN_OBJ_INFO["CollisionOBB"]["MTVs"];	// MTVリスト取得
			list->AddListItem(("MTV_" + mtv.sName).c_str());				// MTVリストに追加
		}
#endif // _DEBUG
	}

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
		bResult = CheckCollisionAABBToOBB(static_cast<ComponentCollisionAABB*>(otherCol), this);
		break;
		// 球コリジョン
	case COL_SPHERE:
		bResult = CheckCollisionOBBToSphere(this, static_cast<ComponentCollisionSphere*>(otherCol), true);
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
	bool bResult = false;

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

	// MTV構造体リストの重なり量、Triger判定を更新
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == otherCol->GetOwnerObject()->GetName())
		{
			mtv.bIsTrigger = otherCol->GetTrigger();
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
	tObb.vCenter = Col->GetPosition();
	// ローカル軸
	tObb.vAxis[AXIS_X] = Col->GetRotation().GetRightVector();
	tObb.vAxis[AXIS_Y] = Col->GetRotation().GetUpVector();
	tObb.vAxis[AXIS_Z] = Col->GetRotation().GetForwardVector();
	// 各軸の長さ(ボックスの大きさの半分)
	tObb.fExtent[AXIS_X] = Col->GetScale().x * 0.5f;
	tObb.fExtent[AXIS_Y] = Col->GetScale().y * 0.5f;
	tObb.fExtent[AXIS_Z] = Col->GetScale().z * 0.5f;

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
	bool bResult = false;
	T_MTV tMtv = T_MTV();	// 最小移動ベクトル構造体

	// 自身OBBの各ローカル軸を分離軸として、相手OBBとの分離判定を行う
	for (int i = 0; i < 3; i++)
	{
		// 分離軸を設定
		Vector3<float> vAxis = tObb1.vAxis[i];

		// 軸方向のOBBの長さ
		float fLength1 = tObb1.fExtent[i];						// 分離軸がローカル軸の場合、他の軸方向は垂直なので、長さをそのまま使う
		float fLength2 = GetProjectionLength(vAxis, tObb2);	// 相手OBBの射影線の長さを取得

		float fDis = std::abs(vDis.Dot(vAxis));			// 分離軸に中心間の距離を射影(同じ線上で比較を行う)

		float fOverlap = (fLength1 + fLength2) - fDis;		// 重なり量

		// 重なり量が0未満＝分離軸が見つかった
		if (fOverlap < 0.0f)
		{
			tMtv.bIsCol = false;
			bResult = true;
			break;
		}
		// 重なり量が最小の場合、最小移動ベクトルを更新
		else if (fOverlap < tMtv.fOverlap)
		{
			tMtv.vAxis = (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// 衝突方向ベクトルの軸(中心間のベクトルの向きによって反転)
			tMtv.fOverlap = fOverlap;	// 最小移動ベクトルの重なり量
			tMtv.bIsCol = true;		// 衝突している

			bResult = false;
		}
	}

	// 同じ名前のMTVを更新
	for (auto& mtv : m_tMtvs)
	{
		if ((mtv.sName == tObb2.sName && m_pOwnerObj->GetName() == tObb1.sName)
			|| (mtv.sName == tObb1.sName && m_pOwnerObj->GetName() == tObb2.sName))
		{
			mtv.bIsCol = tMtv.bIsCol;
			mtv.vAxis = tMtv.vAxis;
			mtv.fOverlap = tMtv.fOverlap;
			break;
		}
	}

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
	bool bResult = false;
	bool bExitLoop = false;

	T_MTV tMtv = T_MTV();	// 最小移動ベクトル構造体

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
				tMtv.bIsCol = false;	// 衝突していない
				bResult = true;

				bExitLoop = true;
				break;
			}
			// 重なり量が最小の場合、最小移動ベクトルを更新
			else if (fOverlap < tMtv.fOverlap)
			{
				// 最後尾のMTV構造体を更新
				tMtv.vAxis = (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// 衝突方向ベクトルの軸(中心間のベクトルの向きによって反転)
				tMtv.fOverlap = fOverlap;	// 最小移動ベクトルの重なり量
				tMtv.bIsCol = true;		// 衝突している

				bResult = false;
			}

		}
		if (bExitLoop) break;
	}

	// 同じ名前のMTVを更新
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == tObb2.sName)
		{
			mtv.bIsCol = tMtv.bIsCol;
			mtv.vAxis = tMtv.vAxis;
			mtv.fOverlap = tMtv.fOverlap;
			break;
		}
	}

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

	Vector3<float> vRight = tObb.vAxis[AXIS_X] * tObb.fExtent[AXIS_X];	// 右軸
	Vector3<float> vUp = tObb.vAxis[AXIS_Y] * tObb.fExtent[AXIS_Y];	// 上軸
	Vector3<float> vForward = tObb.vAxis[AXIS_Z] * tObb.fExtent[AXIS_Z];	// 前軸

	// 各軸の長さを求め、射影線の長さに加算
	fLength += std::abs(vRight.Dot(vAxis));		// 自身の右方向を分離軸として射影
	fLength += std::abs(vUp.Dot(vAxis));		// 自身の上方向を分離軸として射影
	fLength += std::abs(vForward.Dot(vAxis));	// 自身の前方向を分離軸として射影

	return fLength;
}


/* ========================================
	ゲッター(最小移動ベクトル構造体配列)関数
	-------------------------------------
	戻値：vector<T_MTV>	最小移動ベクトル構造体配列
=========================================== */
std::vector<ComponentCollisionOBB::T_MTV> ComponentCollisionOBB::GetMtvs()
{
	return m_tMtvs;
}

/* ========================================
	セッター(最小移動ベクトル構造体配列)関数
	-------------------------------------
	引数：最小移動ベクトル構造体配列
=========================================== */
void ComponentCollisionOBB::SetMtvs(std::vector<T_MTV> mtvs)
{
	m_tMtvs = mtvs;
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

	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Name", Item::Kind::Text));	// 衝突相手オブジェクト名
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Axis", Item::Kind::Vector));	// 最小移動ベクトルの軸
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Overlap", Item::Kind::Float));	// 最小移動ベクトルの重なり量
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_IsCol", Item::Kind::Bool));		// 衝突しているか

	// 衝突構造体リストのコールバック関数
	Item::ConstCallback callback = [this](const void* arg)
	{
		std::string sObjName = reinterpret_cast<const char*>(arg);	// リスト項目名

		// リスト番号取得
		Item* pList = &WIN_OBJ_INFO["CollisionOBB"]["MTVs"];
		m_nSelectMTV = pList->GetListNo(sObjName.c_str());

		WIN_OBJ_INFO["CollisionOBB"]["MTV_Name"].SetText(m_tMtvs.at(m_nSelectMTV).sName.c_str());	// 衝突相手オブジェクト名
	};

	// 最小移動ベクトル構造体配列の表示
	pGroupColOBB->AddGroupItem(Item::CreateList("MTVs", callback));

	window.AddItem(pGroupColOBB);
}

#endif // _DEBUG
