/* ========================================
	CatRobotGame/
	------------------------------------
	カメラ基本コンポーネント用cpp
	------------------------------------
	ComponentCameraBase.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentCameraBase.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ObjectCamera.h"
#include "MathUtils.h"
#include "WindowAPI.h"
#include "SettingWindowAPI.h"
#include "ColorVec3.h"

// =============== 定数定義 =======================
const float CAMERA_DEFAULT_FOVY			= DirectX::XMConvertToRadians(60.0f);	// 縦画角(拡大縮小)
const float CAMERA_DEFAULT_ASPECT		= WINDOW_INST.GetAspectRatio();			// アスペクト比
const float CAMERA_DEFAULT_NEAR			= 0.1f;		// 最小距離
const float CAMERA_DEFAULT_FAR			= 200.0f;	// 最大距離
const float CAMERA_DEFAULT_ORTHO_WIDTH	= 20.0f;	// 平行投影の幅

// UI用の定数
const float SCREEN_LEFT_EDGE	= WindowConfig::SCREEN_WIDTH  / 2.0f * -1.0;	// 画面左端
const float SCREEN_RIGHT_EDGE	= WindowConfig::SCREEN_WIDTH  / 2.0f;			// 画面右端
const float SCREEN_TOP_EDGE		= WindowConfig::SCREEN_HEIGHT / 2.0f;			// 画面上端
const float SCREEN_BOTTOM_EDGE	= WindowConfig::SCREEN_HEIGHT / 2.0f * -1.0;	// 画面下端

// 視錐台の線の色
const int LINE_NUM = 12;
const Vector3<float> LINE_COLOR = ColorVec3::RED;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
========================================== */
ComponentCameraBase::ComponentCameraBase(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderCameraBase)
	, m_pTransform(nullptr)
	, m_vLook(Vector3<float>::Forward())
	, m_vUp(Vector3<float>::Up())
	, m_fFovY(CAMERA_DEFAULT_FOVY)
	, m_fAspect(CAMERA_DEFAULT_ASPECT)
	, m_fNear(CAMERA_DEFAULT_NEAR)
	, m_fFar(CAMERA_DEFAULT_FAR)
	, m_fOrthoWidth(CAMERA_DEFAULT_ORTHO_WIDTH)
	, m_fPitch(0.0f)
	, m_pFrustumLine(nullptr)
	, m_vNearHW(0.0f, 0.0f)
	, m_vFarHW(0.0f, 0.0f)
	, m_CornersNear()
	, m_CornersFar()
	, m_bDispLine(false)
{
}


