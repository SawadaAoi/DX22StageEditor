/* ========================================
	CatRobotGame/
	------------------------------------
	地面コンポーネント(ボックス)用ヘッダ
	------------------------------------
	説明：キャラが乗る地面のコンポーネント
		　法線や三角形の頂点情報
		 ※AABBやOBBの箱の天面に地面を設定する
		 　三角形の頂点情報の求め方のみを変更
	------------------------------------
	ComponentGroundBox.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentGround.h"

// =============== クラス定義 ===================
class ComponentGroundBox :
    public ComponentGround
{
public:
	ComponentGroundBox(ObjectBase* pOwner);

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

private:
	void UpdateTriangleVertex() override;	// 三角形の頂点を更新

};

