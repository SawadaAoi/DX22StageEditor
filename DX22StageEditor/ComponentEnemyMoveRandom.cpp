/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(ランダム)用cpp
	------------------------------------
	ComponentEnemyMoveRandom.cpp
========================================== */

// =============== インクルード =====================
#include "ColorVec3.h"
#include "ComponentEnemyMoveRandom.h"
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "MathUtils.h"
#include "ObjectBase.h"
#include "ShapeLine.h"
#include "ShapeSphere.h"
#include <format>
#include "ColorVec3.h"

// =============== 定数定義 =======================
const float DEFAULT_TARGET_SWITCH_TIME = 2.0f;			// 目的座標切り替え時間
const float DEFAULT_MOVE_RECT_SIZE = 5.0f;			// 移動範囲のサイズ
const float LIMIT_DISTANCE_SQ = 0.5f * 0.5f;	// 移動先に到達する距離の2乗
const int	LIMIT_RECT_POINT_NUM = 4;			// 移動範囲の頂点数

// 目的座標描画
const Vector3<float> TARGET_COLOR = ColorVec3::BLUE;
const Vector3<float> TARGET_SPHERE_SCALE = { 0.2f, 0.2f, 0.2f };

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有者オブジェクト
========================================== */
ComponentEnemyMoveRandom::ComponentEnemyMoveRandom(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_vTargetPos()
	, m_fTargetSwitchTime(DEFAULT_TARGET_SWITCH_TIME)
	, m_fTargetSwitchCnt(0.0f)
#ifdef _DEBUG
	, m_bDispMoveLimitRect(true)
	, m_bMoveStop(false)
#else
	, m_bDispMoveLimitRect(false)
#endif // _DEBUG
	, m_vRectCenter()
	, m_vtMoveLimitRectLocal()
	, m_vtMoveLimitRectWorld()
	, m_pRectLine(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
======================================= */
void ComponentEnemyMoveRandom::Init()
{
	ComponentEnemyMoveBase::Init();

	// 移動範囲の初期化
	SetMoveLimitRectSquare(DEFAULT_MOVE_RECT_SIZE);

	// 移動範囲描画用のラインを生成
	m_pRectLine = std::make_unique<ShapeLine>(4);

	// 目的座標描画用の球を生成
	m_pTargetSphere = std::make_unique<ShapeSphere>();
	m_pTargetSphere->SetScale(TARGET_SPHERE_SCALE);
	m_pTargetSphere->SetColor(TARGET_COLOR);
	m_pTargetSphere->SetDrawMode(ShapeSphere::E_DrawMode::DRAW_NORMAL);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
======================================= */
void ComponentEnemyMoveRandom::Update()
{
#ifdef _DEBUG
	if (m_bMoveStop) return;
#endif // _DEBUG

	// 目的座標切り替え時間をカウント
	m_fTargetSwitchCnt += DELTA_TIME;

	// 目的座標切り替え時間になったら目的座標を変更
	if (m_fTargetSwitchCnt >= m_fTargetSwitchTime)
	{
		SwitchTargetPos();
	}

	// 移動処理
	Move();

	// 移動範囲座標の更新
	UpdateMoveLimitRectWorldPos();

}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
======================================= */
void ComponentEnemyMoveRandom::Draw()
{
	if (m_bDispMoveLimitRect)
	{
		m_pRectLine->Draw();
		m_pTargetSphere->Draw();
	}
}

/* ========================================
	目的座標初期化関数
	-------------------------------------
	内容：目的座標を初期化
======================================= */
void ComponentEnemyMoveRandom::InitTargetPos()
{
	// 移動範囲座標(ワールド)の更新
	UpdateMoveLimitRectWorldPos();

	// 初期目的座標を設定
	m_vTargetPos = GetRandomPoint(
		m_vtMoveLimitRectWorld[0], m_vtMoveLimitRectWorld[1], m_vtMoveLimitRectWorld[2], m_vtMoveLimitRectWorld[3]);

	// 目的座標描画用の球座標を設定
	m_pTargetSphere->SetPosition(m_vTargetPos);
}

/* ========================================
	移動関数
	-------------------------------------
	内容：移動処理
======================================= */
void ComponentEnemyMoveRandom::Move()
{
	// 目的座標に向かって移動
	Vector3<float> vDistance = m_vTargetPos - m_pCompTransform->GetPosition();

	// 目的座標に到達している場合は目的座標を変更
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		SwitchTargetPos();
	}

	// 移動先に向かって移動
	Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
	vVelocity.x = vDistance.GetNormalize().x * m_fMoveSpeed;
	vVelocity.z = vDistance.GetNormalize().z * m_fMoveSpeed;
	m_pCompRigidbody->SetVelocity(vVelocity);

	// 移動先の座標を向く(高さは考慮しない)
	Vector3<float> vLook = m_vTargetPos;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	目的座標切り替え関数
	-------------------------------------
	内容：目的座標を切り替える
======================================= */
void ComponentEnemyMoveRandom::SwitchTargetPos()
{
	m_fTargetSwitchCnt = 0.0f;

	// 移動範囲内のランダムな座標を目的座標に設定
	m_vTargetPos = GetRandomPoint(
		m_vtMoveLimitRectWorld[0], m_vtMoveLimitRectWorld[1], m_vtMoveLimitRectWorld[2], m_vtMoveLimitRectWorld[3]);

	m_pTargetSphere->SetPosition(m_vTargetPos);

#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveRandom"))
	{
		WIN_OBJ_INFO["EnemyMoveRandom"]["TargetPos"].SetText(
			std::format("X:{:.1f} Y:{:.1f} Z:{:.1f}", m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z).c_str());
	}
#endif // _DEBUG
}

/* ========================================
	ランダム座標取得関数
	-------------------------------------
	内容：四角形内のランダムな座標を取得
	-------------------------------------
	引数1：四角形の左上座標
	引数2：四角形の右上座標
	引数3：四角形の右下座標
	引数4：四角形の左下座標
	-------------------------------------
	戻り値：四角形内のランダムな座標
======================================= */
Vector3<float> ComponentEnemyMoveRandom::GetRandomPoint(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const Vector3<float>& v4)
{
	float areaABC = GetTriangleArea(v1, v2, v3);
	float areaCDA = GetTriangleArea(v3, v4, v1);

	// 三角形ABCと三角形CDAの面積比を計算し、ランダムな座標を取得
	// ※面積が大きい3角形の方が選ばれやすいようにする
	float randomValue = MathUtils::Random(0.0f, 1.0f);
	if (randomValue < areaABC / (areaABC + areaCDA)) {
		return GetRandomPointInTriangle(v1, v2, v3);
	}
	else {
		return GetRandomPointInTriangle(v3, v4, v1);
	}
}

/* ========================================
	ランダム座標取得(三角形)関数
	-------------------------------------
	内容：三角形内のランダムな座標を取得
	-------------------------------------
	引数1：三角形の頂点1
	引数2：三角形の頂点2
	引数3：三角形の頂点3
	-------------------------------------
	戻り値：三角形内のランダムな座標
======================================= */
Vector3<float> ComponentEnemyMoveRandom::GetRandomPointInTriangle(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3)
{
	float u = MathUtils::Random(0.0f, 1.0f);
	float v = MathUtils::Random(0.0f, 1.0f);

	// u + v <= 1 となるよう調整
	if (u + v > 1.0f) {
		u = 1.0f - u;
		v = 1.0f - v;
	}

	// バリセントリック座標を計算
	Vector3<float> P;
	P.x = v1.x + u * (v2.x - v1.x) + v * (v3.x - v1.x);
	P.y = v1.y + u * (v2.y - v1.y) + v * (v3.y - v1.y);
	P.z = v1.z + u * (v2.z - v1.z) + v * (v3.z - v1.z);

	return P;
}

/* ========================================
	三角形面積取得関数
	-------------------------------------
	内容：三角形の面積を取得
	-------------------------------------
	引数1：三角形の頂点1
	引数2：三角形の頂点2
	引数3：三角形の頂点3
	-------------------------------------
	戻り値：三角形の面積
======================================= */
float ComponentEnemyMoveRandom::GetTriangleArea(const Vector3<float>& P1, const Vector3<float>& P2, const Vector3<float>& P3)
{
	// 行列式を利用した公式
	return 0.5f *
		std::abs(
			P1.x * (P2.z - P3.z) +
			P2.x * (P3.z - P1.z) +
			P3.x * (P1.z - P2.z)
		);
}

/* ========================================
	移動範囲座標(ワールド)更新関数
	-------------------------------------
	内容：移動範囲座標を更新
======================================= */
void ComponentEnemyMoveRandom::UpdateMoveLimitRectWorldPos()
{
	// 移動範囲座標(ワールド)を更新
	m_vtMoveLimitRectWorld.clear();
	for (int i = 0; i < m_vtMoveLimitRectLocal.size(); i++)
	{
		Vector3<float> vRectWorld = m_vtMoveLimitRectLocal[i] + m_vRectCenter;
		m_vtMoveLimitRectWorld.push_back(vRectWorld);
	}

	// 座標範囲線の更新
	for (int i = 0; i < LIMIT_RECT_POINT_NUM; i++)
	{
		int nNextIndex = (i + 1) % LIMIT_RECT_POINT_NUM;
		m_pRectLine->UpdateLine(i + 1, m_vtMoveLimitRectWorld[i], m_vtMoveLimitRectWorld[nNextIndex], ColorVec3::BLUE);
	}
}

/* ========================================
	ゲッター(目的座標)関数
	-------------------------------------
	戻値：Vector3<float>	目的座標
=========================================== */
Vector3<float> ComponentEnemyMoveRandom::GetTargetPos() const
{
	return m_vTargetPos;
}

/* ========================================
	ゲッター(目的座標切り替え時間)関数
	-------------------------------------
	戻値：float	目的座標切り替え時間
=========================================== */
float ComponentEnemyMoveRandom::GetTargetSwitchTime() const
{
	return m_fTargetSwitchTime;
}

/* ========================================
	ゲッター(移動範囲座標)関数
	-------------------------------------
	戻値：std::vector<Vector3<float>>	移動範囲座標
=========================================== */
std::vector<Vector3<float>>& ComponentEnemyMoveRandom::GetMoveLimitRect()
{
	return m_vtMoveLimitRectLocal;
}

/* ========================================
	ゲッター(移動範囲中心座標)関数
	-------------------------------------
	戻値：Vector3<float>	移動範囲中心座標
=========================================== */
Vector3<float> ComponentEnemyMoveRandom::GetMoveLimitRectCenter()
{
	return m_vRectCenter;
}

/* ========================================
	セッター(目的座標切り替え時間)関数
	-------------------------------------
	引数：float	目的座標切り替え時間
=========================================== */
void ComponentEnemyMoveRandom::SetTargetSwitchTime(float fTime)
{
	m_fTargetSwitchTime = fTime;
}

/* ========================================
	セッター(移動範囲座標(ローカル))関数
	-------------------------------------
	引数：std::vector<Vector3<float>>	移動範囲座標
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRect(const std::vector<Vector3<float>>& vtRect)
{
	m_vtMoveLimitRectLocal = vtRect;
}

/* ========================================
	移動範囲座標(ローカル)設定関数(正方形)関数
	-------------------------------------
	引数：float	一辺の長さ
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectSquare(float fSize)
{
	m_vtMoveLimitRectLocal.clear();

	float fHalfSize = fSize * 0.5f;

	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfSize, 0.0f, fHalfSize));	// 右上
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfSize, 0.0f, fHalfSize));	// 左上
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfSize, 0.0f, -fHalfSize));	// 左下
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfSize, 0.0f, -fHalfSize));	// 右下
}

/* ========================================
	移動範囲座標(ローカル)設定関数(XZ平面)関数
	-------------------------------------
	引数：Vector2<float>	XZ平面のサイズ
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectXZ(const Vector2<float>& vSize)
{
	m_vtMoveLimitRectLocal.clear();

	float fHalfX = vSize.x * 0.5f;
	float fHalfZ = vSize.y * 0.5f;

	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfX, 0.0f, fHalfZ));		// 右上
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfX, 0.0f, fHalfZ));		// 左上
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfX, 0.0f, -fHalfZ));	// 左下
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfX, 0.0f, -fHalfZ));		// 右下
}


/* ========================================
	移動範囲表示フラグ設定関数
	-------------------------------------
	引数：bool	表示フラグ
=========================================== */
void ComponentEnemyMoveRandom::SetDispMoveLimitRect(bool bDisp)
{
	m_bDispMoveLimitRect = bDisp;
}

/* ========================================
	移動範囲中心座標設定関数
	-------------------------------------
	引数：Vector3<float>	移動範囲中心座標
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectCenter(const Vector3<float>& vCenter)
{
	m_vRectCenter = vCenter;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveRandom::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* EnemyMoveRandom = Item::CreateGroup("EnemyMoveRandom");

	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveStop", Item::Kind::Bool, &m_bMoveStop));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	EnemyMoveRandom->AddGroupItem(Item::CreateValue("TargetPos", Item::Kind::Text));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("SwitchTime", Item::Kind::Float, &m_fTargetSwitchTime));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("SwitchCnt", Item::Kind::Float, &m_fTargetSwitchCnt));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("DispMoveLimitRect", Item::Kind::Bool, &m_bDispMoveLimitRect));

	EnemyMoveRandom->AddGroupItem(Item::CreateBind("RectCenter", Item::Kind::Vector, &m_vRectCenter));

	// 移動範囲再設定
	EnemyMoveRandom->AddGroupItem(Item::CreateCallBack("ResetRect", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		m_vRectCenter = m_pCompTransform->GetPosition();
		InitTargetPos();
	}));
	// 移動範囲リスト
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_1", Item::Kind::Vector, &m_vtMoveLimitRectLocal[0]));	// 右上
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_2", Item::Kind::Vector, &m_vtMoveLimitRectLocal[1]));	// 左上
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_3", Item::Kind::Vector, &m_vtMoveLimitRectLocal[2]));	// 左下
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_4", Item::Kind::Vector, &m_vtMoveLimitRectLocal[3]));	// 右下

	window.AddItem(EnemyMoveRandom);

	WIN_OBJ_INFO["EnemyMoveRandom"]["TargetPos"].SetText(
		std::format("X:{:.1f} Y:{:.1f} Z:{:.1f}", m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z).c_str());

}

#endif // _DEBUG