/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ComponentCameraBase::Init()
{
	// 所有者オブジェクトのTransformコンポーネントを取得
	m_pTransform	= m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pFrustumLine	= std::make_unique<ShapeLine>(LINE_NUM);

	SetNearFarClipPos();	// ニアクリップ面とファークリップ面の座標をセット
	InitNearFarClipLine();	// ニアクリップ面とファークリップ面のライン座標を初期化

#ifdef _DEBUG
	m_bDispLine = true;
#endif // _DEBUG

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentCameraBase::Update()
{
	// 所有カメラオブジェクトを取得
	ObjectCamera* pCamera = static_cast<ObjectCamera*>(m_pOwnerObj);

	// 自分の視錐台線は表示しない
	if (!pCamera->GetActive() && m_bDispLine)
	{
		SetNearFarClipPos();		// ニアクリップ面とファークリップ面の座標をセット
		UpdateNearFarClipLine();	// ニアクリップ面とファークリップ面のライン座標を更新
	}

	// 自身の座標、回転から注視点と上方向を更新
	m_vLook = m_pTransform->GetPosition() + m_pTransform->GetForwardVector();
	m_vUp = m_pTransform->GetUpVector();
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================== */
void ComponentCameraBase::Draw()
{
	// 所有カメラオブジェクトを取得
	ObjectCamera* pCamera = static_cast<ObjectCamera*>(m_pOwnerObj);

	// 自分の視錐台線は表示しない
	if (!pCamera->GetActive() && m_bDispLine)
	{
		m_pFrustumLine->Draw();	// ニアクリップ面とファークリップ面のライン描画
	}
}



/* ========================================
	カメラ移動関数
	-------------------------------------
	内容：カメラを移動する
	-------------------------------------
	引数1：移動量
========================================== */
void ComponentCameraBase::Move(Vector3<float> vMove)
{
	m_pTransform->Translate(vMove);
}

/* ========================================
	カメラ回転関数
	-------------------------------------
	内容：カメラを回転する
	-------------------------------------
	引数1：ピッチ(上下)
	引数2：ヨー(左右)
	引数3：ロール(前後)
========================================== */
void ComponentCameraBase::Rotate(float pitch, float yaw, float roll)
{
#if 1
	Quaternion qYaw = Quaternion::FromAxisAngle(Vector3<float>::Up(), MathUtils::ToRadian(yaw));
	Quaternion qPitch = Quaternion::FromAxisAngle(Vector3<float>::Right(), MathUtils::ToRadian(pitch));
	Quaternion qRoll = Quaternion::FromAxisAngle(Vector3<float>::Forward(), MathUtils::ToRadian(roll));

	m_pTransform->Rotate(qYaw * qPitch * qRoll);

	// 以下残骸として残しておく(バグがまた発生したときの為)
#elif 0
	Quaternion qYaw = Quaternion::FromAxisAngle(Vector3<float>::Up(), MathUtils::ToRadian(yaw));
	Quaternion qRot = m_pCompTransform->GetLocalRotation() * qYaw;


	Vector3<float> vAxisXRot = qRot.RotateKey(Vector3<float>::Right());

	Quaternion qPitch = Quaternion::FromAxisAngleNormalized(vAxisXRot, MathUtils::ToRadian(pitch));
	qRot = qRot * qPitch;

	m_pCompTransform->SetLocalRotation(qRot);
#else
	// 現在の回転を取得
	DirectX::XMVECTOR Rot = m_pCompTransform->GetLocalRotation().ToXMVECTOR();

	// Y軸回転
	DirectX::XMVECTOR quatY = Quaternion::FromAxisAngle(Vector3<float>::Up(), MathUtils::ToRadian(yaw)).ToXMVECTOR();
	DirectX::XMVECTOR qRot = DirectX::XMQuaternionMultiply(Rot, quatY);

	// X軸回転（現在のX軸方向を取得して、その方向を基に回転）
	DirectX::XMVECTOR vAxisX = Vector3<float>::Right().ToXMVECTOR();
	vAxisX = DirectX::XMVector3Rotate(vAxisX, qRot);

	DirectX::XMVECTOR  quatX = DirectX::XMQuaternionRotationAxis(vAxisX, MathUtils::ToRadian(pitch));
	qRot = DirectX::XMQuaternionMultiply(qRot, quatX);

	DirectX::XMFLOAT4 fRot;
	DirectX::XMStoreFloat4(&fRot, qRot);

	m_pCompTransform->SetLocalRotation(Quaternion({ fRot.x, fRot.y, fRot.z }, fRot.w));
#endif
}

/* ========================================
	カメラ回転関数(制限付き)
	-------------------------------------
	内容：カメラを回転する(制限付き)
	-------------------------------------
	引数1：ピッチ(上下)
	引数2：ヨー(左右)
	引数3：縦回転の制限
========================================== */
void ComponentCameraBase::RotateLimit(float pitch, float yaw, float LimitPitch)
{
	m_fPitch += pitch;

	// 縦回転の制限
	if (m_fPitch > LimitPitch || m_fPitch < -LimitPitch)
	{
		m_fPitch -= pitch;	// 回転を戻す
		pitch = 0.0f;		// 回転しないようにする
	}

	// カメラ回転作成
	Quaternion qYaw		= Quaternion::FromAxisAngle(Vector3<float>::Up(), MathUtils::ToRadian(yaw));				// ワールド座標系で回転
	Quaternion qPitch	= Quaternion::FromAxisAngle(m_pTransform->GetRightVector(), MathUtils::ToRadian(pitch));	// ローカル座標系で回転

	m_pTransform->Rotate(qYaw * qPitch);	// 回転
}


/* ========================================
	カメラズーム関数
	-------------------------------------
	内容：カメラの拡大、縮小を行う
	-------------------------------------
	引数1：ズーム量
========================================== */
void ComponentCameraBase::ZoomInOut(float fZoom)
{
	// ズームインアウト
	m_fFovY += fZoom;

	// 画角の制限
	if (m_fFovY < MathUtils::ToRadian(1.0f))
	{
		m_fFovY = MathUtils::ToRadian(1.0f);
	}
	else if (m_fFovY > MathUtils::ToRadian(179.0f))
	{
		m_fFovY = MathUtils::ToRadian(179.0f);
	}
}

/* ========================================
	X軸回転関数
	-------------------------------------
	内容：カメラをX軸回転する
	-------------------------------------
	引数1：回転角度
	引数2：ワールド座標系で回転するか
========================================== */
void ComponentCameraBase::RotateX(float fAngle, bool world)
{
	if (world)
		m_pTransform->Rotate(Vector3<float>::Right(), fAngle);
	else
		m_pTransform->Rotate(m_pTransform->GetRightVector(), fAngle);
}

/* ========================================
	Y軸回転関数
	-------------------------------------
	内容：カメラをY軸回転する
	-------------------------------------
	引数1：回転角度
	引数2：ワールド座標系で回転するか
========================================== */
void ComponentCameraBase::RotateY(float fAngle, bool world)
{
	if (world)
		m_pTransform->Rotate(Vector3<float>::Up(), fAngle);
	else
		m_pTransform->Rotate(m_pTransform->GetUpVector(), fAngle);
}



/* ========================================
	ビュー行列取得関数
	-------------------------------------
	内容：ビュー行列を取得する
	-------------------------------------
	戻値：ビュー行列(XMFLOAT4X4型)
=========================================== */
DirectX::XMFLOAT4X4 ComponentCameraBase::GetViewMatrix()
{
	// ビュー行列を更新
	//m_vLook = m_pTransform->GetWorldPosition() + m_pTransform->GetForwardVector();
	//m_vUp = m_pTransform->GetUpVector();

	DirectX::XMVECTOR pos	= m_pTransform->GetPosition().ToXMVECTOR();				// 自オブジェクトの位置を取得
	DirectX::XMVECTOR look	= m_vLook.ToXMVECTOR();	//(自オブジェクトの位置 + 自オブジェクトの正面ベクトル)
	DirectX::XMVECTOR up	= m_vUp.ToXMVECTOR();						// 自オブジェクトの上方向ベクトルを取得

	// ビュー行列を作成(カメラの位置、カメラの注視点、カメラの上方向を指定)
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, look, up);	

	// ビュー行列を作成(シェーダーに渡す為にXMFLOAT4X4型に変換)
	DirectX::XMFLOAT4X4 mat;

	view = DirectX::XMMatrixTranspose(view);	// 転置(シェーダーは行列の向きが違うので転置する)
	DirectX::XMStoreFloat4x4(&mat, view);		// 変換

	return mat;
}

/* ========================================
	ビュー行列(転置なし)取得関数
	-------------------------------------
	内容：転置なしのビュー行列を取得する
		　ニアファークリップをワールド座標に変換するために使用
	-------------------------------------
	戻値：ビュー行列(XMFLOAT4X4型)
=========================================== */
DirectX::XMFLOAT4X4 ComponentCameraBase::GetViewMatrixNotTransposed()
{
	// ビュー行列を更新
	/*m_vLook = m_pTransform->GetWorldPosition() + m_pTransform->GetForwardVector();
	m_vUp = m_pTransform->GetUpVector();*/

	DirectX::XMVECTOR pos = m_pTransform->GetPosition().ToXMVECTOR();				// 自オブジェクトの位置を取得
	DirectX::XMVECTOR look = m_vLook.ToXMVECTOR();	//(自オブジェクトの位置 + 自オブジェクトの正面ベクトル)
	DirectX::XMVECTOR up = m_vUp.ToXMVECTOR();						// 自オブジェクトの上方向ベクトルを取得

	// ビュー行列を作成(カメラの位置、カメラの注視点、カメラの上方向を指定)
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, look, up);

	// ビュー行列を作成(シェーダーに渡す為にXMFLOAT4X4型に変換)
	DirectX::XMFLOAT4X4 mat;

	DirectX::XMStoreFloat4x4(&mat, view);		// 変換

	return mat;
}

