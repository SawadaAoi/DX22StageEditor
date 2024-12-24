/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(直線)用cpp
	------------------------------------
	ComponentEnemyMoveLinear.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentEnemyMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include <format>
#include "ShapeLine.h"

// =============== 定数定義 =======================
const float LIMIT_DISTANCE_SQ = 0.3f * 0.3f;	// 移動先に到達する距離の2乗

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有者オブジェクト
========================================== */
ComponentEnemyMoveLinear::ComponentEnemyMoveLinear(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_vtWayPoints()
	, m_nCurrentWayPoint(0)
	, m_bIsReverse(false)
#ifdef _DEBUG
	, m_bDispMoveLine(true)
#else
	, m_bDispMoveLine(false)
#endif // _DEBUG
	, m_pMoveLine(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================= */
void ComponentEnemyMoveLinear::Init()
{
	ComponentEnemyMoveBase::Init();	// 基底クラスの初期化処理を呼び出す

	m_pMoveLine = std::make_unique<ShapeLine>(10);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================= */
void ComponentEnemyMoveLinear::Update()
{
	// 移動座標がない場合は何もしない
	if (m_vtWayPoints.empty())
	{
		return;
	}

	// 逆順フラグが立っている場合は逆順に移動
	if (m_bIsReverse)
		ReverseMove();
	else
		Move();

	
	// 移動ラインの更新
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		int nNextIndex = (i + 1) % m_vtWayPoints.size();	// 最後の座標の場合は最初の座標に戻る
		m_pMoveLine->UpdateLine(i + 1, m_vtWayPoints[i], m_vtWayPoints[nNextIndex], Vector3<float>(0.0f, 0.0f, 1.0f));
	}

}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
======================================= */
void ComponentEnemyMoveLinear::Draw()
{
	if (m_bDispMoveLine)
	{
		m_pMoveLine->Draw();
	}
}

/* ========================================
	移動関数
	-------------------------------------
	内容：移動処理
======================================= */
void ComponentEnemyMoveLinear::Move()
{	
	// 現在の座標番号の座標を取得
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// 移動先との距離を計算
	Vector3<float> vDistance = vCurrentWayPoint - m_pCompTransform->GetWorldPosition();

	// 移動先に到達している場合
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		m_nCurrentWayPoint++;

		// 最後の座標に到達した場合
		if (m_nCurrentWayPoint >= m_vtWayPoints.size())
		{
			m_nCurrentWayPoint = 0;	// 最初の座標に戻す
		}
	}
	else
	{
		// 移動先に向かって移動
		m_pCompRigidbody->SetVelocity(vDistance.GetNormalize() * m_fMoveSpeed);

	}

	// 移動先の座標を向く(高さは考慮しない)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetWorldPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	逆移動関数
	-------------------------------------
	内容：逆順に移動処理
======================================= */
void ComponentEnemyMoveLinear::ReverseMove()
{
	// 現在の座標番号の座標を取得
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// 移動先との距離を計算
	Vector3<float> vDistance = vCurrentWayPoint - m_pCompTransform->GetWorldPosition();

	// 移動先に到達している場合
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		m_nCurrentWayPoint--;

		// 最初の座標に到達した場合
		if (m_nCurrentWayPoint < 0)
		{
			m_nCurrentWayPoint = m_vtWayPoints.size() - 1;	// 最後の座標に戻す
		}
	}
	else
	{
		// 移動先に向かって移動
		m_pCompRigidbody->SetVelocity(vDistance.GetNormalize() * m_fMoveSpeed);

	}

	// 移動先の座標を向く(高さは考慮しない)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetWorldPosition().y;
	m_pCompTransform->LookAt(vLook);
}


/* ========================================
	移動座標追加関数
	-------------------------------------
	内容：移動座標を追加
	-------------------------------------
	引数1：移動座標
======================================= */
void ComponentEnemyMoveLinear::AddWayPoint(const Vector3<float>& vWayPoint)
{
	m_vtWayPoints.push_back(vWayPoint);

	// 移動座標が1つの場合は座標を設定
	if (m_vtWayPoints.size() == 1)
	{
		m_pCompTransform->SetLocalPosition(vWayPoint);
	}

#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveLinear"))
	{
		// 移動座標リストの更新
		WIN_OBJ_INFO["EnemyMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
		WIN_OBJ_INFO["EnemyMoveLinear"]["WayPoints"].RemoveListItemAll();
		for (int i = 0; i < m_vtWayPoints.size(); i++)
		{// 座標を文字列に変換
			std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
			std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
			std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

			// リストに追加
			WIN_OBJ_INFO["EnemyMoveLinear"]["WayPoints"].AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
		}
	}
#endif // _DEBUG

}

/* ========================================
	移動座標挿入関数
	-------------------------------------
	内容：指定したインデックスに移動座標を追加
	-------------------------------------
	引数1：移動座標
	引数2：インデックス
======================================= */
void ComponentEnemyMoveLinear::InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex)
{
	// インデックスが範囲外の場合は追加しない
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.insert(m_vtWayPoints.begin() + nIndex, vWayPoint);
}




#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveLinear::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pEnemyMoveLinear = Item::CreateGroup("EnemyMoveLinear");

	pEnemyMoveLinear->AddGroupItem(Item::CreateValue("WayPointNum", Item::Kind::Text));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("CurrentWayPoint", Item::Kind::Int, &m_nCurrentWayPoint));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("Reverse", Item::Kind::Bool, &m_bIsReverse));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("DispMoveLine", Item::Kind::Bool, &m_bDispMoveLine));

	// 移動座標リスト
	Item* WayList = Item::CreateList("WayPoints", nullptr,false);

	// リストに追加
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		// 座標を文字列に変換
		std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
		std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
		std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

		// リストに追加
		WayList->AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
	}

	pEnemyMoveLinear->AddGroupItem(WayList);

	window.AddItem(pEnemyMoveLinear);

	WIN_OBJ_INFO["EnemyMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
}

#endif // _DEBUG
