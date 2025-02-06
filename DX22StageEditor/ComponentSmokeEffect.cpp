/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(移動時煙エフェクト)用cpp
	------------------------------------
	ComponentSmokeEffect.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentSmokeEffect.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "TextureManager.h"
#include "MathUtils.h"

// =============== 定数定義 =======================
const float SMOKE_CREATE_INTERVAL	= 0.1f;	// 煙生成間隔
const float SMOKE_LIFE_TIME			= 0.5f;	// 煙の寿命

// 生成煙パラメータ
// 大きさ
const float SMOKE_SCALE_MIN = 0.2f;
const float SMOKE_SCALE_MAX = 0.8f;

// 座標
const float SMOKE_POS_ADJUST_MIN = -0.25f;
const float SMOKE_POS_ADJUST_MAX = 0.25f;

// オブジェクトと煙生成位置の距離
const float SMOKE_CREATE_POS_DIST_XZ	= 0.5f;
const float SMOKE_CREATE_POS_DIST_Y		= -0.25f;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
=========================================== */
ComponentSmokeEffect::ComponentSmokeEffect(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderSmoke)
	, m_pScene(nullptr)
	, m_fCreateItvTimeCnt(0.0f)
	, m_vMoveDir(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_vCreatePosDist(Vector3<float>(SMOKE_CREATE_POS_DIST_XZ, SMOKE_CREATE_POS_DIST_Y, SMOKE_CREATE_POS_DIST_XZ))
	, m_fLifeTime(SMOKE_LIFE_TIME)
	, m_fCreateInterval(SMOKE_CREATE_INTERVAL)
	, m_fScaleMin(SMOKE_SCALE_MIN)
	, m_fScaleMax(SMOKE_SCALE_MAX)
	, m_fPosAdjustMin(SMOKE_POS_ADJUST_MIN)
	, m_fPosAdjustMax(SMOKE_POS_ADJUST_MAX)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void ComponentSmokeEffect::Init()
{
	m_pScene = m_pOwnerObj->GetOwnerScene();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentSmokeEffect::Update()
{
	// 煙の更新
	for (auto& smoke : m_Smokes)
	{
		smoke.second += DELTA_TIME;

		// 生成されてから一定時間経過したら削除
		if (smoke.second >= m_fLifeTime)
		{
			smoke.first->SetState(ObjectBase::E_State::STATE_DEAD);
			smoke.first = nullptr;
		}
	}

	DeleteSmoke();	// 該当の煙削除

	// オブジェクトが移動していない場合は処理しない
	if (m_vMoveDir == Vector3<float>::Zero()) return;

	// 一定間隔で煙生成
	m_fCreateItvTimeCnt += DELTA_TIME;
	if (m_fCreateItvTimeCnt >= m_fCreateInterval)
	{
		CreateSmoke();
		m_fCreateItvTimeCnt = 0.0f;
	}


}

/* ========================================
	煙生成関数
	-------------------------------------
	内容：煙オブジェクト生成(ビルボード)
		　大きさ、座標がランダム
=========================================== */
void ComponentSmokeEffect::CreateSmoke()
{
	ObjectBase* pSmoke = m_pScene->AddSceneObject<ObjectBase>("Smoke");

	// 大きさ設定
	float scale = MathUtils::Random(m_fScaleMin, m_fScaleMax);
	pSmoke->GetTransform()->SetScale(Vector3<float>(scale, scale, scale));

	// 座標設定
	// 移動方向の逆方向にずらす
	Vector3<float> vMoveDirInv	= m_vMoveDir * -1.0f;							// 移動逆方向
	Vector3<float> vCreatePos	= m_pOwnerObj->GetTransform()->GetPosition();	// オブジェクトの座標を取得

	vCreatePos.x += vMoveDirInv.x * m_vCreatePosDist.x;
	vCreatePos.y += m_vCreatePosDist.y;
	vCreatePos.z += vMoveDirInv.z * m_vCreatePosDist.z;

	// 座標のずらし値
	Vector3<float> vAdjustPos = Vector3<float>::Zero();
	vAdjustPos.x = MathUtils::Random(m_fPosAdjustMin, m_fPosAdjustMax);
	vAdjustPos.z = MathUtils::Random(m_fPosAdjustMin, m_fPosAdjustMax);

	// ずらした座標にランダムな値を加算
	vCreatePos += vAdjustPos;	

	// 座標設定
	pSmoke->GetTransform()->SetPosition(vCreatePos);

	// テクスチャ設定
	pSmoke->AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_SMOKE));

	// 生成した煙をリストに追加
	m_Smokes.push_back(std::make_pair(pSmoke, 0.0f));
}