/* ========================================
	逆ビュー行列取得関数
	-------------------------------------
	内容：ビュー行列の逆行列を取得する
		　ビルボードのワールド行列に乗算する
	-------------------------------------
	戻値：ビュー行列の逆行列(XMMATRIX型)
=========================================== */
DirectX::XMMATRIX ComponentCameraBase::GetInvViewMatrix()
{
	DirectX::XMFLOAT4X4	fView = GetViewMatrix();					// ビュー行列取得
	DirectX::XMMATRIX	matInv = DirectX::XMLoadFloat4x4(&fView);	// ビュー行列をXMMatrixに変換

	// 転置したビュー行列を元に戻す
	matInv = DirectX::XMMatrixTranspose(matInv);
	DirectX::XMStoreFloat4x4(&fView, matInv);					// 元に戻した行列をXMFLOAT4X4に変換

	fView._41 = fView._42 = fView._43 = 0.0f;					// 位置情報を0にする(逆行列で移動情報は必要ない)

	matInv = DirectX::XMLoadFloat4x4(&fView);					// XMMatrixに変換
	matInv = DirectX::XMMatrixInverse(nullptr, matInv);			// 逆行列を求める

	return matInv;
}

/* ========================================
	プロジェクション行列取得関数
	-------------------------------------
	内容：プロジェクション行列を取得する
	-------------------------------------
	戻値：プロジェクション行列(XMFLOAT4X4型)
=========================================== */
DirectX::XMFLOAT4X4 ComponentCameraBase::GetProjectionMatrix()
{
	// プロジェクション行列を作成
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar);	

	// プロジェクション行列を作成(シェーダーに渡す為にXMFLOAT4X4型に変換)
	DirectX::XMFLOAT4X4 mat;

	proj = DirectX::XMMatrixTranspose(proj);	// 転置
	DirectX::XMStoreFloat4x4(&mat, proj);		// 変換

	return mat;
}

