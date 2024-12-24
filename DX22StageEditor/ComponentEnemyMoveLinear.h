/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(直線)用ヘッダ
	------------------------------------
	説明：敵キャラの直線移動処理クラス
		　指定した座標を順番に移動する
	------------------------------------
	ComponentEnemyMoveLinear.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentEnemyMoveBase.h"
#include <vector>
#include <memory>

// =============== 前方宣言 =======================
class ShapeLine;


// =============== クラス定義 =====================
class ComponentEnemyMoveLinear :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveLinear(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	void AddWayPoint(const Vector3<float>& vWayPoint);
	void InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void ReverseMove();
private:
	std::vector<Vector3<float>> m_vtWayPoints;	// 移動座標
	int m_nCurrentWayPoint;						// 現在の座標番号

	bool m_bIsReverse;							// 逆順フラグ

	bool m_bDispMoveLine;						// 移動ライン表示フラグ
	std::unique_ptr<ShapeLine> m_pMoveLine;	// 移動ライン描画用
};