/* ========================================
	煙削除関数
	-------------------------------------
	内容：煙オブジェクト削除
=========================================== */
void ComponentSmokeEffect::DeleteSmoke()
{
	// 煙が消えたものを削除
	// nullptr を指している要素を削除する
	auto newEnd = std::remove_if(m_Smokes.begin(), m_Smokes.end(),
		[](const std::pair<ObjectBase*, float>& smoke)
	{
		return smoke.first == nullptr;
	});

	// remove_if によって不要な要素が末尾に移動されているため、それを削除する
	m_Smokes.erase(newEnd, m_Smokes.end());
}



/* ========================================
	セッター(移動方向)関数
	-------------------------------------
	引数：Vector3<float> vMoveDir
=========================================== */
void ComponentSmokeEffect::SetMoveDir(Vector3<float> vMoveDir)
{
	m_vMoveDir = vMoveDir;
}

/* ========================================
	セッター(生成位置調整)関数
	-------------------------------------
	引数：Vector3<float> vCreatePosDist
=========================================== */
void ComponentSmokeEffect::SetCreatePosDist(Vector3<float> vCreatePosDist)
{
	m_vCreatePosDist = vCreatePosDist;
}

/* ========================================
	セッター(生成間隔)関数
	-------------------------------------
	引数：float fCreateInterval
=========================================== */
void ComponentSmokeEffect::SetCreateInterval(float fCreateInterval)
{
	m_fCreateInterval = fCreateInterval;
}

/* ========================================
	セッター(煙の寿命)関数
	-------------------------------------
	引数：float fLifeTime
=========================================== */
void ComponentSmokeEffect::SetLifeTime(float fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

/* ========================================
	セッター(スケール)関数
	-------------------------------------
	引数1：float fScaleMin
	引数2：float fScaleMax
=========================================== */
void ComponentSmokeEffect::SetScale(float fScaleMin, float fScaleMax)
{
	m_fScaleMin = fScaleMin;
	m_fScaleMax = fScaleMax;
}

/* ========================================
	セッター(位置調整)関数
	-------------------------------------
	引数1：float fPosAdjustMin
	引数2：float fPosAdjustMax
=========================================== */
void ComponentSmokeEffect::SetPosAdjust(float fPosAdjustMin, float fPosAdjustMax)
{
	m_fPosAdjustMin = fPosAdjustMin;
	m_fPosAdjustMax = fPosAdjustMax;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentSmokeEffect::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupSmokeEffect = Item::CreateGroup("SmokeEffect");

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreateInterval", Item::Kind::Float, &m_fCreateInterval));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreateItvTimeCnt", Item::Kind::Float, &m_fCreateItvTimeCnt));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("LifeTime", Item::Kind::Float, &m_fLifeTime));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreatePosDist", Item::Kind::Vector, &m_vCreatePosDist));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("MoveDir", Item::Kind::Vector, &m_vMoveDir));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("ScaleMin", Item::Kind::Float, &m_fScaleMin));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("ScaleMax", Item::Kind::Float, &m_fScaleMax));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("PosAdjustMin", Item::Kind::Float, &m_fPosAdjustMin));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("PosAdjustMax", Item::Kind::Float, &m_fPosAdjustMax));

	window.AddItem(pGroupSmokeEffect);

}
#endif // _DEBUG