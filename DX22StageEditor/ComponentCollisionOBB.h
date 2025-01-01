/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント(OBB)用ヘッダ
	------------------------------------
	説明：オブジェクトの回転に合わせて
		　回転する衝突判定形状
	------------------------------------
	ComponentCollisionOBB.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentCollisionBase.h"

// =============== クラス定義 =====================
class ComponentCollisionOBB :
    public ComponentCollisionBase
{
public:
	// 軸の種類(T_OBBのvAxis, fExtentを見やすくするため)
	enum E_Axis
	{
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z,
	};

	// OBB構造体(衝突判定に使う値をまとめる)
	struct T_OBB
	{
		Vector3<float>	vCenter;		// 中心座標
		Vector3<float>	vAxis[3];		// 軸
		float			fExtent[3];		// 各軸の長さ(Vector3じゃないのはfor文で使うため)
		std::string		sName;			// 衝突相手オブジェクト名

		T_OBB()
		{
			vCenter = Vector3<float>(0.0f, 0.0f, 0.0f);
			vAxis[AXIS_X] = Vector3<float>(1.0f, 0.0f, 0.0f);
			vAxis[AXIS_Y] = Vector3<float>(0.0f, 1.0f, 0.0f);
			vAxis[AXIS_Z] = Vector3<float>(0.0f, 0.0f, 1.0f);
			fExtent[AXIS_X] = 0.0f;
			fExtent[AXIS_Y] = 0.0f;
			fExtent[AXIS_Z] = 0.0f;
			sName = "None";
		}
	};


public:
	ComponentCollisionOBB(ObjectBase* pOwner);
	void Init() override;
	void Update() override;

	// 衝突判定
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionOBB(ComponentCollisionBase* otherCol);		// OBBとOBB
	bool CheckCollisionAABB(ComponentCollisionAABB* otherCol);		// OBBとAABB
	bool CheckCollisionSphere(ComponentCollisionSphere* otherCol);	// OBBと球
	T_OBB CreateOBB(ComponentCollisionBase* Col);					// OBB構造体作成



	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	bool CheckLocalSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis);	// OBB同士のローカル軸分離判定
	bool CheckCrossSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis);	// OBB同士の交差軸分離判定

	float GetProjectionLength(Vector3<float> vAxis, T_OBB tObb);	// 射影線の長さ取得
};

