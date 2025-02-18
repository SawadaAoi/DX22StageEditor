/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(ステージギミック直線移動)用cpp
	------------------------------------
	ComponentGimmickMoveLinear.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentGimmickMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include <format>
#include "ShapeLine.h"
#include "ColorVec3.h"

// =============== 定数定義 =======================
const float LIMIT_DISTANCE_SQ = 0.01f * 0.01f;	// 移動先に到達する距離の2乗

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有者オブジェクト
========================================== */
ComponentGimmickMoveLinear::ComponentGimmickMoveLinear(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderGimmick)
	, m_pCompTransform(nullptr)
	//, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(0.1f)
	, m_vtWayPoints()
	, m_nCurrentWayPoint(0)
	, m_bIsReverse(false)
#ifdef _DEBUG
	, m_bDispMoveLine(true)
	, m_nSelectWayPointIdx(0)
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
========================================= */
void ComponentGimmickMoveLinear::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pMoveLine = std::make_unique<ShapeLine>(10);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ComponentGimmickMoveLinear::Update()
{
	// 移動座標が1つ以下の場合は処理を行わない
	if (m_vtWayPoints.size() <= 1)
	{
		return;
	}

	// 逆順フラグが立っている場合は逆順に移動
	if (m_bIsReverse)
		ReverseMove();
	else
		Move();


	if (m_bDispMoveLine)
	{	// 移動ラインの更新
		for (int i = 0; i < m_vtWayPoints.size(); i++)
		{
			int nNextIndex = (i + 1) % m_vtWayPoints.size();	// 最後の座標の場合は最初の座標に戻る
			Vector3<float> vStart = m_vtWayPoints[i];
			Vector3<float> vEnd = m_vtWayPoints[nNextIndex];
			m_pMoveLine->UpdateLine(i + 1, vStart, vEnd, ColorVec3::BLUE);
		}
	}

}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================= */
void ComponentGimmickMoveLinear::Draw()
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
========================================= */
void ComponentGimmickMoveLinear::Move()
{
	// 現在の座標番号の座標を取得
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// 移動先とのベクトルを計算
	Vector3<float> vDir = vCurrentWayPoint - m_pCompTransform->GetPosition();

	// 移動先に到達している場合
	if (vDir.LengthSq() < LIMIT_DISTANCE_SQ)
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
		m_pCompTransform->Translate(vDir.GetNormalize() * m_fMoveSpeed * DELTA_TIME);
	}

}

/* ========================================
	逆移動関数
	-------------------------------------
	内容：逆順に移動処理
========================================= */
void ComponentGimmickMoveLinear::ReverseMove()
{
	// 現在の座標番号の座標を取得
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// 移動先とのベクトルを計算
	Vector3<float> vDir = vCurrentWayPoint - m_pCompTransform->GetPosition();

	// 移動先に到達している場合
	if (vDir.LengthSq() < LIMIT_DISTANCE_SQ)
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
		m_pCompTransform->Translate(vDir.GetNormalize() * m_fMoveSpeed * DELTA_TIME);

	}

	// 移動先の座標を向く(高さは考慮しない)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}


/* ========================================
	移動座標追加関数
	-------------------------------------
	内容：移動座標を追加
	-------------------------------------
	引数1：移動座標
========================================= */
void ComponentGimmickMoveLinear::AddWayPoint(const Vector3<float>& vWayPoint)
{
	m_vtWayPoints.push_back(vWayPoint);

	// 移動座標が1つの場合は座標を設定
	if (m_vtWayPoints.size() == 1)
	{
		m_pCompTransform->SetPosition(vWayPoint);
	}

	m_nCurrentWayPoint = 0;

#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// 移動座標リストの更新
		InitDebugWayPointList();
		m_nSelectWayPointIdx = m_nCurrentWayPoint;
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
========================================= */
void ComponentGimmickMoveLinear::InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex)
{
	// インデックスが範囲外の場合は追加しない
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.insert(m_vtWayPoints.begin() + nIndex, vWayPoint);
#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// 移動座標リストの更新
		InitDebugWayPointList();
	}
#endif // _DEBUG
}


/* ========================================
	移動座標削除関数
	-------------------------------------
	内容：指定したインデックスの移動座標を削除
	-------------------------------------
	引数1：インデックス
========================================= */
void ComponentGimmickMoveLinear::RemoveWayPoint(int nIndex)
{
	// 移動ラインの更新
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		m_pMoveLine->UpdateLine(i + 1, Vector3<float>::Zero(), Vector3<float>::Zero());
	}

	// インデックスが範囲外の場合は削除しない
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.erase(m_vtWayPoints.begin() + nIndex);

	// インデックスが最後の座標の場合は最後の座標に戻す
	if (m_nCurrentWayPoint >= m_vtWayPoints.size())
	{
		m_nCurrentWayPoint = m_vtWayPoints.size() - 1;
	}

#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// 移動座標リストの更新
		InitDebugWayPointList();
		m_nSelectWayPointIdx = m_nCurrentWayPoint;
	}
#endif // _DEBUG
}


