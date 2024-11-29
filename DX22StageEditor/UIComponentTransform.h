/* ========================================
	DX22Base/
	------------------------------------
	UI用座標変換コンポーネント用ヘッダ
	------------------------------------
	説明：位置・回転・スケール管理を行う
	------------------------------------
	UIComponentTransform.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "UIComponentBase.h"
#include "Vector2.h"	// 2次元ベクトルクラス

// =============== クラス定義 =====================
class UIComponentTransform :
    public UIComponentBase
{
public:
	UIComponentTransform(UIObjectBase* pOwner);

	void Init() override;
	void Update() override;
	void UpdateWorldTransform();	// ワールド変換の更新
	void ClearParent();			// 親オブジェクトを削除

	// 座標変換
	void Translate(const Vector2<float>& vPos);	// 移動
	void Translate(float x, float y);			// 移動
	void TranslateX(float x);					// X軸移動
	void TranslateY(float y);					// Y軸移動

	// 回転変換
	void Rotate(float fRot);					// 回転

	// スケール変換
	void Scale(const Vector2<float>& vScale);	// スケール
	void Scale(float x, float y);				// スケール
	void ScaleX(float x);						// X軸スケール
	void ScaleY(float y);						// Y軸スケール

	// ゲッター
	Vector2<float> GetLocalPosition();	// ローカル座標の取得	
	Vector2<float> GetWorldPosition();	// ワールド座標の取得
	float GetLocalRotation();			// ローカル回転の取得
	float GetWorldRotation();			// ワールド回転の取得
	Vector2<float> GetLocalScale();		// ローカルスケールの取得
	Vector2<float> GetWorldScale();		// ワールドスケールの取得

	// セッター
	void SetLocalPosition(Vector2<float> vPos);	// ローカル座標の設定
	void SetWorldPosition(Vector2<float> vPos);	// ワールド座標の設定
	void SetLocalRotation(float fRot);			// ローカル回転の設定
	void SetWorldRotation(float fRot);			// ワールド回転の設定
	void SetLocalScale(Vector2<float> vScale);	// ローカルスケールの設定
	void SetWorldScale(Vector2<float> vScale);	// ワールドスケールの設定

	DEFINE_UI_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	// ローカル変換
	Vector2<float>	m_vLocalPosition;	// 位置
	float			m_fLocalRotation;	// 回転
	Vector2<float>	m_vLocalScale;		// スケール

	// ワールド変換
	Vector2<float>	m_vWorldPosition;	// 位置
	float			m_fWorldRotation;	// 回転
	Vector2<float>	m_vWorldScale;		// スケール
};

