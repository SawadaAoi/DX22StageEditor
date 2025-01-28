/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(クリア時演出用cpp
	------------------------------------
	ObjectCameraGameClear.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectCameraGameClear.h"
#include "ComponentTransform.h"
#include "ComponentCameraBase.h"
#include "ObjectCameraPlayer.h"
#include "SceneBase.h"


// =============== 定数定義 =======================
const float CAMERA_STAND_BY_TIME = 2.0f;

// プレイヤーに近づく移動
const Vector3<float> CAMERA_NEAR_PLAYER_POS = Vector3<float>(0.0f, 2.0f, -3.0f);
const float NEAR_PLAYER_MOVE_TIME = 0.5f;

// ズームアウト移動
const Vector3<float> CAMERA_ZOOM_OUT_POS = Vector3<float>(20.0f, 20.0f, -20.0f);	// ステージを俯瞰で見る座標
const float ZOOM_OUT_MOVE_TIME = 8.0f;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1: シーンクラスのポインタ
========================================== */
ObjectCameraGameClear::ObjectCameraGameClear(SceneBase* pScene)
	: ObjectCamera(pScene)
	, m_fStandByTimeCnt(0.0f)
	, m_bZoomOutStart(false)
	, m_pTarget(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectCameraGameClear::InitLocal()
{
	ObjectCamera::InitLocal();

	// プレイヤーカメラを取得
	ObjectCameraPlayer* pPlayerCamera = m_pOwnerScene->GetSceneObject<ObjectCameraPlayer>();

	// プレイヤーカメラの情報をコピー(カメラの切り替えを自然に見せるため)
	m_pCompTransform->SetLocalPosition(pPlayerCamera->GetTransform()->GetLocalPosition());	// 座標
	m_pCompTransform->SetLocalRotation(pPlayerCamera->GetTransform()->GetLocalRotation());	// 回転
	m_pCompCameraBase->SetLook(pPlayerCamera->GetCameraBase()->GetLook());					// 注視点

	// ターゲット(プレイヤー)を取得
	m_pTarget = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);

	// プレイヤー接近移動開始
	EventMoveNearPlayer();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ObjectCameraGameClear::UpdateLocal()
{
	// 常にプレイヤーを向く
	m_pCompCameraBase->SetLook(m_pTarget->GetTransform()->GetPosition());
	m_pCompTransform->LookAt(m_pTarget->GetTransform()->GetPosition());

	// 待機時間カウント
	m_fStandByTimeCnt += DELTA_TIME;

	// 待機時間が経過したらズームアウト開始
	if (m_fStandByTimeCnt >= CAMERA_STAND_BY_TIME && !m_bZoomOutStart)
	{
		m_fStandByTimeCnt = 0.0f;
		m_bZoomOutStart = true;
		EventZoomOutStart();
	}
}

/* ========================================
	プレイヤー接近移動関数
	-------------------------------------
	内容：プレイヤーをZ軸負方向から見る座標に移動
=========================================== */
void ObjectCameraGameClear::EventMoveNearPlayer()
{
	Vector3<float> playerPos = m_pTarget->GetTransform()->GetPosition();	// プレイヤーの座標
	Vector3<float> vTargetPos = (playerPos + CAMERA_NEAR_PLAYER_POS);			// プレイヤーの座標からZ軸負方向に移動

	// 目的座標へ指定時間で移動
	m_pCompTransform->MoveTo(vTargetPos, NEAR_PLAYER_MOVE_TIME);
}

/* ========================================
	ズームアウト開始関数
	-------------------------------------
	内容：ステージを俯瞰で見る座標に移動を開始
=========================================== */
void ObjectCameraGameClear::EventZoomOutStart()
{
	// 目的座標へ指定時間で移動
	m_pCompTransform->MoveTo(CAMERA_ZOOM_OUT_POS, 8.0f);
}
