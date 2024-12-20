/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント基底用cpp
	------------------------------------
	ComponentCollisionBase.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentCollisionBase.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionSphere.h"
#include <algorithm>

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
=========================================== */
ComponentCollisionBase::ComponentCollisionBase(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderCollisionBase)
	, m_pOwnerTransform(nullptr)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vPositionOffset(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation(Quaternion())
	, m_bRefOwnerTransform(true)
	, m_eColType(COL_NONE)
	, m_pShape(nullptr)
	, m_bIsDynamic(true)
	, m_bIsEnabled(true)
	, m_bIsDispCol(true)
	, m_bIsTrigger(false)
#ifdef _DEBUG
	, m_pColObjList(nullptr)
#endif // _DEBUG

{
	m_bColStatesMap.clear();

	// 所持オブジェクトの所属シーンの衝突判定管理リストに追加
	m_pOwnerObj->GetOwnerScene()->AddObjectCollision(this);

	m_pOwnerTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：終了処理
=========================================== */
ComponentCollisionBase::~ComponentCollisionBase()
{
	// 所持オブジェクトの所属シーンの衝突判定管理リストから削除
	m_pOwnerObj->GetOwnerScene()->RemoveObjectCollision(this);
}



/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentCollisionBase::Update()
{
	// 所持オブジェクトのトランスフォームが存在しない場合は処理しない
	if (!m_pOwnerTransform) return;

	// 所有オブジェクトの座標＋調整座標
	m_vPosition = m_pOwnerTransform->GetWorldPosition() + m_vPositionOffset;

	// 所持オブジェクトのトランスフォームを参照する場合
	if (m_bRefOwnerTransform)
	{
		// 回転、スケールを親オブジェクトから取得
		m_qRotation = m_pOwnerTransform->GetWorldRotation();
		m_vScale = m_pOwnerTransform->GetWorldScale();
	}
#ifdef _DEBUG
	if (CHECK_DISP_COMP(m_sColCompName.c_str()))
	{
		// 衝突オブジェクトリストの追加、削除
		if (m_pColObjList != nullptr)
		{
			for (auto& colObj : m_bColStatesMap)
			{
				if (colObj.second == true)
					m_pColObjList->AddListItem(colObj.first->GetName().c_str());
				else
					m_pColObjList->RemoveListItem(colObj.first->GetName().c_str());
			}
		}
	}
	// 全オブジェクト当たり判定線表示フラグ更新
	m_bIsDispColAll = WIN_BASIC_SETTING["DispCollision"].GetBool();
#endif // _DEBUG
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ComponentCollisionBase::Draw()
{
	if (!m_bIsDispColAll) return;	// 全コリジョン表示フラグがtrueの場合は処理しない

	if (!m_bIsDispCol) return;	// コリジョン表示フラグがfalseの場合は処理しない

	// デバッグ用図形をセットしている場合
	if (m_pShape)
	{
		m_pShape->Draw();
	}
}

/* ========================================
	衝突判定更新関数
	-------------------------------------
	内容：衝突判定の更新処理
	-------------------------------------
	引数1：衝突相手の当たり判定コンポーネント
=========================================== */
void ComponentCollisionBase::UpdateCollision(ComponentCollisionBase* otherCol)
{
	ObjectBase* pOtherObj = otherCol->GetOwnerObject();	// 相手のオブジェクトを取得

	if (pOtherObj->GetState() == ObjectBase::STATE_DEAD) return;	// 相手のオブジェクトが死亡状態なら処理しない
	if (!m_bIsEnabled) return;										// このコリジョンが無効なら処理しない
	if (!otherCol->GetEnable()) return;								// 相手のコリジョンが無効なら処理しない
	if (!m_bIsDynamic && !otherCol->GetDynamic()) return;			// 両方のコリジョンが静的なら処理しない

	// 衝突オブジェクトが一覧にない場合、追加
	if (m_bColStatesMap.find(pOtherObj) == m_bColStatesMap.end())
	{
		m_bColStatesMap.emplace(pOtherObj, false);	// 衝突状態をfalseに設定(衝突していない)
	}

	bool bColPrev = m_bColStatesMap.at(pOtherObj);	// 衝突状態(前フレーム)
	bool bColCurrent = CheckCollision(otherCol);	// 衝突判定(現在のフレーム)
	
	// 衝突開始
	// 前フレームが衝突していなく、現在のフレームが衝突している場合
	if (!bColPrev && bColCurrent)
	{
		m_pOwnerObj->OnCollisionEnter(pOtherObj);
	}
	// 衝突中
	// 前フレームも現在のフレームも衝突している場合
	else if (bColPrev && bColCurrent)
	{
		m_pOwnerObj->OnCollisionStay(pOtherObj);
	}
	// 衝突終了
	// 前フレームが衝突していて、現在のフレームが衝突していない場合
	else if (bColPrev && !bColCurrent)
	{
		m_pOwnerObj->OnCollisionExit(pOtherObj);
	}

	// 衝突状態を更新
	m_bColStatesMap.at(pOtherObj) = bColCurrent;

}

/* ========================================
	衝突状態マップ更新関数
	-------------------------------------
	内容：衝突状態マップの更新処理
		　衝突判定の結果、相手のオブジェクトが変わる事があるので、
		  衝突判定更新後に呼び出して、マップを更新する
	-------------------------------------
	引数1：衝突相手の当たり判定コンポーネント
=========================================== */
void ComponentCollisionBase::UpdateCollisionMap(ComponentCollisionBase* otherCol)
{
	ObjectBase* pOtherObj = otherCol->GetOwnerObject();	// 相手のオブジェクトを取得

	// 死亡状態の場合はマップから削除
	if (pOtherObj->GetState() == ObjectBase::STATE_DEAD)
	{
		m_bColStatesMap.erase(otherCol->GetOwnerObject());

#ifdef _DEBUG
		// デバッグ衝突オブジェクトリストから削除
		if (CHECK_DISP_COMP(m_sColCompName.c_str()))
		{
			if (!m_pColObjList)
				WIN_OBJ_INFO["CollObjectList"].RemoveListItem(pOtherObj->GetName().c_str());
		}
#endif // _DEBUG

		return;
	}
}

/* ========================================
	AABB・球衝突判定関数
	-------------------------------------
	内容：AABBと球が衝突しているかを確認する
	-------------------------------------
	引数1：オブジェクト球コリジョン
	引数2：オブジェクトAABBコリジョン
	-------------------------------------
	戻値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionBase::CheckCollisionAABBToSphere(ComponentCollisionAABB* colAABB, ComponentCollisionSphere* colSphere)
{
	Vector3<float> vClosestPoint;	// AABB上で球に最も近い点

	// 各軸での最も近い点を求める
	vClosestPoint.x = std::clamp(colSphere->GetPosition().x, colAABB->GetMin().x,colAABB->GetMax().x);
	vClosestPoint.y = std::clamp(colSphere->GetPosition().y, colAABB->GetMin().y,colAABB->GetMax().y);
	vClosestPoint.z = std::clamp(colSphere->GetPosition().z, colAABB->GetMin().z,colAABB->GetMax().z);

	// 最も近い点と球の中心との距離を求める(計算量を減らすために2乗距離で比較)
	float fDistance = (vClosestPoint - colSphere->GetPosition()).LengthSq();

	// 球の半径よりも距離が近い場合は衝突している
	return fDistance < (colSphere->GetRadius() * colSphere->GetRadius());
}

/* ========================================
	AABB・OBBの衝突判定関数
	-------------------------------------
	内容：AABBとOBBが衝突しているかを確認する
	-------------------------------------
	引数1：AABBコリジョン
	引数2：OBBコリジョン
	-------------------------------------
	戻り値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionBase::CheckCollisionAABBToOBB(ComponentCollisionAABB* colAABB, ComponentCollisionOBB* colOBB)
{
	// AABBを親クラスにキャスト
	ComponentCollisionBase* m_pAABBAsOBB = static_cast<ComponentCollisionBase*>(colAABB);

	// AABBをOBBとして扱うために値を設定
	m_pAABBAsOBB->SetPosition(colAABB->GetPosition());				// 座標はAABBの座標
	m_pAABBAsOBB->SetScale(colAABB->GetMax() - colAABB->GetMin());	// 大きさはAABBの最大座標と最小座標の差
	m_pAABBAsOBB->SetRotation(Quaternion());						// 回転はなし
	
	return colOBB->CheckCollisionOBB(m_pAABBAsOBB);
}

/* ========================================
	OBB・球衝突判定関数
	-------------------------------------
	内容：OBBと球が衝突しているかを確認する
	-------------------------------------
	引数1：OBBコリジョン
	引数2：球コリジョン
	引数3：最小移動ベクトル設定フラグ
		 　※true=最小移動ベクトルを設定する(OBB側から呼ばれた時)
	-------------------------------------
	戻り値：true=衝突している/false=衝突していない
=========================================== */
bool ComponentCollisionBase::CheckCollisionOBBToSphere(ComponentCollisionOBB* colOBB, ComponentCollisionSphere* colSphere, bool bSetMtv)
{
	using T_MTV		= ComponentCollisionOBB::T_MTV;				// 名称省略
	T_MTV tMtv		= T_MTV();									// MTV構造体

	bool bResult	= false;	// 衝突結果

	// OBBの構造体作成
	ComponentCollisionOBB::T_OBB tObb	= colOBB->CreateOBB(colOBB);						
	// 球の中心からOBBの中心までのベクトルを計算
	Vector3<float> vDis					= colSphere->GetPosition() - colOBB->GetPosition();	
	// OBB上の最も近い点を求める
	Vector3<float> vClosestOBB			= colOBB->GetPosition();	// OBBの中心を初期値とする


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

	Vector3<float> vSphCenToCloOBB = colSphere->GetPosition() - vClosestOBB;	// 球の中心からOBB上の最も近い点までのベクトル
	// 最も近い点と球の中心との距離を求める(計算量を減らすために2乗距離で比較)
	float fDistanceSq = vSphCenToCloOBB.LengthSq();
	float fRadiusSq = colSphere->GetRadius() * colSphere->GetRadius();	// 球の半径の2乗

	// 球の半径よりも距離が近い場合は衝突している
	if(fDistanceSq < fRadiusSq)
	{
		// 衝突している場合、最小移動ベクトルを設定
		tMtv.vAxis		= vSphCenToCloOBB.GetNormalize();					// 衝突軸(球の中心からOBB上の最も近い点へのベクトル)の反対方向
		tMtv.fOverlap	= colSphere->GetRadius() - std::sqrt(fDistanceSq);	// 重なり量
		tMtv.bIsCol		= true;												// 衝突結果

		bResult			= true;
	}
	else
	{
		tMtv.bIsCol		= false;
		bResult			= false;
	}

	// 最小移動ベクトルを設定する場合
	if (bSetMtv)
	{
		std::vector<T_MTV> mtvs = colOBB->GetMtvs();	// MTV構造体リストを取得
		// 同じ名前のMTVを更新
		for (auto& mtv : mtvs)
		{
			if (mtv.sName == colSphere->GetOwnerObject()->GetName())
			{
				mtv.bIsCol		= tMtv.bIsCol;
				mtv.vAxis		= tMtv.vAxis;
				mtv.fOverlap	= tMtv.fOverlap;
				mtv.bIsTrigger = colSphere->GetTrigger();
				break;
			}
		}
		colOBB->SetMtvs(mtvs);							// OBBのMTV構造体リストを更新
	}

	return bResult;
	
}


/* ========================================
	ゲッター(座標)関数
	-------------------------------------
	戻値：座標
=========================================== */
Vector3<float> ComponentCollisionBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	ゲッター(座標オフセット)関数
	-------------------------------------
	戻値：座標オフセット
=========================================== */
Vector3<float> ComponentCollisionBase::GetPositionOffset()
{
	return m_vPositionOffset;
}

/* ========================================
	ゲッター(スケール)関数
	-------------------------------------
	戻値：スケール
=========================================== */
Vector3<float> ComponentCollisionBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	ゲッター(回転)関数
	-------------------------------------
	戻値：回転
=========================================== */
Quaternion ComponentCollisionBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	ゲッター(親トランスフォーム参照フラグ)関数
	-------------------------------------
	戻値：トランスフォーム参照フラグ
=========================================== */
bool ComponentCollisionBase::GetRefOwnerTransform()
{
	return m_bRefOwnerTransform;
}

/* ========================================
	ゲッター(コリジョン種類)関数
	-------------------------------------
	戻値：コリジョン種類
=========================================== */
ComponentCollisionBase::E_ColType ComponentCollisionBase::GetColType()
{
	return m_eColType;
}

/* ========================================
	ゲッター(動的コリジョンフラグ)関数
	-------------------------------------
	戻値：動的コリジョンフラグ
=========================================== */
bool ComponentCollisionBase::GetDynamic()
{
	return m_bIsDynamic;
}

/* ========================================
	ゲッター(有効フラグ)関数
	-------------------------------------
	戻値：有効フラグ
=========================================== */
bool ComponentCollisionBase::GetEnable()
{
	return m_bIsEnabled;
}

/* ========================================
	ゲッター(トリガーフラグ)関数
	-------------------------------------
	戻値：トリガーフラグ
=========================================== */
bool ComponentCollisionBase::GetTrigger()
{
	return m_bIsTrigger;
}

/* ========================================
	セッター(座標)関数
	-------------------------------------
	引数1：座標
=========================================== */
void ComponentCollisionBase::SetPosition(Vector3<float> vPos)
{
	m_vPosition = vPos;
}

/* ========================================
	セッター(座標オフセット)関数
	-------------------------------------
	引数1：座標オフセット
=========================================== */
void ComponentCollisionBase::SetPositionOffset(Vector3<float> vOffset)
{
	m_vPositionOffset = vOffset;
}

/* ========================================
	セッター(スケール)関数
	-------------------------------------
	引数1：スケール
=========================================== */
void ComponentCollisionBase::SetScale(Vector3<float> vScale)
{
	m_vScale = vScale;
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数1：回転
=========================================== */
void ComponentCollisionBase::SetRotation(Quaternion qRot)
{
	m_qRotation = qRot;
}

/* ========================================
	セッター(親トランスフォーム参照フラグ)関数
	-------------------------------------
	引数1：トランスフォーム参照フラグ
=========================================== */
void ComponentCollisionBase::SetRefOwnerTransform(bool bRef)
{
	m_bRefOwnerTransform = bRef;
}

/* ========================================
	セッター(コリジョン種類)関数
	-------------------------------------
	引数1：コリジョン種類
=========================================== */
void ComponentCollisionBase::SetColType(E_ColType eType)
{
	m_eColType = eType;
}

/* ========================================
	セッター(動的コリジョンフラグ)関数
	-------------------------------------
	引数1：動的コリジョンフラグ
=========================================== */
void ComponentCollisionBase::SetDynamic(bool bDynamic)
{
	m_bIsDynamic = bDynamic;
}

/* ========================================
	セッター(有効フラグ)関数
	-------------------------------------
	引数1：有効フラグ
=========================================== */
void ComponentCollisionBase::SetEnable(bool bEnable)
{
	m_bIsEnabled = bEnable;
}

/* ========================================
	セッター(トリガーフラグ)関数
	-------------------------------------
	引数1：トリガーフラグ
=========================================== */
void ComponentCollisionBase::SetTrigger(bool bTrigger)
{
	m_bIsTrigger = bTrigger;
}


#ifdef _DEBUG
/* ========================================
	当たり判定基底デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：グループ項目
	引数2：コンポーネント名(デバッグメニュー表示確認用)
=========================================== */
void ComponentCollisionBase::DebugColBase(DebugUI::Item* pGroupItem, std::string sCompName)
{
	using namespace DebugUI;

	m_sColCompName = sCompName;

	// トランスフォーム------------------------
	// 所持オブジェクトトランスフォーム参照
	pGroupItem->AddGroupItem(Item::CreateBind("IsOwnerRefTransform", Item::Kind::Bool, &m_bRefOwnerTransform));
	// 座標オフセット
	pGroupItem->AddGroupItem(Item::CreateBind("PosOffset", Item::Kind::Vector, &m_vPositionOffset));
	// 回転
	// 表示だけオイラー角に変換(クォータニオンは直接入力できないため)
	pGroupItem->AddGroupItem(Item::CreateCallBack("Rotation  ", Item::Kind::Vector,
		[this](bool isWrite, void* arg)	// isWrite:入力があるかどうか arg:入力値
	{
		// 入力値、表示値はデグリー、内部はラジアンで扱う
		Vector3<float>* pDispDeg = static_cast<Vector3<float>*>(arg);

		// 値変更時
		if (isWrite)
		{
			// クォータニオンに変換し、変数に格納
			m_qRotation = Quaternion::FromEulerAngle(
				Vector3<float>(
					MathUtils::ToRadian(pDispDeg->x),
					MathUtils::ToRadian(pDispDeg->y),
					MathUtils::ToRadian(pDispDeg->z)
				));
		}
		else
		{
			// クォータニオンをオイラー角に変換し、表示
			*pDispDeg = Vector3<float>(
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().x),
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().y),
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().z)
			);
		}
	}));
	// 大きさ
	pGroupItem->AddGroupItem(Item::CreateBind("Scale  ", Item::Kind::Vector, &m_vScale));

	// --------------------------------------

	pGroupItem->AddGroupItem(Item::CreateBind("IsDispCollision",Item::Kind::Bool, &m_bIsDispCol));	// コリジョン表示フラグ
	pGroupItem->AddGroupItem(Item::CreateBind("IsDynamic",		Item::Kind::Bool, &m_bIsDynamic));	// 動的コリジョンフラグ
	pGroupItem->AddGroupItem(Item::CreateBind("IsEnabled",		Item::Kind::Bool, &m_bIsEnabled));	// 有効フラグ
	pGroupItem->AddGroupItem(Item::CreateBind("IsTrigger",		Item::Kind::Bool, &m_bIsTrigger));	// トリガーフラグ

	m_pColObjList = Item::CreateList("CollObjectList", nullptr);	// 衝突オブジェクトリスト
	m_pColObjList->AddListItem("None");	// デフォルトでNoneを追加(空の場合表示されないため
	pGroupItem->AddGroupItem(m_pColObjList);

}
#endif // _DEBUG
