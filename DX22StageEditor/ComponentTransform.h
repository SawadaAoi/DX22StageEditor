/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(座標変換)用ヘッダ
	------------------------------------
	説明：位置・回転・スケール管理を行う
	------------------------------------
	ComponentTransform.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"
#include "Vector3.h"
#include "Quaternion.h"	// 回転


// =============== クラス定義 =====================
class ComponentTransform :
    public ComponentBase
{
public:
	ComponentTransform(ObjectBase* pOwner);

    void Init() override;
	void Update() override;
	void UpdateWorldTransform();	// ワールド変数の更新
	void ClearParent();			// 親オブジェクトを削除

	// 座標変換
	void Translate(const Vector3<float>& translation);
	void Translate(float x, float y, float z);
	void TranslateX(float x);
	void TranslateY(float y);
	void TranslateZ(float z);

	// 回転変換
	void Rotate(const Quaternion& rotation);
	void Rotate(Vector3<float> axis, float angle);	
	void Rotate(const Vector3<float>& angles);
	void Rotate(float x, float y, float z);
	void RotateX(float degAngle);
	void RotateY(float degAngle);
	void RotateZ(float degAngle);

	// スケール変換
	void Scale(const Vector3<float>& scale);
	void Scale(float x, float y, float z);
	void ScaleX(float x);	
	void ScaleY(float y);
	void ScaleZ(float z);

	// その他便利関数
	void LookAt(const Vector3<float>& target, const Vector3<float>& up = Vector3<float>::Up());
	void MoveTo(const Vector3<float>& target, float fTime);
	void MoveToward(const Vector3<float>& target, float fSpeed, float fDistance = 0.0f);
	Vector3<float> GetForwardVector() const;
	Vector3<float> GetRightVector() const;
	Vector3<float> GetUpVector() const;

    // ゲッター
    Vector3<float> GetLocalPosition() const;
	Vector3<float> GetWorldPosition() const;
	Vector3<float> GetLocalRotationEuler() const;
	Vector3<float> GetWorldRotationEuler() const;
	Quaternion GetLocalRotation() const;
	Quaternion GetWorldRotation() const;
	Vector3<float> GetLocalScale() const;
	Vector3<float> GetWorldScale() const;

	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	DirectX::XMFLOAT4X4 GetWorldMatrixOffset(
		const Vector3<float>& vPos = Vector3<float>::Zero(), 
		const Quaternion& qRot = Quaternion(), 
		const Vector3<float>& vScale = {1.0f,1.0f,1.0f}) const;

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

	// セッター
	void SetLocalPosition(const Vector3<float>& position);
	void SetLocalRotation(const Quaternion& rotation);
	void SetLocalRotation(const Vector3<float> axis, float degAngle);
	void SetLocalRotationEuler(const Vector3<float>& angles);
	void SetLocalScale(const Vector3<float>& scale);

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	void CallbackRotation(bool bIsWrite, void* arg, Quaternion& qRotation);
#endif // _DEBUG

private:
	void UpdateMoveTo();	// MoveToの更新
private:
	// ローカル
    Vector3<float>	m_vLocalPosition;	// 座標
	Quaternion		m_qLocalRotation;	// 回転
	Vector3<float>	m_vLocalScale;		// スケール
	
	// ワールド(親オブジェクトを考慮した座標)
	Vector3<float>	m_vWorldPosition;	// 座標
	Quaternion		m_qWorldRotation;	// 回転
	Vector3<float>	m_vWorldScale;		// スケール

	// MoveTo用
	Vector3<float>	m_vMoveStartPos;	// 移動開始座標
	Vector3<float>	m_vMoveEndPos;		// 移動終了座標
	float			m_fMoveTime;		// 移動時間
	float			m_fMoveCurrentTime;	// 現在の移動時間
	bool			m_bMoveTo;			// 移動中かどうか
};

