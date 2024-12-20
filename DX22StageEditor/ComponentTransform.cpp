/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(座標変換)用cpp
	------------------------------------
	ComponentTransform.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentTransform.h"
#include "MathUtils.h"
#include <DirectXMath.h>
#include "ObjectBase.h"
#include "MathUtils.h"

// =============== 定数定義 =======================

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentTransform::ComponentTransform(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderTransform)
	, m_vLocalPosition(0.0f, 0.0f, 0.0f)
	, m_qLocalRotation()
	, m_vLocalScale(1.0f, 1.0f, 1.0f)
	, m_bMoveTo(false)
	, m_vMoveStartPos(0.0f, 0.0f, 0.0f)
	, m_vMoveEndPos(0.0f, 0.0f, 0.0f)
	, m_fMoveTime(0.0f)
	, m_fMoveCurrentTime(0.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ComponentTransform::Init()
{
	
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentTransform::Update()
{
	// MoveTo関数が開始されている場合
	if (m_bMoveTo)
	{
		UpdateMoveTo();
	}

	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	ワールド座標更新関数
	-------------------------------------
	内容：ワールド座標を更新する
=========================================== */
void ComponentTransform::UpdateWorldTransform()
{	
	// 親オブジェクトがある場合
	if (m_pOwnerObj->GetParentObject())
	{
		// 親オブジェクトのTransformコンポーネントを取得
		ComponentTransform* pParentTran = m_pOwnerObj->GetParentObject()->GetComponent<ComponentTransform>();

		// ローカル行列(回転、座標)を生成
		DirectX::XMMATRIX localMat =
			m_qLocalRotation.ToDirectXMatrix() *
			DirectX::XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

		// 親オブジェクトのワールド行列(回転、座標)を生成
		DirectX::XMMATRIX parentMat =
			pParentTran->GetWorldRotation().ToDirectXMatrix() *		// 回転
			DirectX::XMMatrixTranslation(							// 座標		
				pParentTran->GetWorldPosition().x, 
				pParentTran->GetWorldPosition().y, 
				pParentTran->GetWorldPosition().z);
		
		// ローカルと親の行列を掛け合わせてワールド行列(回転、座標)を生成
		DirectX::XMMATRIX matrix = localMat * parentMat;

		// 作成した行列から座標、回転を取得
		m_vWorldPosition = Vector3<float>::FromMatrix_Translation(matrix);	// 座標取得
		m_qWorldRotation = Quaternion::FromDirectXMatrix(matrix);			// 回転取得

		// ワールドスケールを計算(回転、座標と一緒に計算すると大きさが不正確になるため別で計算)
		m_vWorldScale = m_vLocalScale * pParentTran->GetWorldScale();
	}
	else
	{
		// 親オブジェクトがない場合はローカル座標をワールド座標とする
		m_vWorldPosition	= m_vLocalPosition;
		m_qWorldRotation	= m_qLocalRotation;
		m_vWorldScale		= m_vLocalScale;
	}
}

/* ========================================
	親オブジェクト解除関数
	-------------------------------------
	内容：親オブジェクトが解除された際の処理
=========================================== */
void ComponentTransform::ClearParent()
{
	// ワールド座標をローカル座標に設定(現在の状態を保持するため)
	m_vLocalPosition	= m_vWorldPosition;
	m_qLocalRotation	= m_qWorldRotation;
	m_vLocalScale		= m_vWorldScale;
}

/* ========================================
	座標移動関数
	-------------------------------------
	内容：座標を移動する
	-------------------------------------
	引数1：移動量	Vector3<float>
=========================================== */
void ComponentTransform::Translate(const Vector3<float>& translation)
{
	m_vLocalPosition += translation;
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	座標移動関数
	-------------------------------------
	内容：座標を移動する
	-------------------------------------
	引数1：移動量	x
	引数2：移動量	y
	引数3：移動量	z
=========================================== */
void ComponentTransform::Translate(float x, float y, float z)
{
	Translate(Vector3<float>(x, y, z));
}

/* ========================================
	X軸移動関数
	-------------------------------------
	内容：X軸を移動する
	-------------------------------------
	引数1：移動量	float
=========================================== */
void ComponentTransform::TranslateX(float x)
{
	Translate(Vector3<float>(x, 0.0f, 0.0f));
}

/* ========================================
	Y軸移動関数
	-------------------------------------
	内容：Y軸を移動する
	-------------------------------------
	引数1：移動量	float
=========================================== */
void ComponentTransform::TranslateY(float y)
{
	Translate(Vector3<float>(0.0f, y, 0.0f));
}

/* ========================================
	Z軸移動関数
	-------------------------------------
	内容：Z軸を移動する
	-------------------------------------
	引数1：移動量	float
=========================================== */
void ComponentTransform::TranslateZ(float z)
{
	Translate(Vector3<float>(0.0f, 0.0f, z));
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：引数の回転を加える
	-------------------------------------
	引数1：回転	Quaternion
=========================================== */
void ComponentTransform::Rotate(const Quaternion& rotation)
{
	m_qLocalRotation = m_qLocalRotation * rotation;
	m_qLocalRotation.Normalize();	// 正規化
	UpdateWorldTransform();			// ワールド座標の更新
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：引数の回転を加える
	-------------------------------------
	引数1：軸				Vector3<float>
	引数2：角度(デグリー)	float
=========================================== */
void ComponentTransform::Rotate(Vector3<float> axis, float angle)
{
	Quaternion q = Quaternion::FromAxisAngleNormalized(axis, MathUtils::ToRadian(angle));
	Rotate(q);
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：引数の回転を加える
	-------------------------------------
	引数1：回転(デグリー) Vector3<float>
=========================================== */
void ComponentTransform::Rotate(const Vector3<float>& angles)
{
	// デグリー→ラッドに変換
	Quaternion q = Quaternion::FromEulerAngle({
		MathUtils::ToRadian(angles.x),
		MathUtils::ToRadian(angles.y),
		MathUtils::ToRadian(angles.z),
		});
	Rotate(q);
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：引数の回転を加える
	-------------------------------------
	引数1：回転	float
	引数2：回転	float
	引数3：回転	float
=========================================== */
void ComponentTransform::Rotate(float x, float y, float z)
{
	// デグリー→ラッドに変換した値を渡す
	Quaternion q = Quaternion::FromEulerAngle({
		MathUtils::ToRadian(x),
		MathUtils::ToRadian(y),
		MathUtils::ToRadian(z),
		});
	this->Rotate(q);
}

/* ========================================
	X軸回転加算関数
	-------------------------------------
	内容：X軸での回転を加える
	-------------------------------------
	引数1：回転量(デグリー)
=========================================== */
void ComponentTransform::RotateX(float degAngle)
{
	this->Rotate(Vector3<float>::Right(), degAngle);
}

/* ========================================
	Y軸回転加算関数
	-------------------------------------
	内容：Y軸での回転を加える
	-------------------------------------
	引数1：回転量(デグリー)
=========================================== */
void ComponentTransform::RotateY(float degAngle)
{
	this->Rotate(Vector3<float>::Up(), degAngle);
}

/* ========================================
	Z軸回転加算関数
	-------------------------------------
	内容：Z軸での回転を加える
	-------------------------------------
	引数1：回転量(デグリー)
=========================================== */
void ComponentTransform::RotateZ(float degAngle)
{
	this->Rotate(Vector3<float>::Forward(), degAngle);
}

/* ========================================
	スケール変更関数
	-------------------------------------
	内容：引数のスケールを加える
	-------------------------------------
	引数1：スケール	Vector3<float>
=========================================== */
void ComponentTransform::Scale(const Vector3<float>& scale)
{
	m_vLocalScale *= scale;
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	スケール変更関数
	-------------------------------------
	内容：引数のスケールを加える
	-------------------------------------
	引数1：スケール	float
	引数2：スケール	float
	引数3：スケール	float
=========================================== */
void ComponentTransform::Scale(float x, float y, float z)
{
	this->Scale(Vector3<float>(x, y, z));
}

/* ========================================
	X軸スケール変更関数
	-------------------------------------
	内容：X軸のスケールを加える
	-------------------------------------
	引数1：スケール	float
=========================================== */
void ComponentTransform::ScaleX(float x)
{
	this->Scale(Vector3<float>(x, 1.0f, 1.0f));
}

/* ========================================
	Y軸スケール変更関数
	-------------------------------------
	内容：Y軸のスケールを加える
	-------------------------------------
	引数1：スケール	float
=========================================== */
void ComponentTransform::ScaleY(float y)
{
	this->Scale(Vector3<float>(1.0f, y, 1.0f));
}

/* ========================================
	Z軸スケール変更関数
	-------------------------------------
	内容：Z軸のスケールを加える
	-------------------------------------
	引数1：スケール	float
=========================================== */
void ComponentTransform::ScaleZ(float z)
{
	this->Scale(Vector3<float>(1.0f, 1.0f, z));
}

/* ========================================
	向き変更関数
	-------------------------------------
	内容：引数の座標を向く
	-------------------------------------
	引数1：向く座標	Vector3<float>
	引数2：上方向	Vector3<float>
=========================================== */
void ComponentTransform::LookAt(const Vector3<float>& target, const Vector3<float>& up)
{
	// 自オブジェクトからターゲットへのベクトル
	Vector3<float> forward = target - this->m_vLocalPosition;
	forward.Normalize();	// 正規化

	// 右方向ベクトル
	Vector3<float> right = up.Cross(forward);	// 上方向と前方向の外積
	right.Normalize();	// 正規化

	// 上方向ベクトル
	Vector3<float> newUp = forward.Cross(right);	// 前方向と右方向の外積
	newUp.Normalize();	// 正規化

	// 回転行列を生成(行方向)
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixSet(
		right.x, right.y, right.z, 0.0f,		
		newUp.x, newUp.y, newUp.z, 0.0f,	
		forward.x, forward.y, forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 回転行列からクォータニオンを生成
	SetLocalRotation(Quaternion::FromDirectXMatrix(rotationMatrix));

}

/* ========================================
	目的地移動(時間指定)関数
	-------------------------------------
	内容：指定した秒数をかけて目標座標に移動する
	-------------------------------------
	引数1：目標座標	Vector3<float>
	引数2：移動時間	float(秒)
=========================================== */
void ComponentTransform::MoveTo(const Vector3<float>& target, float fTime)
{
	m_bMoveTo			= true;			// 移動中フラグを立てる
	m_vMoveStartPos		= m_vLocalPosition;	// 移動開始座標を現在座標に
	m_vMoveEndPos		= target;		// 移動終了座標を目標座標に
	m_fMoveTime			= fTime;		// 移動時間を設定
	m_fMoveCurrentTime	= 0.0f;			// 現在の移動時間を初期化
}


/* ========================================
	目的値移動(一定速度)関数
	-------------------------------------
	内容：一定のスピードで目標座標に移動する
	-------------------------------------
	引数1：目標座標	Vector3<float>
	引数2：限界距離	float
	引数3：移動速度	float
=========================================== */
void ComponentTransform::MoveToward(const Vector3<float>& target,  float fSpeed, float fDistance)
{
	Vector3<float> fDirVec	= target - m_vLocalPosition;	// 目標までのベクトル
	float fLength			= fDirVec.Length();				// ベクトルの長さ

	// 到達していない場合
	if (fLength > fDistance)
	{
		Translate(fDirVec.GetNormalize() * fSpeed);	// 移動
	}
	else
	{
		return;	// 到達した場合は移動しない
	}

}

/* ========================================
	前方ベクトル取得関数
	-------------------------------------
	内容：ローカル座標の前方ベクトルを取得
	-------------------------------------
	戻値：前方ベクトル	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetForwardVector() const
{
	return m_qLocalRotation.GetForwardVector();
}

/* ========================================
	右方ベクトル取得関数
	-------------------------------------
	内容：ローカル座標の右方ベクトルを取得
	-------------------------------------
	戻値：右方ベクトル	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetRightVector() const
{
	return m_qLocalRotation.GetRightVector();
}

/* ========================================
	上方ベクトル取得関数
	-------------------------------------
	内容：ローカル座標の上方ベクトルを取得
	-------------------------------------
	戻値：上方ベクトル	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetUpVector() const
{
	return m_qLocalRotation.GetUpVector();
}


/* ========================================
	目的地移動移動更新(時間指定)関数
	-------------------------------------
	内容：MoveToの実際の処理。MoveTo関数が呼ばれた後、
		　Update関数内で呼び出す
=========================================== */
void ComponentTransform::UpdateMoveTo()
{

	// 経過時間を加算
	m_fMoveCurrentTime += DELTA_TIME;	// 経過時間を加算

	// 現在の座標を更新
	m_vLocalPosition = Vector3<float>::Lerp(m_vMoveStartPos, m_vMoveEndPos, m_fMoveCurrentTime / m_fMoveTime);

	// 移動終了
	if (m_fMoveCurrentTime >= m_fMoveTime)
	{
		m_vLocalPosition = m_vMoveEndPos;	// 目標座標に合わせる
		m_bMoveTo	= false;			// 移動中フラグを下げる
	}
}


/* ========================================
	ゲッター(ローカル座標)関数
	-------------------------------------
	戻値：座標	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetLocalPosition() const
{
	return m_vLocalPosition;
}

/* ========================================
	ゲッター(ワールド座標)関数
	-------------------------------------
	戻値：座標	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetWorldPosition() const
{
	return m_vWorldPosition;
}

/* ========================================
	ゲッター(ローカル回転(オイラー))関数
	-------------------------------------
	戻値：回転	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetLocalRotationEuler() const
{
	return m_qLocalRotation.ToEulerAngle();
}

/* ========================================
	ゲッター(ワールド回転(オイラー))関数
	-------------------------------------
	戻値：回転	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetWorldRotationEuler() const
{
	return m_qWorldRotation.ToEulerAngle();
}

/* ========================================
	ゲッター(ローカル回転(クォータニオン))関数
	-------------------------------------
	戻値：回転	Quaternion
=========================================== */
Quaternion ComponentTransform::GetLocalRotation() const
{
	return m_qLocalRotation;
}

/* ========================================
	ゲッター(ワールド回転(クォータニオン))関数
	-------------------------------------
	戻値：回転	Quaternion
=========================================== */
Quaternion ComponentTransform::GetWorldRotation() const
{
	return m_qWorldRotation;
}

/* ========================================
	ゲッター(ローカルスケール)関数
	-------------------------------------
	戻値：スケール	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetLocalScale() const
{
	return m_vLocalScale;
}

/* ========================================
	ゲッター(ワールドスケール)関数
	-------------------------------------
	戻値：スケール	Vector3<float>
=========================================== */
Vector3<float> ComponentTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

/* ========================================
	ゲッター(ワールド行列)関数
	-------------------------------------
	戻値：ワールド行列	DirectX::XMFLOAT4X4
=========================================== */
DirectX::XMFLOAT4X4 ComponentTransform::GetWorldMatrix() const
{
	DirectX::XMFLOAT4X4 mat;
	// ワールド行列の計算
	DirectX::XMStoreFloat4x4(&mat,			// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(				// 転地
			DirectX::XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z) *			// 拡大縮小
			m_qWorldRotation.ToDirectXMatrix() *													// クォータニオン回転
			DirectX::XMMatrixTranslation(m_vWorldPosition.x, m_vWorldPosition.y, m_vWorldPosition.z)// 移動
		));

	return mat;
}

/* ========================================
	ゲッター(ワールド行列)関数
	-------------------------------------
	内容：座標、回転、スケールを調整してワールド行列を取得
	-------------------------------------
	引数1：座標	Vector3<float>
	引数2：回転	Quaternion
	引数3：スケール	Vector3<float>
	-------------------------------------
	戻値：ワールド行列	DirectX::XMFLOAT4X4
=========================================== */
DirectX::XMFLOAT4X4 ComponentTransform::GetWorldMatrixOffset(const Vector3<float>& vPos, const Quaternion& qRot, const Vector3<float>& vScale) const
{
	Vector3<float> vOffsetPos = m_vWorldPosition + vPos;	// 座標
	Quaternion qOffsetRot = m_qWorldRotation * qRot;		// 回転
	Vector3<float> vOffsetScale = m_vWorldScale * vScale;	// スケール

	DirectX::XMFLOAT4X4 mat;
	// ワールド行列の計算
	DirectX::XMStoreFloat4x4(&mat,			// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(				// 転地
			DirectX::XMMatrixScaling(vOffsetScale.x, vOffsetScale.y, vOffsetScale.z) *			// 拡大縮小
			qOffsetRot.ToDirectXMatrix() *														// クォータニオン回転
			DirectX::XMMatrixTranslation(vOffsetPos.x, vOffsetPos.y, vOffsetPos.z)				// 移動
		));

	return mat;
}


/* ========================================
	セッター(座標)関数
	-------------------------------------
	引数1：座標	Vector3<float>
=========================================== */
void ComponentTransform::SetLocalPosition(const Vector3<float>& position)
{
	m_vLocalPosition = position;
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数1：回転	Quaternion
=========================================== */
void ComponentTransform::SetLocalRotation(const Quaternion& rotation)
{
	m_qLocalRotation = rotation;
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数1：回転軸　 Vector3<float>
	引数1：回転量(ラジアン float
=========================================== */
void ComponentTransform::SetLocalRotation(const Vector3<float> axis, float angle)
{
	m_qLocalRotation = Quaternion::FromAxisAngleNormalized(axis, angle);
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数1：回転	Vector3<float>
=========================================== */
void ComponentTransform::SetLocalRotationEuler(const Vector3<float>& angles)
{
	m_qLocalRotation = Quaternion::FromEulerAngle(angles);
	UpdateWorldTransform();	// ワールド座標の更新
}

/* ========================================
	セッター(スケール)関数
	-------------------------------------
	引数1：スケール	Vector3<float>
=========================================== */
void ComponentTransform::SetLocalScale(const Vector3<float>& scale)
{
	m_vLocalScale = scale;
	UpdateWorldTransform();	// ワールド座標の更新
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentTransform::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupTran = Item::CreateGroup("Transform");

	// 親オブジェクトがある場合
	if (m_pOwnerObj->GetParentObject())
	{
		// ワールド変数
		pGroupTran->AddGroupItem(Item::CreateValue("World", Item::Kind::Label));
		// 座標
		pGroupTran->AddGroupItem(Item::CreateBind("Position", Item::Kind::Vector, &m_vWorldPosition));
		// 回転
		// 表示だけオイラー角に変換(クォータニオンは直接入力できないため)
		pGroupTran->AddGroupItem(Item::CreateCallBack("Rotation", Item::Kind::Vector,
		[this](bool isWrite, void* arg)	// isWrite:入力があるかどうか arg:入力値
		{
			CallbackRotation(isWrite, arg, m_qWorldRotation);
		}));
		// 大きさ
		pGroupTran->AddGroupItem(Item::CreateBind("Scale", Item::Kind::Vector, &m_vWorldScale));

		// 親クラスがある場合は、ローカルとワールドの2️種類を表示する為、見出しを追加
		pGroupTran->AddGroupItem(Item::CreateValue("Local", Item::Kind::Label));
	}

	// ローカル変数
	// 座標
	pGroupTran->AddGroupItem(Item::CreateBind("Position ", Item::Kind::Vector, &m_vLocalPosition, true));
	// 回転
	pGroupTran->AddGroupItem(Item::CreateCallBack("Rotation ", Item::Kind::Vector,
		[this](bool isWrite, void* arg)
	{
		CallbackRotation(isWrite, arg, m_qLocalRotation);
	}));
	// 大きさ
	pGroupTran->AddGroupItem(Item::CreateBind("Scale ", Item::Kind::Vector, &m_vLocalScale));

	window.AddItem(pGroupTran);
}

/* ========================================
	コールバック関数(回転)
	-------------------------------------
	内容：回転のコールバック関数
	-------------------------------------
	引数1：書き込みフラグ
	引数2：引数
	引数3：回転変数
=========================================== */
void ComponentTransform::CallbackRotation(bool bIsWrite, void* arg, Quaternion& qRotation)
{
	// 入力値、表示値はデグリー、内部はラジアンで扱う
	Vector3<float>* pDispDeg = static_cast<Vector3<float>*>(arg);

	// 値変更時
	if (bIsWrite)
	{
		// クォータニオンに変換し、変数に格納
		qRotation = Quaternion::FromEulerAngle(
			Vector3<float>(
				MathUtils::ToRadian(pDispDeg->x),
				MathUtils::ToRadian(pDispDeg->y),
				MathUtils::ToRadian(pDispDeg->z)
			));
	}
	else
	{
		// クォータニオンをオイラー角に変換し、表示
		*pDispDeg = Vector3<float>(
			MathUtils::ToDegree(qRotation.ToEulerAngle().x),
			MathUtils::ToDegree(qRotation.ToEulerAngle().y),
			MathUtils::ToDegree(qRotation.ToEulerAngle().z)
		);
	}
}
#endif // _DEBUG
