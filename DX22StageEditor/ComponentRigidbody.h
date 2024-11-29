/* ========================================
	DX22Base/
	------------------------------------
	物理挙動コンポーネント用ヘッダ
	------------------------------------
	説明：物理挙動を管理するコンポーネント
		　(速度、加速度、質量、重力の有無)
	------------------------------------
	ComponentRigidbody.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include "Vector3.h"

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentGroundRaycast;
class ComponentCollisionOBB;

// =============== クラス定義 ===================
class ComponentRigidbody :
    public ComponentBase
{
public:
	// 力の種類
	enum E_ForceMode
	{
		FORCE,				// 継続的な力
		IMPULSE,			// 瞬間的な力
	};

public:
    ComponentRigidbody(ObjectBase* pOwner);
	~ComponentRigidbody() {};
	void Init() override;
	void Update() override;

	void AddForce(const Vector3<float>& vForce, E_ForceMode eMode = E_ForceMode::FORCE);

    // セッター
    void SetVelocity(const Vector3<float>& vVelocity);
	void SetAcceleration(const Vector3<float>& vAcceleration);
	void SetMass(float fMass);
	void SetUseGravity(bool bUseGravity);
	void SetAirDrag(float fAirDrag);
	void SetGroundDrag(float fGroundDrag);

	// ゲッター
	Vector3<float> GetVelocity() const;
	Vector3<float> GetAcceleration() const;
	float GetMass() const;
	bool GetUseGravity() const;

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void CheckRaycastGround();		// 地面接触判定
	void ResolveOverlapCollision();	// 衝突時のめり込み解決
private:
	ComponentTransform*		m_pCompTransform;		// 座標、回転、スケール
	ComponentGroundRaycast* m_pCompGroundRay;		// 地面接触判定
	ComponentCollisionOBB*	m_pCompCollisionOBB;	// 衝突判定(OBB)

    Vector3<float>			m_vVelocity;		// 速度
    Vector3<float>			m_vAcceleration;	// 加速度
	float					m_fMass;			// 質量
	bool					m_bUseGravity;		// 重力の有無
	
	float m_fAirDrag;		// 空気抵抗
	float m_fGroundDrag;	// 地面摩擦

	bool					m_bIsGround;			// 地面に接触しているかどうか
	float					m_fGroundCheckDelay;	// 地面接触判定の遅延時間

#ifdef _DEBUG
	Vector3<float>			m_vDispAccel;		// 加速度表示
#endif // _DEBUG


};