/* ========================================
	平行投影プロジェクション行列取得関数
	-------------------------------------
	内容：平行投影プロジェクション行列を取得する
	-------------------------------------
	戻値：平行投影プロジェクション行列(XMFLOAT4X4型)
=========================================== */
DirectX::XMFLOAT4X4 ComponentCameraBase::GetProjectionMatrixOrtho()
{
	// プロジェクション行列を作成
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(m_fOrthoWidth, m_fOrthoWidth / m_fAspect, m_fNear, m_fFar);

	// プロジェクション行列を作成(シェーダーに渡す為にXMFLOAT4X4型に変換)
	DirectX::XMFLOAT4X4 mat;

	proj = DirectX::XMMatrixTranspose(proj);	// 転置
	DirectX::XMStoreFloat4x4(&mat, proj);		// 変換

	return mat;
}

/* ========================================
	2D用ビュー行列取得関数
	-------------------------------------
	内容：2D用ビュー行列を取得する
	-------------------------------------
	戻値：2D用ビュー行列(XMFLOAT4X4型)
=========================================== */
DirectX::XMFLOAT4X4 ComponentCameraBase::GetProjectionMatrixUI()
{
	DirectX::XMFLOAT4X4 mat;

	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		SCREEN_LEFT_EDGE,		// 左端
		SCREEN_RIGHT_EDGE,		// 右端
		SCREEN_BOTTOM_EDGE,		// 下端
		SCREEN_TOP_EDGE,		// 上端
		0.0f,
		1.0f
	);
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(proj));

	return mat;
}

