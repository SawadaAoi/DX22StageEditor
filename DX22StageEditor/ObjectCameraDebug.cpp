/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(デバッグ)用cpp
	------------------------------------
	ObjectCameraDebug.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectCameraDebug.h"
#include "ComponentCameraDebug.h"
#include "ComponentTransform.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1: シーンクラスのポインタ
=========================================== */
ObjectCameraDebug::ObjectCameraDebug(SceneBase* pScene)
	: ObjectCamera(pScene)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectCameraDebug::InitLocal()
{
	ObjectCamera::InitLocal();										// 基底クラスの初期化関数を呼ぶ
	m_pCompTransform->SetPosition(Vector3(0.0f, 1.0f, -5.0f));	// カメラ位置を設定
	AddComponent<ComponentCameraDebug>();							// デバッグ機能を追加
}
