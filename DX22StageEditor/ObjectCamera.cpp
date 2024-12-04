/* ========================================
	DX22Base/
	------------------------------------
	カメラオブジェクト用cpp
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
	, m_pCameraBase(nullptr)
	, m_pTransform(nullptr)
	, m_bActive(false)
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：コンポーネントの追加
=========================================== */
void ObjectCamera::InitLocal()
{
	// コンポーネントの追加
	m_pCameraBase = AddComponent<ComponentCameraBase>();	// カメラの基本機能
	m_pTransform = GetComponent<ComponentTransform>();		// 座標、回転、拡大縮小(初めからセット済み)

	m_pTransform->SetLocalPosition(Vector3(0.0f, 1.0f, -5.0f));

	AddComponent<ComponentCameraDebug>();	// デバッグカメラ

	CAMERA_MNG_INST.AddCamera(this);	// カメラマネージャにカメラを追加


	AddComponent<ComponentBillboard>();	// ビルボード
	GetComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::CAMERA_ICON));	// テクスチャ
}

/* ========================================
	終了関数
	-------------------------------------
	内容：カメラの削除
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
	return m_pCameraBase->GetViewMatrix();
}

/* ========================================
	ゲッター(ビュー行列の逆行列)関数
	-------------------------------------
	戻値：ビュー行列の逆行列
=========================================== */
DirectX::XMMATRIX ObjectCamera::GetInvViewMatrix()
{
	return m_pCameraBase->GetInvViewMatrix();
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
		return m_pCameraBase->GetProjectionMatrixOrtho();
	}
	else
	{
		return m_pCameraBase->GetProjectionMatrix();
	}

#else
	return m_pCameraBase->GetProjectionMatrix();

#endif // _DEBUG

}

/* ========================================
	ゲッター(2D用プロジェクション行列)関数
	-------------------------------------
	戻値：2D用プロジェクション行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixUI()
{
	return m_pCameraBase->GetProjectionMatrixUI();
}

/* ========================================
	ゲッター(平行投影プロジェクション行列)関数
	-------------------------------------
	戻値：平行投影プロジェクション行列
=========================================== */
DirectX::XMFLOAT4X4 ObjectCamera::GetProjectionMatrixOrtho()
{
	return m_pCameraBase->GetProjectionMatrixOrtho();
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
	セッター(有効フラグ)関数
	-------------------------------------
	引数1：有効フラグ
=========================================== */
void ObjectCamera::SetActive(bool bActive)
{
	m_bActive = bActive;
}
