/* ========================================
	DX22Base/
	------------------------------------
	UI用座標変換コンポーネント用cpp
	------------------------------------
	UIComponentTransform.cpp
========================================== */

// =============== インクルード =====================
#include "UIComponentTransform.h"
#include "UIObjectBase.h"
#include "Vector3.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所持オブジェクト
=========================================== */
UIComponentTransform::UIComponentTransform(UIObjectBase* pOwner)
	: UIComponentBase(pOwner, UIOrderTransform)
	, m_vLocalPosition(0.0f, 0.0f)
	, m_fLocalRotation(0.0f)
	, m_vLocalScale(1.0f, 1.0f)
	, m_vWorldPosition(0.0f, 0.0f)
	, m_fWorldRotation(0.0f)
	, m_vWorldScale(1.0f, 1.0f)
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void UIComponentTransform::Init()
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void UIComponentTransform::Update()
{
	UpdateWorldTransform();
}

/* ========================================
	ワールド座標更新関数
	-------------------------------------
	内容：ワールド座標を更新する
=========================================== */
void UIComponentTransform::UpdateWorldTransform()
{
	// 親オブジェクトがある場合
	if (m_pOwnerObj->GetParentObject())
	{
		UIComponentTransform* pParentTran = m_pOwnerObj->GetParentObject()->GetComponent<UIComponentTransform>();

		// ローカル行列(回転、座標)を生成
		DirectX::XMMATRIX localMat =
			DirectX::XMMatrixRotationZ(m_fLocalRotation) *
			DirectX::XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, 0.0f);

		// 親オブジェクトのワールド行列(回転、座標)を生成
		DirectX::XMMATRIX parentMat =
			DirectX::XMMatrixRotationZ(pParentTran->GetWorldRotation()) *
			DirectX::XMMatrixTranslation(
				pParentTran->GetWorldPosition().x, 
				pParentTran->GetWorldPosition().y,
				0.0f);

		// 自身のローカル行列(回転、座標)と親オブジェクトのワールド行列(回転、座標)を合成
		DirectX::XMMATRIX matrix = localMat * parentMat;

		// 合成行列から座標、回転を取得
		Vector3<float> vPos	= Vector3<float>::FromMatrix_Translation(matrix);	// 座標取得
		Vector3<float> vRot	= Vector3<float>::FromMatrix_RotationEuler(matrix);	// 回転取得
		m_vWorldPosition.x	= vPos.x;
		m_vWorldPosition.y	= vPos.y;
		m_fWorldRotation	= vRot.z;

		// ワールドスケールを計算(回転、座標と一緒に計算すると大きさが不正確になるため別で計算)
		m_vWorldScale = m_vLocalScale * pParentTran->GetWorldScale();
	}
	else
	{
		// 親オブジェクトがない場合はローカル座標をワールド座標に設定
		m_vWorldPosition	= m_vLocalPosition;
		m_fWorldRotation	= m_fLocalRotation;
		m_vWorldScale		= m_vLocalScale;
	}
}

/* ========================================
	親オブジェクト解除関数
	-------------------------------------
	内容：親オブジェクトが解除された際の処理
=========================================== */
void UIComponentTransform::ClearParent()
{
	// ワールド座標をローカル座標に設定(現在の状態を保持するため)
	m_vLocalPosition	= m_vWorldPosition;
	m_fLocalRotation	= m_fWorldRotation;
	m_vLocalScale		= m_vWorldScale;
}

/* ========================================
	座標移動関数
	-------------------------------------
	内容：座標を移動する
	-------------------------------------
	引数1：移動量	Vector2<float>
=========================================== */
void UIComponentTransform::Translate(const Vector2<float>& vPos)
{
	m_vLocalPosition += vPos;
	UpdateWorldTransform();
}

/* ========================================
	座標移動関数
	-------------------------------------
	内容：座標を移動する
	-------------------------------------
	引数1：移動量	X座標
	引数2：移動量	Y座標
=========================================== */
void UIComponentTransform::Translate(float x, float y)
{
	Translate(Vector2<float>(x, y));
}

/* ========================================
	X座標移動関数
	-------------------------------------
	内容：X座標を移動する
	-------------------------------------
	引数1：移動量	X座標
=========================================== */
void UIComponentTransform::TranslateX(float x)
{
	Translate(Vector2<float>(x, 0.0f));
}

