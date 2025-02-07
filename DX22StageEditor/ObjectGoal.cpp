/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ゴール)用cpp
	------------------------------------
	ObjectGoal.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGoal.h"
#include "ComponentCollisionSphere.h"
#include "ComponentModelStatic.h"
#include "ComponentTransform.h"

#include "ModelStaticManager.h"

// =============== 定数定義 =======================
const float LIGHT_DIFFUSE	= 1.0f;	// 拡散反射率
const float LIGHT_SPECULAR	= 2.5f;	// 鏡面反射率
const float LIGHT_SHININESS = 0.3f;	// 輝き係数

const Vector3<float> POS_OFFSET = { 0.0f, 0.0f, 0.0f };	// モデルオフセット

// 移動アニメ
const float ROTATE_SPEED	= 50.0f;	// 回転速度(1秒間の回転角度)
const float FLOAT_SPEED		= 2.0f;		// 浮遊速度
const float FLOAT_RANGE		= 0.1f;		// 浮遊範囲


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectGoal::ObjectGoal(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_bIsGoal(false)
	, m_pModel(nullptr)
	, m_fAnimeTimeCnt(0.0f)
	, m_bIsSetBasePos(false)
{
	SetTag(E_ObjectTag::Goal);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectGoal::InitLocal()
{
	AddComponent<ComponentCollisionSphere>()->SetTrigger(true);

	m_pModel = AddComponent<ComponentModelStatic>();
	m_pModel->SetModel(GET_MODEL_STATIC(MODEL_KEY::GOAL_NIKUKYU));
	m_pModel->SetPosOffset(POS_OFFSET);

	// マテリアル設定
	SetLightMaterial(LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_SHININESS);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectGoal::UpdateLocal()
{
	// 基準座標設定
	if (!m_bIsSetBasePos) 
	{
		m_vBasePos = GetTransform()->GetPosition();	// 基準座標設定
		m_bIsSetBasePos = true;
	}

	// ゴールしたら非表示にする
	if (m_bIsGoal)
	{
		m_pModel->SetIsVisible(false);
	}

	GetTransform()->RotateY(ROTATE_SPEED * DELTA_TIME);	// 回転

	// 上下にふわふわと揺らす
	m_fAnimeTimeCnt += DELTA_TIME;	// アニメーション時間カウント

	Vector3<float> vPos = m_vBasePos;		// 現在の座標取得
	vPos.y += sinf(m_fAnimeTimeCnt * FLOAT_SPEED) * FLOAT_RANGE;	// 上下移動
	GetTransform()->SetPosition(vPos);							// 座標設定
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectGoal::OnCollisionEnter(ObjectBase* pHit)
{
	// プレイヤーと接触したらゴールフラグを立てる
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		m_bIsGoal = true;
	}
}

/* ========================================
	ゲッター(ゴールフラグ)関数
	-------------------------------------
	戻値：bool	ゴールフラグ
=========================================== */
bool ObjectGoal::GetIsGoal()
{
	return m_bIsGoal;
}

/* ========================================
	セッター(ゴールフラグ)関数
	-------------------------------------
	引数：bool	ゴールフラグ
=========================================== */
void ObjectGoal::SetIsGoal(bool bIsGoal)
{
	m_bIsGoal = bIsGoal;
}