/* ========================================
	ゲッター(移動速度)関数
	-------------------------------------
	戻値：移動速度
=========================================== */
float ComponentGimmickMoveLinear::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	ゲッター(移動座標配列)関数
	-------------------------------------
	戻値：std::vector<Vector3<float>>& 移動座標配列
=========================================== */
std::vector<Vector3<float>>& ComponentGimmickMoveLinear::GetWayPoints()
{
	return m_vtWayPoints;
}

/* ========================================
	ゲッター(逆順フラグ)関数
	-------------------------------------
	戻値：bool	逆順フラグ
=========================================== */
bool ComponentGimmickMoveLinear::GetIsReverse()
{
	return m_bIsReverse;
}

/* ========================================
	セッター(移動速度)関数
	-------------------------------------
	引数1：float	移動速度
=========================================== */
void ComponentGimmickMoveLinear::SetMoveSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}

/* ========================================
	セッター(逆順フラグ)関数
	-------------------------------------
	引数1：bool	逆順フラグ
=========================================== */
void ComponentGimmickMoveLinear::SetIsReverse(bool bIsReverse)
{
	m_bIsReverse = bIsReverse;
}



#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentGimmickMoveLinear::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupMoveLinear = Item::CreateGroup("GimmickMoveLinear");

	pGroupMoveLinear->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	pGroupMoveLinear->AddGroupItem(Item::CreateBind("Reverse", Item::Kind::Bool, &m_bIsReverse));
	pGroupMoveLinear->AddGroupItem(Item::CreateBind("DispMoveLine", Item::Kind::Bool, &m_bDispMoveLine, false, true));
	pGroupMoveLinear->AddGroupItem(Item::CreateValue("WayPointNum", Item::Kind::Text));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("CurrentWayPoint", Item::Kind::Int,		// 現在の座標番号
		[this](bool isWrite, void* arg) { FuncWayCurrent(isWrite, arg); }));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("AddWayPoint", Item::Kind::Command,		// 移動座標追加
		[this](bool isWrite, void* arg) { AddWayPoint(m_pCompTransform->GetPosition()); }));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("RemoveWayPoint", Item::Kind::Command, 	// 移動座標削除
		[this](bool isWrite, void* arg) { RemoveWayPoint(m_nSelectWayPointIdx); }, false, true));

	// 移動座標
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("WayPoint", Item::Kind::Vector,
		[this](bool isWrite, void* arg) { FuncWayPoint(isWrite, arg); }));

	// 移動座標リスト
	pGroupMoveLinear->AddGroupItem(Item::CreateList("WayPointList", [this](const void* arg)
	{
		std::string sName = reinterpret_cast<const char*>(arg);
		m_nSelectWayPointIdx = WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].GetListNo(sName.c_str());

	}));

	window.AddItem(pGroupMoveLinear);
	InitDebugWayPointList();

	m_nSelectWayPointIdx = 0;
}

/* ========================================
	移動座標リスト初期化関数
	-------------------------------------
	内容：デバッグ用の移動座標リストを初期化
=========================================== */
void ComponentGimmickMoveLinear::InitDebugWayPointList()
{
	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].RemoveListItemAll();
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		// 座標を文字列に変換
		std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
		std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
		std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

		// リストに追加
		WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
	}

	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].SetListNo(m_nSelectWayPointIdx);
}

/* ========================================
	デバッグ項目クリック(座標番号)関数
	-------------------------------------
	内容：デバッグ用の座標番号をクリックした時の処理
	-------------------------------------
	引数1：書き込みフラグ
	引数2：引数
=========================================== */
void ComponentGimmickMoveLinear::FuncWayCurrent(bool isWrite, void* arg)
{
	int nWayPoint = *reinterpret_cast<int*>(arg);	// 引数の座標番号を取得
	if (isWrite)
	{
		m_nCurrentWayPoint = nWayPoint;
		// 範囲外の場合は範囲内に収める
		if (m_nCurrentWayPoint >= m_vtWayPoints.size())
			m_nCurrentWayPoint = m_vtWayPoints.size() - 1;
		else if (m_nCurrentWayPoint < 0)
			m_nCurrentWayPoint = 0;
	}
	else
	{
		*reinterpret_cast<int*>(arg) = m_nCurrentWayPoint;
	}
}

/* ========================================
	デバッグ項目クリック(座標)関数
	-------------------------------------
	内容：デバッグ用の座標をクリックした時の処理
	-------------------------------------
	引数1：書き込みフラグ
	引数2：引数
=========================================== */
void ComponentGimmickMoveLinear::FuncWayPoint(bool isWrite, void* arg)
{
	if (m_vtWayPoints.empty()) return;
	Vector3<float>* pvPos = reinterpret_cast<Vector3<float>*>(arg);

	if (isWrite)
	{
		m_vtWayPoints[m_nSelectWayPointIdx] = *pvPos;	// 変更した座標を設定
		InitDebugWayPointList();						// 移動座標リストの更新

	}
	else
	{
		*pvPos = m_vtWayPoints[m_nSelectWayPointIdx];
	}
}

#endif // _DEBUG
