/* ========================================
	CatRobotGame/
	------------------------------------
	カメラ基本コンポーネント用ヘッダ
	------------------------------------
	説明：カメラの基本機能を提供する
	------------------------------------
	ComponentCameraBase.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include "Vector3.h"		// 3次元ベクトルクラス
#include "Vector2.h"
#include "ShapeLine.h"
#include <memory>

// =============== クラス定義 =====================
class ComponentCameraBase :
    public ComponentBase
{
public:
	ComponentCameraBase(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move(Vector3<float> vMove);							// カメラ移動関数
	void Rotate(float pitch, float yaw, float roll);			// カメラ回転関数
	void RotateLimit(float pitch, float yaw, float LimitPitch);	// カメラ回転関数(制限付き)
	void ZoomInOut(float fZoom);								// ズームインアウト関数
	void RotateX(float fAngle, bool world);						// X軸回転関数
	void RotateY(float fAngle, bool world);						// Y軸回転関数


	DirectX::XMFLOAT4X4 GetViewMatrix();					// ビュー行列取得関数
	DirectX::XMFLOAT4X4 GetViewMatrixNotTransposed();		// ビュー行列取得関数
	DirectX::XMMATRIX GetInvViewMatrix();					// ビュー行列の逆行列取得関数

	DirectX::XMFLOAT4X4 GetProjectionMatrix();			// プロジェクション行列取得関数
	DirectX::XMFLOAT4X4 GetProjectionMatrixOrtho();		// 平行投影プロジェクション行列取得関数
	DirectX::XMFLOAT4X4 GetProjectionMatrixUI();		// 2D用ビュー行列取得関数

	// ゲッター
	Vector3<float> GetForward();	// カメラの前方向取得関数
	Vector3<float> GetRight();		// カメラの右方向取得関数
	Vector3<float> GetUp();			// カメラの上方向取得関数
	Vector3<float> GetLook();		// カメラの注視点取得関数
	float GetFovY();				// 縦方向の画角取得関数
	float GetAspect();				// アスペクト比取得関数
	float GetNear();				// ニアクリップ面取得関数
	float GetFar();					// ファークリップ面取得関数
	float GetOrthoWidth();			// 平行投影の幅取得関数
	float GetPitch();				// X軸回転角度取得関数


	// セッター
	void SetLook(Vector3<float> vLook);	// 注視点設定関数
	void SetUp(Vector3<float> vUp);		// 上方向設定関数
	void SetFovY(float fFovY);			// 縦方向の画角設定関数
	void SetAspect(float fAspect);		// アスペクト比設定関数
	void SetNear(float fNear);			// ニアクリップ面設定関数
	void SetFar(float fFar);			// ファークリップ面設定関数
	void SetOrthoWidth(float fOrthoWidth);	// 平行投影の幅設定関数
	void SetPitch(float fPitch);		// X軸回転角度設定関数


	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void SetNearFarClipPos();	// ニアクリップ面とファークリップ面のライン描画関数
	void InitNearFarClipLine();		// ニアクリップ面とファークリップ面のライン描画関数
	void UpdateNearFarClipLine();	// ニアクリップ面とファークリップ面のライン描画関数
private:

	class ComponentTransform* m_pTransform;	// このコンポーネントを所持するオブジェクトのトランスフォーム

	// ビュー行列を作成する値
	Vector3<float> m_vLook;		// カメラの注視点
	Vector3<float> m_vUp;		// カメラの上方向

	// プロジェクション行列を作成する値
	float m_fFovY;			// 縦方向の画角
	float m_fAspect;		// アスペクト比
	float m_fNear;			// カメラとの最小距離
	float m_fFar;			// カメラとの最大距離
	float m_fOrthoWidth;	// 平行投影の幅
	float m_fPitch;			// X軸回転角度

	std::unique_ptr<ShapeLine> m_pFrustumLine;	// カメラ描画範囲の視錐台ライン
	bool m_bDispLine;							// ライン表示フラグ

	Vector2<float> m_vNearHW;	// ニアクリップ面の幅と高さ
	Vector2<float> m_vFarHW;	// ファークリップ面の幅と高さ

	Vector3<float> m_CornersNear[4];	// ニアクリップ面の4隅の座標
	Vector3<float> m_CornersFar[4];	// ファークリップ面の4隅の座標

};

