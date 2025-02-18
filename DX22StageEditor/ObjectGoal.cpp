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
	, m_bOnFloatAnime(true)
	, m_vBasePos(Vector3<float>(0.0f, 0.0f, 0.0f))
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
	// ゴールしたら非表示にする
	if (m_bIsGoal)
	{
		m_pModel->SetIsVisible(false);
	}

	if (!m_bOnFloatAnime) return;	// アニメーションフラグが立っていない場合は処理しない

	GetTransform()->RotateY(ROTATE_SPEED * DELTA_TIME);	// 回転

	// 上下にふわふわと揺らす
	m_fAnimeTimeCnt += DELTA_TIME;	// アニメーション時間カウント

	Vector3<float> vMovePos = Vector3<float>::Zero();					// 移動座標
	vMovePos.y += sinf(m_fAnimeTimeCnt * FLOAT_SPEED) * FLOAT_RANGE;	// 上下移動
	GetTransform()->SetLocalPosition(m_vBasePos + vMovePos);			// 座標設定
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

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectGoal::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// 基本座標
	data.vBasePos = m_vBasePos;

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectGoal::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// 基本座標
	m_vBasePos = data.vBasePos;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectGoal::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGoal = Item::CreateGroup("Goal");

	// 基本座標
	pGroupGoal->AddGroupItem(Item::CreateBind("BasePos", Item::Vector, &m_vBasePos));

	// 浮遊アニメーション
	pGroupGoal->AddGroupItem(Item::CreateBind("OnFloatAnime", Item::Bool, &m_bOnFloatAnime));

	window.AddItem(pGroupGoal);
}
#endif // _DEBUG