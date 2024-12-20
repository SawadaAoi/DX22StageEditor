/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(基礎)用ヘッダ
	------------------------------------
	説明：カメラオブジェクトの基礎処理を管理する
	------------------------------------
	ObjectCamera.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"
#include <DirectXMath.h>

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentCameraBase;

// =============== クラス定義 =====================
class ObjectCamera :
	public ObjectBase
{
public:
	ObjectCamera(SceneBase* pScene);

	void InitLocal() override;
	void UninitLocal() override;

	// ゲッター
	DirectX::XMFLOAT4X4 GetViewMatrix();		// ビュー行列取得関数
	DirectX::XMMATRIX GetInvViewMatrix();		// ビュー行列の逆行列取得関数

	DirectX::XMFLOAT4X4 GetProjectionMatrix();	// プロジェクション行列取得関数
	DirectX::XMFLOAT4X4 GetProjectionMatrixUI();	// 2D用プロジェクション行列取得関数
	DirectX::XMFLOAT4X4 GetProjectionMatrixOrtho();	// 平行投影プロジェクション行列取得関数

	bool GetActive();

	ComponentCameraBase* GetCameraBase();

	DEFINE_OBJECT_TYPE(ObjectCamera)	// オブジェクトの種類ID取得関数

		// セッター
		void SetActive(bool bActive);

protected:
	ComponentCameraBase* m_pCompCameraBase;	// カメラの基本機能

	bool m_bActive;	// アクティブ状態
};
