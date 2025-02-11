/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(基礎)用cpp
	------------------------------------
	ObjectCamera.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectCamera.h"
#include "ComponentCameraBase.h"
#include "ComponentTransform.h"
#include "CameraManager.h"
#include "ComponentCameraDebug.h"
#include "CameraManager.h"
#include "ComponentBillboard.h"
#include "TextureManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1: シーンクラスのポインタ
=========================================== */
ObjectCamera::ObjectCamera(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompCameraBase(nullptr)
	, m_bActive(false)
{
	SetTag(E_ObjectTag::Camera);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ObjectCamera::InitLocal()
{
	// コンポーネントの追加
	m_pCompCameraBase = AddComponent<ComponentCameraBase>();	// カメラの基本機能
	m_pCompTransform = GetComponent<ComponentTransform>();		// 座標、回転、拡大縮小(初めからセット済み)

	// カメラマネージャにカメラを追加
	CAMERA_MNG_INST.AddCamera(this);	// InitLocalで追加するのは名前が設定された後にするため

#ifdef _DEBUG
	// デバッグ用カメラアイコン
	AddComponent<ComponentBillboard>();
	GetComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::DEBUG_CAMERA_ICON));
#endif // _DEBUG
}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void ObjectCamera::UninitLocal()
{
	CAMERA_MNG_INST.RemoveCamera(this);	// カメラマネージャからカメラを削除
}


/* ========================================
	ゲッター(ビュー行列)関数
	-------------------------------------
	戻値：ビュー行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetViewMatrix()
{
	return m_pCompCameraBase->GetViewMatrix();
}

/* ========================================
	ゲッター(ビュー行列の逆行列)関数
	-------------------------------------
	戻値：ビュー行列の逆行列
=========================================== */
DirectX::XMMATRIX ObjectCamera::GetInvViewMatrix()
{
	return m_pCompCameraBase->GetInvViewMatrix();
}

/* ========================================
	ゲッター(プロジェクション行列)関数
	-------------------------------------
	戻値：プロジェクション行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrix()
{
#ifdef _DEBUG
	// デバッグメニューの平行投影フラグがONの場合
	if (WIN_CAMERA_INFO["Orthographic"].GetBool())
	{
		return m_pCompCameraBase->GetProjectionMatrixOrtho();
	}
	else
	{
		return m_pCompCameraBase->GetProjectionMatrix();
	}

#else
	return m_pCompCameraBase->GetProjectionMatrix();

#endif // _DEBUG

}

/* ========================================
	ゲッター(2D用プロジェクション行列)関数
	-------------------------------------
	戻値：2D用プロジェクション行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixUI()
{
	return m_pCompCameraBase->GetProjectionMatrixUI();
}

/* ========================================
	ゲッター(平行投影プロジェクション行列)関数
	-------------------------------------
	戻値：平行投影プロジェクション行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixOrtho()
{
	return m_pCompCameraBase->GetProjectionMatrixOrtho();
}

/* ========================================
	ゲッター(有効フラグ)関数
	-------------------------------------
	戻値：有効フラグ状態
=========================================== */
bool ObjectCamera::GetActive()
{
	return m_bActive;
}

/* ========================================
	ゲッター(カメラ基本機能)関数
	-------------------------------------
	戻値：カメラ基本機能
=========================================== */
ComponentCameraBase* ObjectCamera::GetCameraBase()
{
	return m_pCompCameraBase;
}


/* ========================================
	セッター(有効フラグ)関数
	-------------------------------------
	引数1：有効フラグ
=========================================== */
void ObjectCamera::SetActive(bool bActive)
{
	m_bActive = bActive;
}
