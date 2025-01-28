/* ========================================
	DX22Base/
	------------------------------------
	物理挙動コンポーネント用cpp
	------------------------------------
	ComponentRigidbody.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "ComponentGroundRaycast.h"
#include "ComponentCollisionBase.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionSphere.h"
#include "ObjectBase.h"


// =============== 定数定義 =======================
const float GRAVITY_FORCE		= -9.8f;	// 重力
const float GROUND_CHECK_DELAY	= 0.1f;		// 地面接触判定の遅延時間(

// 抵抗初期値
const float DEFAULT_AIR_DRAG	= 0.1f;  // 空気抵抗の係数
const float DEFAULT_GROUND_DRAG = 0.5f;  // 地面上での摩擦（減衰）係数

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentRigidbody::ComponentRigidbody(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderRigidBody)
	, m_pCompTransform(nullptr)
	, m_pCompGroundRay(nullptr)
	, m_pCompCollisionBase(nullptr)
	, m_vVelocity(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_vAcceleration(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_fMass(1.0f)
	, m_bUseGravity(true)
	, m_bIsGround(false)
	, m_fGroundCheckDelay(0.0f)
	, m_fAirDrag(DEFAULT_AIR_DRAG)
	, m_fGroundDrag(DEFAULT_GROUND_DRAG)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ComponentRigidbody::Init()
{
	m_pCompTransform		= m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompGroundRay		= m_pOwnerObj->GetComponent<ComponentGroundRaycast>();

	SetCollisionComponent();	
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ComponentRigidbody::Update()
{
	// 重力を使用する場合は重力を加算
	if (m_bUseGravity)
	{
		m_vAcceleration += Vector3<float>(0.0f, GRAVITY_FORCE, 0.0f);
	}

	// 抵抗力の計算（空気中と地面で異なる係数を使用）
	float dragCoefficient		= m_bIsGround ? m_fGroundDrag : m_fAirDrag;
	Vector3<float> fDragForce	= (m_vVelocity * -1.0f) * dragCoefficient;

	// 抵抗力を加速度に反映
	m_vAcceleration += fDragForce / m_fMass;

	m_vVelocity		 += m_vAcceleration * DELTA_TIME;		// 加速度を速度に加算
	m_pCompTransform->Translate(m_vVelocity * DELTA_TIME);	// 速度を座標に加算


	// 衝突判定がある場合
	if (m_pCompCollisionBase)
	{
		ResolveOverlapCollision();	// 衝突時のめり込み解決
	}
	else
	{
		SetCollisionComponent();
	}


	// 地面接触判定がある場合
	if (m_pCompGroundRay)
	{
		CheckRaycastGround();	// 地面接触判定
		
	}
	else
	{
		m_pCompGroundRay = m_pOwnerObj->GetComponent<ComponentGroundRaycast>();
	}


	//DebugConsole::Printf("Velocity: %f, %f, %f", m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	//DebugConsole::Printf("HitFlg: %d", m_pGroundRaycast->GetHitFlg());

#ifdef _DEBUG
	// 加速度表示用(毎フレームリセットされるので別変数に保存)
	m_vDispAccel = m_vAcceleration;

#endif // _DEBUG
	m_vAcceleration = Vector3<float>::Zero();			// 加速度をリセット

}

/* ========================================
	力加算関数
	-------------------------------------
	内容：オブジェクトに力を加算する
	-------------------------------------
	引数1：力のベクトル
	引数2：力の種類
========================================= */
void ComponentRigidbody::AddForce(const Vector3<float>& vForce, E_ForceMode eMode)
{
	// 質量が0の場合は計算しない
	if (m_fMass == 0.0f) return;

	switch (eMode)
	{
	// 力を継続的に加算
	case E_ForceMode::FORCE:
		m_vAcceleration += vForce / m_fMass;
		break;
	// 瞬間的な力を加算
	case E_ForceMode::IMPULSE:
		m_vVelocity += vForce / m_fMass;
		break;
	}

	// ジャンプの場合は地面接触判定の遅延時間を設定
	if (eMode == E_ForceMode::IMPULSE && vForce.y > 0.0f && m_pCompGroundRay)
	{
		m_fGroundCheckDelay = GROUND_CHECK_DELAY;	
	}
}

/* ========================================
	地面接触判定関数
	-------------------------------------
	内容：地面に自身が持つレイが接触しているかどうかを判定
========================================= */
void ComponentRigidbody::CheckRaycastGround()
{
	// 地面接触判定の遅延
	// 上方向に力を加えても次フレームでまだ接地判定が有効の為、上速度が0リセットされてしまうので
	// 力を加えてから一定時間は接地判定を無効にする
	if (m_fGroundCheckDelay > 0.0f)
	{
		m_fGroundCheckDelay -= DELTA_TIME;
	}
	// 地面に接触している場合は速度を0にする
	else if (m_pCompGroundRay->GetHitFlg())
	{
		m_vVelocity.y = 0.0f;
		m_bIsGround = true;
	}
	else
	{
		m_bIsGround = false;
	}
}