/* ========================================
	ニアファークリップ面座標セット関数
	-------------------------------------
	内容：ニアクリップ面とファークリップ面の
		　四角形の頂点の座標をセットする
=========================================== */
void ComponentCameraBase::SetNearFarClipPos()
{
	// ニアクリップ面
	m_vNearHW.y = 2.0f * m_fNear * tanf(m_fFovY / 2.0f);	// 高さ
	m_vNearHW.x = m_vNearHW.y * m_fAspect;					// 幅
	// ファークリップ面
	m_vFarHW.y = 2.0f * m_fFar * tanf(m_fFovY / 2.0f);		// 高さ
	m_vFarHW.x = m_vFarHW.y * m_fAspect;					// 幅

	// ニアクリップ面の4隅の座標
	m_CornersNear[0] = Vector3<float>(-m_vNearHW.x / 2.0f, m_vNearHW.y / 2.0f, m_fNear);	// 左上
	m_CornersNear[1] = Vector3<float>(m_vNearHW.x / 2.0f, m_vNearHW.y / 2.0f, m_fNear);		// 右上
	m_CornersNear[2] = Vector3<float>(-m_vNearHW.x / 2.0f, -m_vNearHW.y / 2.0f, m_fNear);	// 左下
	m_CornersNear[3] = Vector3<float>(m_vNearHW.x / 2.0f, -m_vNearHW.y / 2.0f, m_fNear);	// 右下
	// ファークリップ面の4隅の座標
	m_CornersFar[0] = Vector3<float>(-m_vFarHW.x / 2.0f, m_vFarHW.y / 2.0f, m_fFar);		// 左上
	m_CornersFar[1] = Vector3<float>(m_vFarHW.x / 2.0f, m_vFarHW.y / 2.0f, m_fFar);			// 右上
	m_CornersFar[2] = Vector3<float>(-m_vFarHW.x / 2.0f, -m_vFarHW.y / 2.0f, m_fFar);		// 左下
	m_CornersFar[3] = Vector3<float>(m_vFarHW.x / 2.0f, -m_vFarHW.y / 2.0f, m_fFar);		// 右下

	// ビュー行列の逆行列を取得
	DirectX::XMFLOAT4X4 viewMat = GetViewMatrixNotTransposed();
	DirectX::XMMATRIX invViewMat = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&viewMat));

	// ワールド座標に変換
	// ※ニア、ファークリップ面の座標をビュー行列の逆行列で変換
	// ※ワールド座標→ビュー座標に変換するためその逆を行う
	for (int i = 0; i < 4; i++)
	{
		m_CornersNear[i] = Vector3<float>::FromXMVECTOR(DirectX::XMVector3Transform(m_CornersNear[i].ToXMVECTOR(), invViewMat));
		m_CornersFar[i] = Vector3<float>::FromXMVECTOR(DirectX::XMVector3Transform(m_CornersFar[i].ToXMVECTOR(), invViewMat));
	}
	
}

/* ========================================
	ニアファークリップ面ライン初期化関数
	-------------------------------------
	内容：ニアクリップ面とファークリップ面の
		　四角形のラインを初期化する
=========================================== */
void ComponentCameraBase::InitNearFarClipLine()
{
	// ニアクリップ面四角形のライン
	m_pFrustumLine->AddLine(m_CornersNear[0], m_CornersNear[1], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[1], m_CornersNear[3], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[3], m_CornersNear[2], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[2], m_CornersNear[0], LINE_COLOR);

	// ファークリップ面四角形のライン
	m_pFrustumLine->AddLine(m_CornersFar[0], m_CornersFar[1],LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersFar[1], m_CornersFar[3],LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersFar[3], m_CornersFar[2],LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersFar[2], m_CornersFar[0],LINE_COLOR);

	// ニアクリップ面とファークリップ面を繋ぐライン(視錐台の線)
	m_pFrustumLine->AddLine(m_CornersNear[0], m_CornersFar[0], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[1], m_CornersFar[1], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[2], m_CornersFar[2], LINE_COLOR);
	m_pFrustumLine->AddLine(m_CornersNear[3], m_CornersFar[3], LINE_COLOR);
}

