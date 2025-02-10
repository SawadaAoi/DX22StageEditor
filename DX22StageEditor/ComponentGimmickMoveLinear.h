/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(ステージギミック直線移動)用ヘッダ
	------------------------------------
	説明：ステージギミックの直線移動を行うコンポーネント
		　敵キャラのコンポーネントとの違いは、
		  向きと、移動が座標指定であること
	------------------------------------
	ComponentGimmickMoveLinear.h
========================================== */
#pragma once
#include "ComponentBase.h"

// =============== インクルード =====================
#include <vector>
#include <memory>

// =============== 前方宣言 =======================
class ShapeLine;
class ComponentTransform;
class ComponentRigidbody;

// =============== クラス定義 =====================
class ComponentGimmickMoveLinear :
    public ComponentBase
{
public:
	ComponentGimmickMoveLinear(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	void AddWayPoint(const Vector3<float>& vWayPoint);
	void InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex);
	void RemoveWayPoint(int nIndex);

	// ゲッター
	float GetMoveSpeed();
	std::vector<Vector3<float>>& GetWayPoints();
	bool GetIsReverse();

	// セッター
	void SetMoveSpeed(float fSpeed);
	void SetIsReverse(bool bIsReverse);	

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	void InitDebugWayPointList();
	void FuncWayCurrent(bool isWrite, void* arg);
	void FuncWayPoint(bool isWrite, void* arg);
#endif // _DEBUG
private:
	void Move();
	void ReverseMove();
private:
	ComponentTransform* m_pCompTransform;
	float				m_fMoveSpeed;

	std::vector<Vector3<float>> m_vtWayPoints;	// 移動座標配列
	int m_nCurrentWayPoint;						// 現在の座標番号

	bool m_bIsReverse;							// 逆順フラグ

	bool m_bDispMoveLine;						// 移動ライン表示フラグ
	std::unique_ptr<ShapeLine> m_pMoveLine;		// 移動ライン描画用

#ifdef _DEBUG
	int m_nSelectWayPointIdx; 					// 選択中の座標番号
#endif // _DEBUG
};

