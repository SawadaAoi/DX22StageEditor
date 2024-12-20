/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F���C�g�I�u�W�F�N�g�̏���
	------------------------------------
	ObjectLight.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"
#include "ComponentLightBase.h"

// =============== �O���錾 =======================
class ComponentLightBase;
class ComponentTransform;

// =============== �N���X��` =====================
class ObjectLight :
    public ObjectBase
{
public:
    ObjectLight(SceneBase* pScene);
	virtual void InitLocal()	override;
    virtual void UninitLocal()	override;

    // �Q�b�^�[
	Vector3<float> GetPosition() const;
	ComponentLightBase::E_LightType GetLightType() const;
    Vector3<float> GetColor() const;
	Vector3<float> GetDirection() const;
	float GetRange() const;
	float GetAngle() const;

	// �Z�b�^�[
	void SetColor(const Vector3<float>& color);
	void SetRange(float range);
	void SetAngle(float angle);


	DEFINE_OBJECT_TYPE(ObjectLight)
protected:
    ComponentLightBase* m_pCompLightBase;

};