/* ========================================
	ニアファークリップ面ライン更新関数
	-------------------------------------
	内容：ニアクリップ面とファークリップ面の
		　四角形のラインを更新する
=========================================== */
void ComponentCameraBase::UpdateNearFarClipLine()
{
	m_pFrustumLine->UpdateLine(1, m_CornersNear[0], m_CornersNear[1], LINE_COLOR);
	m_pFrustumLine->UpdateLine(2, m_CornersNear[1], m_CornersNear[3], LINE_COLOR);
	m_pFrustumLine->UpdateLine(3, m_CornersNear[3], m_CornersNear[2], LINE_COLOR);
	m_pFrustumLine->UpdateLine(4, m_CornersNear[2], m_CornersNear[0], LINE_COLOR);

	m_pFrustumLine->UpdateLine(5, m_CornersFar[0], m_CornersFar[1], LINE_COLOR);
	m_pFrustumLine->UpdateLine(6, m_CornersFar[1], m_CornersFar[3], LINE_COLOR);
	m_pFrustumLine->UpdateLine(7, m_CornersFar[3], m_CornersFar[2], LINE_COLOR);
	m_pFrustumLine->UpdateLine(8, m_CornersFar[2], m_CornersFar[0], LINE_COLOR);

	m_pFrustumLine->UpdateLine(9,	m_CornersNear[0], m_CornersFar[0], LINE_COLOR);
	m_pFrustumLine->UpdateLine(10,	m_CornersNear[1], m_CornersFar[1], LINE_COLOR);
	m_pFrustumLine->UpdateLine(11,	m_CornersNear[2], m_CornersFar[2], LINE_COLOR);
	m_pFrustumLine->UpdateLine(12,	m_CornersNear[3], m_CornersFar[3], LINE_COLOR);
}



/* ========================================
	ゲッター(カメラ正面ベクトル)関数
	-------------------------------------
	戻値：カメラの正面ベクトル
=========================================== */
Vector3<float> ComponentCameraBase::GetForward()
{
	return m_pTransform->GetForwardVector();
}

/* ========================================
	ゲッター(カメラ右方向ベクトル)関数
	-------------------------------------
	戻値：カメラの右方向ベクトル
=========================================== */
Vector3<float> ComponentCameraBase::GetRight()
{
	return m_pTransform->GetRightVector();
}

/* ========================================
	ゲッター(カメラ上方向ベクトル)関数
	-------------------------------------
	戻値：カメラの上方向ベクトル
=========================================== */
Vector3<float> ComponentCameraBase::GetUp()
{
	return m_pTransform->GetUpVector();
}

/* ========================================
	ゲッター(カメラ注視点)関数
	-------------------------------------
	戻値：Vector3<float> カメラの注視点
=========================================== */
Vector3<float> ComponentCameraBase::GetLook()
{
	return m_vLook;
}

/* ========================================
	ゲッター(FOV)関数
	-------------------------------------
	戻値：float 垂直方向の視野角(FOV)
=========================================== */
float ComponentCameraBase::GetFovY()
{
	return m_fFovY;
}

/* ========================================
	ゲッター(アスペクト比)関数
	-------------------------------------
	戻値：float カメラのアスペクト比
=========================================== */
float ComponentCameraBase::GetAspect()
{
	return m_fAspect;
}

/* ========================================
	ゲッター(近クリップ距離)関数
	-------------------------------------
	戻値：float カメラの近クリップ距離
=========================================== */
float ComponentCameraBase::GetNear()
{
	return m_fNear;
}

/* ========================================
	ゲッター(遠クリップ距離)関数
	-------------------------------------
	戻値：float カメラの遠クリップ距離
=========================================== */
float ComponentCameraBase::GetFar()
{
	return m_fFar;
}

