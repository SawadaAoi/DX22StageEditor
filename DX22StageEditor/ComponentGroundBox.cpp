/* ========================================
	CatRobotGame/
	------------------------------------
	地面コンポーネント(ボックス)用cpp
	------------------------------------
	ComponentGroundBox.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"
#include <vector>

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentGroundBox::ComponentGroundBox(ObjectBase* pOwner)
	: ComponentGround(pOwner)
{
}

/* ========================================
	三角形頂点更新関数
	-------------------------------------
	内容：所有オブジェクトのスケール、座標に合わせて
		　三角形の頂点を更新
=========================================== */
void ComponentGroundBox::UpdateTriangleVertex()
{
	// 所有オブジェクトの座標、回転、スケールを取得
	Vector3<float> vScale	= m_pCompTransform->GetWorldScale();
	Vector3<float> vPos		= m_pCompTransform->GetWorldPosition();
	Quaternion vRot			= m_pCompTransform->GetWorldRotation();

	// 四角形の上面の頂点座標を計算
	std::vector<Vector3<float>> vVertexPos;		// 頂点座標
	Vector3<float> vHalfScale = vScale / 2.0f;	// 半分のスケール

	// 原点にある時のボックスの天面の頂点座標を求める
	vVertexPos.push_back(Vector3<float>(vHalfScale.x, vHalfScale.y, vHalfScale.z));		// 右上
	vVertexPos.push_back(Vector3<float>(-vHalfScale.x, vHalfScale.y, vHalfScale.z));	// 左上
	vVertexPos.push_back(Vector3<float>(vHalfScale.x, vHalfScale.y, -vHalfScale.z));	// 右下
	vVertexPos.push_back(Vector3<float>(-vHalfScale.x, vHalfScale.y, -vHalfScale.z));	// 左下

	// 座標、回転を適用
	for (int i = 0; i < vVertexPos.size(); i++)
	{
		// 回転
		vVertexPos[i] = vRot.Rotate(vVertexPos[i]);
		// 座標
		vVertexPos[i] += vPos;
	}

	// 三角形の頂点を設定
	// 1つ目の三角形 (左上、右上、左下)
	m_TriangleVertices[0].pos[0] = vVertexPos[1];	// 左上
	m_TriangleVertices[0].pos[1] = vVertexPos[0];	// 右上
	m_TriangleVertices[0].pos[2] = vVertexPos[2];	// 左下

	// 2つ目の三角形 (右上、右下、左下)
	m_TriangleVertices[1].pos[0] = vVertexPos[0];	// 右上
	m_TriangleVertices[1].pos[1] = vVertexPos[3];	// 右下
	m_TriangleVertices[1].pos[2] = vVertexPos[2];	// 左下
	
}
