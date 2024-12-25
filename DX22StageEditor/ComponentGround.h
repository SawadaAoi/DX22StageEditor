/* ========================================
	CatRobotGame/
	------------------------------------
	地面コンポーネント用ヘッダ
	------------------------------------
	説明：キャラが乗る地面のコンポーネント
		　法線や三角形の頂点情報
	------------------------------------
	ComponentGround.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Vector3.h"
#include "ComponentBase.h"
#include <memory>
#include <vector>
#include "ShapeLine.h"

// =============== 前方宣言 =======================
class ComponentTransform;


// =============== クラス定義 ===================
class ComponentGround :
    public ComponentBase
{
public:
	// 四角形を2つの三角形に分割した時の頂点構造体
	struct T_TriangleVertex
	{
		Vector3<float> pos[3];	// 座標
	};

public:
	ComponentGround(ObjectBase* pOwner);
	~ComponentGround() {};

	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	Vector3<float> GetWorldNormalDirection();			// 法線を取得
	bool GetIsDispNormal();								// 法線の表示フラグを取得
	std::vector<T_TriangleVertex> GetTriangleVertex();	// 三角形の頂点を取得

	// セッター
	void SetNormalDirection(const Vector3<float>& vNormal);		// 法線の向きを設定
	void SetIsDispNormal(bool bIsDispNormal);					// 法線の表示フラグを設定

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数


#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
protected:
	virtual void UpdateTriangleVertex();	// 三角形の頂点を更新
protected:
	ComponentTransform* m_pCompTransform;	// 所有オブジェクトの座標、回転、スケール

	std::unique_ptr<ShapeLine>			m_pNormalLine;				// 法線
	Vector3<float>						m_vNormalDirection;			// 法線の方向
	Vector3<float>						m_vWorldNormalDirection;	// ワールド座標の法線の方向
	bool								m_bIsDispNormal;			// 法線の表示フラグ
	std::vector<T_TriangleVertex>		m_TriangleVertices;			// 地面を構成する三角形の頂点(左上、右下)
};