/* ========================================
	ゲッター(オルト幅)関数
	-------------------------------------
	戻値：float カメラの正投影幅
=========================================== */
float ComponentCameraBase::GetOrthoWidth()
{
	return m_fOrthoWidth;
}

/* ========================================
	ゲッター(ピッチ角)関数
	-------------------------------------
	戻値：float カメラのピッチ角
=========================================== */
float ComponentCameraBase::GetPitch()
{
	return m_fPitch;
}

/* ========================================
	セッター(カメラ注視点)関数
	-------------------------------------
	引数：Vector3<float> vLook - 設定する注視点
=========================================== */
void ComponentCameraBase::SetLook(Vector3<float> vLook)
{
	m_vLook = vLook;
}

/* ========================================
	セッター(カメラ上方向)関数
	-------------------------------------
	引数：Vector3<float> vUp - 設定する上方向ベクトル
=========================================== */
void ComponentCameraBase::SetUp(Vector3<float> vUp)
{
	m_vUp = vUp;
}

/* ========================================
	セッター(FOV)関数
	-------------------------------------
	引数：float fFovY - 設定する垂直方向の視野角(FOV)
=========================================== */
void ComponentCameraBase::SetFovY(float fFovY)
{
	m_fFovY = fFovY;
}

/* ========================================
	セッター(アスペクト比)関数
	-------------------------------------
	引数1：カメラのアスペクト比
=========================================== */
void ComponentCameraBase::SetAspect(float fAspect)
{
	m_fAspect = fAspect;
}

/* ========================================
	セッター(近クリップ距離)関数
	-------------------------------------
	引数1：カメラの近クリップ距離
=========================================== */
void ComponentCameraBase::SetNear(float fNear)
{
	m_fNear = fNear;
}

/* ========================================
	セッター(遠クリップ距離)関数
	-------------------------------------
	引数1：カメラの遠クリップ距離
=========================================== */
void ComponentCameraBase::SetFar(float fFar)
{
	m_fFar = fFar;
}

/* ========================================
	セッター(オルト幅)関数
	-------------------------------------
	引数1：カメラの正投影幅
=========================================== */
void ComponentCameraBase::SetOrthoWidth(float fOrthoWidth)
{
	m_fOrthoWidth = fOrthoWidth;
}

/* ========================================
	セッター(ピッチ角)関数
	-------------------------------------
	引数1：カメラのピッチ角
=========================================== */
void ComponentCameraBase::SetPitch(float fPitch)
{
	m_fPitch = fPitch;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentCameraBase::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCameraBase = Item::CreateGroup("CameraBase");


	pGroupCameraBase->AddGroupItem(Item::CreateBind("LookPos", Item::Kind::Vector, &m_vLook));
	pGroupCameraBase->AddGroupItem(Item::CreateBind("UpVector", Item::Kind::Vector, &m_vUp));

	pGroupCameraBase->AddGroupItem(Item::CreateCallBack("FovY", Item::Kind::Float, [this](bool isWrite, void* arg)
	{
		float* pDispDeg = static_cast<float*>(arg);
		if (isWrite)
		{
			m_fFovY = DirectX::XMConvertToRadians(*pDispDeg);
		}
		else
		{
			*pDispDeg = DirectX::XMConvertToDegrees(m_fFovY);
		}

	}));


	pGroupCameraBase->AddGroupItem(Item::CreateBind("Aspect", Item::Kind::Float, &m_fAspect));
	pGroupCameraBase->AddGroupItem(Item::CreateBind("Near", Item::Kind::Float, &m_fNear));
	pGroupCameraBase->AddGroupItem(Item::CreateBind("Far", Item::Kind::Float, &m_fFar));

	pGroupCameraBase->AddGroupItem(Item::CreateBind("Pitch", Item::Kind::Float, &m_fPitch));

	pGroupCameraBase->AddGroupItem(Item::CreateBind("DispLine", Item::Kind::Bool, &m_bDispLine));

	window.AddItem(pGroupCameraBase);
}

#endif // _DEBUG