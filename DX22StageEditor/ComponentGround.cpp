/* ========================================
	CatRobotGame/
	------------------------------------
	地面コンポーネント用cpp
	------------------------------------
	ComponentGround.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentGround.h"
#include "ComponentTransform.h"
#include "ShapeLine.h"
#include "ObjectBase.h"
#include "ColorVec3.h"

// =============== 定数定義 =======================
const Vector3<float> NORMAL_DIRERCTION = Vector3<float>(0.0f, 1.0f, 0.0f);	// 法線の向き
const int TRIANGLE_NUM = 2;	// 三角形の数

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentGround::ComponentGround(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderGround)
	, m_vNormalDirection(NORMAL_DIRERCTION)
	, m_vWorldNormalDirection(NORMAL_DIRERCTION)
	, m_bIsDispNormal(false)
	, m_pNormalLine(nullptr)
	, m_pCompTransform(nullptr)
{
	m_TriangleVertices.resize(TRIANGLE_NUM);

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ComponentGround::Init()
{
	// 所有オブジェクトの座標、回転、スケールを取得
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();	

	// 法線を表示するための線を初期化
	m_pNormalLine = std::make_unique<ShapeLine>(Vector3<float>::Zero(), NORMAL_DIRERCTION, ColorVec3::RED);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentGround::Update()
{
	UpdateTriangleVertex();		// 三角形の頂点情報更新

	m_vWorldNormalDirection = m_pCompTransform->GetRotation().Rotate(m_vNormalDirection);

	if (m_bIsDispNormal)
	{
		// 法線の位置、向き、スケールを更新
		m_pNormalLine->SetPos(m_pCompTransform->GetPosition());
		m_pNormalLine->SetRotation(m_pCompTransform->GetRotation());
	}
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ComponentGround::Draw()
{
	if (m_bIsDispNormal)
	{
		// 法線を描画
		m_pNormalLine->Draw();
	}
}


/* ========================================
	三角形頂点更新関数
	-------------------------------------
	内容：所有オブジェクトのスケール、座標に合わせて
		　三角形の頂点を更新
=========================================== */
void ComponentGround::UpdateTriangleVertex()
{
	Vector3<float> vScaleHalf	= m_pCompTransform->GetScale() / 2.0f;
	Vector3<float> vPos			= m_pCompTransform->GetPosition();

	// 三角形の頂点を設定 (Z軸は奥が正)
	// 1つ目の三角形 (左上、右上、左下)
	m_TriangleVertices[0].pos[0] = Vector3<float>(-vScaleHalf.x, 0.0f, vScaleHalf.z);   // 左上
	m_TriangleVertices[0].pos[1] = Vector3<float>(vScaleHalf.x, 0.0f, vScaleHalf.z);    // 右上
	m_TriangleVertices[0].pos[2] = Vector3<float>(-vScaleHalf.x, 0.0f, -vScaleHalf.z);  // 左下

	// 2つ目の三角形 (右上、右下、左下)
	m_TriangleVertices[1].pos[0] = Vector3<float>(vScaleHalf.x, 0.0f, vScaleHalf.z);    // 右上
	m_TriangleVertices[1].pos[1] = Vector3<float>(vScaleHalf.x, 0.0f, -vScaleHalf.z);   // 右下
	m_TriangleVertices[1].pos[2] = Vector3<float>(-vScaleHalf.x, 0.0f, -vScaleHalf.z);  // 左下

	// 親オブジェクトの回転に合わせて三角形の頂点を回転
	Quaternion rotation = m_pCompTransform->GetRotation();
	for (int i = 0; i < TRIANGLE_NUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// 1. 回転中心を原点に移動 (相対座標に変換)
			Vector3<float> relativePos = m_TriangleVertices[i].pos[j];

			// 2. 頂点を回転
			relativePos = rotation.Rotate(relativePos);

			// 3. 元の位置に戻す
			m_TriangleVertices[i].pos[j] = relativePos + vPos;
		}
	}

}

/* ========================================
	ゲッター(ワールド空間法線ベクトル)関数
	-------------------------------------
	戻値：Vector3<float>	法線ベクトル
=========================================== */
Vector3<float> ComponentGround::GetWorldNormalDirection()
{
	return  m_vWorldNormalDirection;
}

/* ========================================
	ゲッター(法線表示フラグ)関数
	-------------------------------------
	戻値：bool	法線表示フラグ
=========================================== */
bool ComponentGround::GetIsDispNormal()
{
	return m_bIsDispNormal;
}

/* ========================================
	ゲッター(三角形の頂点)関数
	-------------------------------------
	戻値：std::vector<T_TriangleVertex>	三角形の頂点
=========================================== */
std::vector<ComponentGround::T_TriangleVertex> ComponentGround::GetTriangleVertex()
{
	return m_TriangleVertices;
}


/* ========================================
	セッター(法線ベクトル)関数
	-------------------------------------
	引数：Vector3<float> vNormal	法線ベクトル
=========================================== */
void ComponentGround::SetNormalDirection(const Vector3<float>& vNormal)
{
	m_vNormalDirection = vNormal;
}

/* ========================================
	セッター(法線表示フラグ)関数
	-------------------------------------
	引数：bool bIsDispNormal	法線表示フラグ
=========================================== */
void ComponentGround::SetIsDispNormal(bool bIsDispNormal)
{
	m_bIsDispNormal = bIsDispNormal;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentGround::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGround = Item::CreateGroup("Ground");

	pGroupGround->AddGroupItem(Item::CreateBind("NormalDirection",	Item::Kind::Vector, &m_vNormalDirection));			// 法線方向
	pGroupGround->AddGroupItem(Item::CreateBind("WorldNormalDirection", Item::Kind::Vector, &m_vWorldNormalDirection));	// 法線方向(ワールド座標)
	pGroupGround->AddGroupItem(Item::CreateBind("IsDispNormal",		Item::Kind::Bool, &m_bIsDispNormal));				// 法線表示フラグ

	// 三角形の頂点
	for (int i = 0; i < TRIANGLE_NUM; i++)
	{
		std::string sTriangle = "Triangle" + std::to_string(i);
		pGroupGround->AddGroupItem(Item::CreateValue(sTriangle.c_str(), Item::Kind::Label));	// 見出し(連番)

		// 頂点数分ループ
		for (int j = 0; j < 3; j++)
		{
			std::string sVertex = "Vertex" + std::to_string(j);
			pGroupGround->AddGroupItem(Item::CreateBind(sVertex.c_str(), Item::Kind::Vector, &m_TriangleVertices[i].pos[j]));
		}

	}

	window.AddItem(pGroupGround);
}

#endif // _DEBUG