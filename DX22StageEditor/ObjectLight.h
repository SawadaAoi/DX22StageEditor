/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト用ヘッダ
	------------------------------------
	説明：ライトオブジェクトの処理
	------------------------------------
	ObjectLight.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"
#include "ComponentLightBase.h"

// =============== 前方宣言 =======================
class ComponentLightBase;
class ComponentTransform;

// =============== クラス定義 =====================
class ObjectLight :
    public ObjectBase
{
public:
    ObjectLight(SceneBase* pScene);
	virtual void InitLocal()	override;
    virtual void UninitLocal()	override;

    // ゲッター
	Vector3<float> GetPosition() const;
	ComponentLightBase::E_LightType GetLightType() const;
    Vector3<float> GetColor() const;
	Vector3<float> GetDirection() const;
	float GetRange() const;
	float GetAngle() const;

	// セッター
	void SetColor(const Vector3<float>& color);
	void SetRange(float range);
	void SetAngle(float angle);


	DEFINE_OBJECT_TYPE(ObjectLight)
protected:
    ComponentLightBase* m_pCompLightBase;

};

