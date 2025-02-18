/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(移動床)用ヘッダ
	------------------------------------
	説明：移動する床のオブジェクト
		　(プレイヤーが乗ることができる)
	------------------------------------
	ObjectMoveFloor.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"
#include "ObjectBlock.h"


// =============== 前方宣言 =======================
class ComponentGimmickMoveLinear;
class ComponentRigidbody;
class ComponentGroundBox;

// =============== クラス定義 =====================
class ObjectMoveFloor :
    public ObjectBlock
{
public:
	// セーブデータ
	struct S_SaveData
	{
		float fMoveSpeed;	// 移動速度
		int nWayPointNum;	// 移動座標数
		bool bIsReverse;	// 逆順フラグ
	};
public:
	ObjectMoveFloor(SceneBase* pScene);
	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;
	void OnCollisionExit(ObjectBase* pHit) override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectMoveFloor)
private:
	ComponentGimmickMoveLinear* m_pCompMoveLinear;
	ComponentRigidbody* m_pCompRigidbody;
	ComponentGroundBox* m_pCompGroundBox;
};