/* ========================================
	Y座標移動関数
	-------------------------------------
	内容：Y座標を移動する
	-------------------------------------
	引数1：移動量	Y座標
=========================================== */
void UIComponentTransform::TranslateY(float y)
{
	Translate(Vector2<float>(0.0f, y));
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：引数の回転を加える
	-------------------------------------
	引数1：回転量(デグリー) float
=========================================== */
void UIComponentTransform::Rotate(float fRot)
{
	m_fLocalRotation += MathUtils::ToRadian(fRot);
	UpdateWorldTransform();
}

/* ========================================
	スケール変更関数
	-------------------------------------
	内容：スケールを変更する
	-------------------------------------
	引数1：スケール	Vector2<float>
=========================================== */
void UIComponentTransform::Scale(const Vector2<float>& vScale)
{
	m_vLocalScale *= vScale;
	UpdateWorldTransform();
}

/* ========================================
	スケール変更関数
	-------------------------------------
	内容：スケールを変更する
	-------------------------------------
	引数1：スケール	X座標
	引数2：スケール	Y座標
=========================================== */
void UIComponentTransform::Scale(float x, float y)
{
	Scale(Vector2<float>(x, y));
	UpdateWorldTransform();
}

/* ========================================
	X座標スケール変更関数
	-------------------------------------
	内容：X座標スケールを変更する
	-------------------------------------
	引数1：スケール	X座標
=========================================== */
void UIComponentTransform::ScaleX(float x)
{
	Scale(Vector2<float>(x, 1.0f));
	UpdateWorldTransform();
}

/* ========================================
	Y座標スケール変更関数
	-------------------------------------
	内容：Y座標スケールを変更する
	-------------------------------------
	引数1：スケール	Y座標
=========================================== */
void UIComponentTransform::ScaleY(float y)
{
	Scale(Vector2<float>(1.0f, y));
	UpdateWorldTransform();
}


/* ========================================
	ゲッター(ローカル座標)関数
	-------------------------------------
	戻値：座標	Vector3<float>
=========================================== */
Vector2<float> UIComponentTransform::GetLocalPosition()
{
	return m_vLocalPosition;
}

/* ========================================
	ゲッター(ワールド座標)関数
	-------------------------------------
	戻値：座標	Vector3<float>
=========================================== */
Vector2<float> UIComponentTransform::GetWorldPosition()
{
	return m_vWorldPosition;
}

/* ========================================
	ゲッター(ローカル回転)関数
	-------------------------------------
	戻値：回転	float
=========================================== */
float UIComponentTransform::GetLocalRotation()
{
	return m_fLocalRotation;
}

/* ========================================
	ゲッター(ワールド回転)関数
	-------------------------------------
	戻値：回転	float
=========================================== */
float UIComponentTransform::GetWorldRotation()
{
	return m_fWorldRotation;
}

/* ========================================
	ゲッター(ローカルスケール)関数
	-------------------------------------
	戻値：スケール	Vector3<float>
=========================================== */
Vector2<float> UIComponentTransform::GetLocalScale()
{
	return m_vLocalScale;
}

/* ========================================
	ゲッター(ワールドスケール)関数
	-------------------------------------
	戻値：スケール	Vector3<float>
=========================================== */
Vector2<float> UIComponentTransform::GetWorldScale()
{
	return m_vWorldScale;
}

/* ========================================
	セッター(ローカル座標)関数
	-------------------------------------
	引数：座標	Vector3<float>
=========================================== */
void UIComponentTransform::SetLocalPosition(Vector2<float> vPos)
{
	m_vLocalPosition = vPos;
}

/* ========================================
	セッター(ワールド座標)関数
	-------------------------------------
	引数：座標	Vector3<float>
=========================================== */
void UIComponentTransform::SetWorldPosition(Vector2<float> vPos)
{
	m_vWorldPosition = vPos;
}

/* ========================================
	セッター(ローカル回転)関数
	-------------------------------------
	引数：回転	float
=========================================== */
void UIComponentTransform::SetLocalRotation(float fRot)
{
	m_fLocalRotation = fRot;
}

/* ========================================
	セッター(ワールド回転)関数
	-------------------------------------
	引数：回転	float
=========================================== */
void UIComponentTransform::SetWorldRotation(float fRot)
{
	m_fWorldRotation = fRot;
}

/* ========================================
	セッター(ローカルスケール)関数
	-------------------------------------
	引数：スケール	Vector3<float>
=========================================== */
void UIComponentTransform::SetLocalScale(Vector2<float> vScale)
{
	m_vLocalScale = vScale;
}

/* ========================================
	セッター(ワールドスケール)関数
	-------------------------------------
	引数：スケール	Vector3<float>
=========================================== */
void UIComponentTransform::SetWorldScale(Vector2<float> vScale)
{
	m_vWorldScale = vScale;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void UIComponentTransform::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupUITran = Item::CreateGroup("UI_Transform");

	// 親オブジェクトがある場合
	if (m_pOwnerObj->GetParentObject())
	{
		// ワールド変数
		pGroupUITran->AddGroupItem(Item::CreateValue("World", Item::Kind::Label));
		// 座標
		pGroupUITran->AddGroupItem(Item::CreateBind("Position", Item::Kind::Vector2, &m_vWorldPosition));
		// 回転
		// 表示だけオイラー角に変換(クォータニオンは直接入力できないため)
		pGroupUITran->AddGroupItem(Item::CreateCallBack("Rotation", Item::Kind::Float,
			[this](bool isWrite, void* arg)	// isWrite:入力があるかどうか arg:入力値
		{
			float* fRotDeg = static_cast<float*>(arg);
			// 値変更時
			if (isWrite)
				m_fWorldRotation = MathUtils::ToRadian(*fRotDeg);
			else
				*fRotDeg = MathUtils::ToDegree(m_fWorldRotation);
		}));
		// 大きさ
		pGroupUITran->AddGroupItem(Item::CreateBind("Scale", Item::Kind::Vector2, &m_vWorldScale));

		// 親クラスがある場合は、ローカルとワールドの2️種類を表示する為、見出しを追加
		pGroupUITran->AddGroupItem(Item::CreateValue("Local", Item::Kind::Label));
	}

	// ローカル変数
	// 座標
	pGroupUITran->AddGroupItem(Item::CreateBind("Position ", Item::Kind::Vector2, &m_vLocalPosition, true));
	// 回転
	pGroupUITran->AddGroupItem(Item::CreateCallBack("Rotation ", Item::Kind::Float,
		[this](bool isWrite, void* arg)
	{
		float* fRotDeg = static_cast<float*>(arg);
		// 値変更時
		if (isWrite)
			m_fLocalRotation = MathUtils::ToRadian(*fRotDeg);
		else
			*fRotDeg = MathUtils::ToDegree(m_fLocalRotation);
	}));
	// 大きさ
	pGroupUITran->AddGroupItem(Item::CreateBind("Scale ", Item::Kind::Vector2, &m_vLocalScale));

	window.AddItem(pGroupUITran);
}
#endif // _DEBUG
