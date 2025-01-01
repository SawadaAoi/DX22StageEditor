/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(プレイヤー)用cpp
	------------------------------------
	ObjectCameraPlayer.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectCameraPlayer.h"
#include "SceneBase.h"

#include "ObjectPlayer.h"

#include "ComponentTransform.h"
#include "ComponentCameraBase.h"

#include "Input.h"
#include "MathUtils.h"

// =============== 定数定義 =======================
const float DEFAULT_ROT_SPEED	= 0.01f;						// デフォルト回転速度
const float DEFAULT_DISTANCE	= 8.0f;							// デフォルト距離
const float DEFAULT_RAD_Y		= MathUtils::ToRadian(45.0f);	// デフォルトY軸回転角度
const float DEFAULT_RAD_XZ		= MathUtils::ToRadian(180.0f);	// デフォルトXZ平面回転角度

// 角度制限
const float LIMIT_RAD_Y_MAX = MathUtils::ToRadian(89.0f);	// Y軸回転角度最大値
const float LIMIT_RAD_Y_MIN = MathUtils::ToRadian(20.0f);	// Y軸回転角度最小値

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1: シーンクラスのポインタ
=========================================== */
ObjectCameraPlayer::ObjectCameraPlayer(SceneBase* pScene)
	: ObjectCamera(pScene)
	, m_pPlayer(nullptr)
	, m_fRotSpeed(DEFAULT_ROT_SPEED)
	, m_fDistance(DEFAULT_DISTANCE)
	, m_fRadY(DEFAULT_RAD_Y)
	, m_fRadXZ(DEFAULT_RAD_XZ)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ObjectCameraPlayer::InitLocal()
{
	ObjectCamera::InitLocal();
	// プレイヤーオブジェクトを取得
	m_pPlayer = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectCameraPlayer::UpdateLocal()
{
	// カメラがアクティブでない場合は処理しない
	if (!m_bActive) return;

	// プレイヤーオブジェクトが取得できていない場合は取得
	if (!m_pPlayer)
	{
		m_pPlayer = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);
		return;
	}


	// プレイヤーの座標を取得
	Vector3<float> playerPos = m_pPlayer->GetComponent<ComponentTransform>()->GetWorldPosition();

	// 注視点をプレイヤーの座標に設定
	m_pCompCameraBase->SetLook(playerPos);
	m_pCompTransform->LookAt(playerPos);		// 移動時にカメラの正面ベクトルを使用するので回転も変更する

	if (Input::IsKeyPress(VK_UP))		m_fRadY		+= m_fRotSpeed;
	if (Input::IsKeyPress(VK_DOWN))		m_fRadY		-= m_fRotSpeed;
	if (Input::IsKeyPress(VK_LEFT))		m_fRadXZ	-= m_fRotSpeed;
	if (Input::IsKeyPress(VK_RIGHT))	m_fRadXZ	+= m_fRotSpeed;

	CheckLimitRadY();	// Y軸回転角度制限

	Vector3<float> cameraPos;
	cameraPos.x = m_fDistance * sinf(m_fRadXZ) * cosf(m_fRadY) + m_pCompCameraBase->GetLook().x;
	cameraPos.y = m_fDistance * sinf(m_fRadY) + m_pCompCameraBase->GetLook().y;
	cameraPos.z = m_fDistance * cosf(m_fRadY) * cosf(m_fRadXZ) + m_pCompCameraBase->GetLook().z;

	m_pCompTransform->SetLocalPosition(cameraPos);
}

/* ========================================
	Y軸回転角度制限関数
	-------------------------------------
	内容：Y軸回転角度の制限
=========================================== */
void ObjectCameraPlayer::CheckLimitRadY()
{
	if (m_fRadY > LIMIT_RAD_Y_MAX) m_fRadY = LIMIT_RAD_Y_MAX;
	if (m_fRadY < LIMIT_RAD_Y_MIN) m_fRadY = LIMIT_RAD_Y_MIN;
}


/* ========================================
	ゲッター(ターゲット距離)関数
	-------------------------------------
	戻値：float ターゲット距離
=========================================== */
float ObjectCameraPlayer::GetDistance()
{
	return m_fDistance;
}

/* ========================================
	ゲッター(カメラY軸回転角度)関数
	-------------------------------------
	戻値：float カメラY軸回転角度
=========================================== */
float ObjectCameraPlayer::GetRadY()
{
	return m_fRadY;
}

/* ========================================
	ゲッター(カメラXZ平面回転角度)関数
	-------------------------------------
	戻値：float カメラXZ平面回転角度
=========================================== */
float ObjectCameraPlayer::GetRadXZ()
{
	return 0.0f;
}

/* ========================================
	セッター(ターゲット距離)関数
	-------------------------------------
	引数1：float ターゲット距離
=========================================== */
void ObjectCameraPlayer::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

/* ========================================
	セッター(カメラY軸回転角度)関数
	-------------------------------------
	引数1：float カメラY軸回転角度
=========================================== */
void ObjectCameraPlayer::SetRadY(float fRadY)
{
	m_fRadY = fRadY;
}

/* ========================================
	セッター(カメラXZ平面回転角度)関数
	-------------------------------------
	引数1：float カメラXZ平面回転角度
=========================================== */
void ObjectCameraPlayer::SetRadXZ(float fRadXZ)
{
	m_fRadXZ = fRadXZ;
}