/* ========================================
	めり込み解消関数
	-------------------------------------
	内容：衝突時のめり込みを元に戻す
========================================= */
void ComponentRigidbody::ResolveOverlapCollision()
{
	using T_MTV = ComponentCollisionOBB::T_MTV;

	// めり込み情報配列を取得
	std::vector<T_MTV> mtvs = m_pCompCollisionBase->GetMtvs();

	// オブジェクト数分ループ
	for (const T_MTV& mtv : mtvs)
	{
		if (!mtv.bIsCol) continue;										// 衝突していない場合は次のオブジェクトへ
		if (mtv.bIsTrigger) continue;									// トリガーの場合は次のオブジェクトへ
		if (m_pCompCollisionBase->GetTrigger()) continue;				// トリガーの場合は次のオブジェクトへ
		// レイ衝突地面は無視
		// ※地面ブロックが斜めの場合に、めり込み解消があると、滑り落ちるのを防ぐため
		if (mtv.sName == m_pCompGroundRay->GetHitObjName()) continue;	

		// めり込み方向
		Vector3<float> vReturnDir	= mtv.vAxis * -1.0f;	// 衝突軸の反対方向
		float fOverlapDis			= mtv.fOverlap;			// 重なり量

		m_pCompTransform->Translate(vReturnDir * fOverlapDis);

		// 衝突軸に沿った速度の減衰
		float fDot = m_vVelocity.Dot(vReturnDir);	// 移動方向がどれだけめり込み方向に沿っているか

		// 移動方向とめり込み方向が逆の場合
		if (fDot < 0.0f)
		{
			m_vVelocity -= (vReturnDir * fDot);	// めり込み方向に対する速度の成分を減算
		}
	}
}

/* ========================================
	衝突コンポーネント設定関数
	-------------------------------------
	内容：所有者オブジェクトに設定されているコリジョンコンポーネントを取得
		　形状によって異なるコリジョンコンポーネントを取得
========================================= */
void ComponentRigidbody::SetCollisionComponent()
{
	// AABB
	if (m_pOwnerObj->GetComponent<ComponentCollisionAABB>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionAABB>();
	}
	// OBB
	else if (m_pOwnerObj->GetComponent<ComponentCollisionOBB>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionOBB>();
	}
	// 球
	else if (m_pOwnerObj->GetComponent<ComponentCollisionSphere>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionSphere>();
	}
}


/* ========================================
	セッター(速度)関数
	-------------------------------------
	引数：Vector3<float>	速度
=========================================== */
void ComponentRigidbody::SetVelocity(const Vector3<float>& vVelocity)
{
	m_vVelocity = vVelocity;
}

/* ========================================
	セッター(加速度)関数
	-------------------------------------
	引数：Vector3<float>	加速度
=========================================== */
void ComponentRigidbody::SetAcceleration(const Vector3<float>& vAcceleration)
{
	m_vAcceleration = vAcceleration;
}

/* ========================================
	セッター(質量)関数
	-------------------------------------
	引数：float	質量
=========================================== */
void ComponentRigidbody::SetMass(float fMass)
{
	m_fMass = fMass;
}

/* ========================================
	セッター(重力有効フラグ)関数
	-------------------------------------
	引数：bool	重力有効フラグ
=========================================== */
void ComponentRigidbody::SetUseGravity(bool bUseGravity)
{
	m_bUseGravity = bUseGravity;

}

/* ========================================
	セッター(空気抵抗)関数
	-------------------------------------
	引数：float	空気抵抗
=========================================== */
void ComponentRigidbody::SetAirDrag(float fAirDrag)
{
	m_fAirDrag = fAirDrag;
}

/* ========================================
	セッター(地面摩擦)関数
	-------------------------------------
	引数：float	地面摩擦
=========================================== */
void ComponentRigidbody::SetGroundDrag(float fGroundDrag)
{
	m_fGroundDrag = fGroundDrag;
}


/* ========================================
	ゲッター(速度)関数
	-------------------------------------
	戻値：Vector3<float>	速度
=========================================== */
Vector3<float> ComponentRigidbody::GetVelocity() const
{
	return m_vVelocity;
}

/* ========================================
	ゲッター(加速度)関数
	-------------------------------------
	戻値：Vector3<float>	加速度
=========================================== */
Vector3<float> ComponentRigidbody::GetAcceleration() const
{
	return m_vAcceleration;
}

/* ========================================
	ゲッター(質量)関数
	-------------------------------------
	戻値：float	質量
=========================================== */
float ComponentRigidbody::GetMass() const
{
	return m_fMass;
}

/* ========================================
	セッター(重力有効フラグ)関数
	-------------------------------------
	引数：bool	重力有効フラグ
=========================================== */
bool ComponentRigidbody::GetUseGravity() const
{
	return m_bUseGravity;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentRigidbody::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupRig = Item::CreateGroup("Rigidbody");

	pGroupRig->AddGroupItem(Item::CreateBind("Velocity",	Item::Kind::Vector, &m_vVelocity));
	pGroupRig->AddGroupItem(Item::CreateBind("Acceleration",Item::Kind::Vector, &m_vDispAccel));
	pGroupRig->AddGroupItem(Item::CreateBind("Mass",		Item::Kind::Float,	&m_fMass));
	pGroupRig->AddGroupItem(Item::CreateBind("UseGravity",	Item::Kind::Bool,	&m_bUseGravity));
	pGroupRig->AddGroupItem(Item::CreateBind("AirDrag",		Item::Kind::Float,	&m_fAirDrag));
	pGroupRig->AddGroupItem(Item::CreateBind("GroundDrag",	Item::Kind::Float,	&m_fGroundDrag));

	window.AddItem(pGroupRig);
}

#endif // _DEBUG