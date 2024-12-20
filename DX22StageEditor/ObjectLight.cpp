/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g�pcpp
	------------------------------------
	ObjectLight.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectLight.h"
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "LightManager.h"
#include "TextureManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ObjectLight::ObjectLight(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompLightBase(nullptr)
{
	SetTag(E_ObjectTag::Light);
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏�����
========================================== */
void ObjectLight::InitLocal()
{
	LIGHT_MNG_INST.AddLight(this);

	// �R���|�[�l���g�擾
	m_pCompTransform = GetComponent<ComponentTransform>();
	m_pCompLightBase = AddComponent<ComponentLightBase>();

	ComponentBillboard* pCompBillboard = AddComponent<ComponentBillboard>();
	pCompBillboard->SetTexture(GET_TEXTURE_DATA(TEX_KEY::LIGHT_ICON));
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏I������
========================================== */
void ObjectLight::UninitLocal()
{
	LIGHT_MNG_INST.RemoveLight(this);
}

/* ========================================
	�Q�b�^�[(���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	���C�g�ʒu
=========================================== */
Vector3<float> ObjectLight::GetPosition() const
{
	return m_pCompTransform->GetWorldPosition();
}

/* ========================================
	�Q�b�^�[(���C�g�^�C�v)�֐�
	-------------------------------------
	�ߒl�FComponentLightBase::E_LightType	���C�g�^�C�v
=========================================== */
ComponentLightBase::E_LightType ObjectLight::GetLightType() const
{
	return m_pCompLightBase->GetLightType();
}

/* ========================================
	�Q�b�^�[(�f�B�t���[�Y�J���[)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�f�B�t���[�Y�J���[
=========================================== */
Vector3<float> ObjectLight::GetColor() const
{
	return m_pCompLightBase->GetColor();
}

/* ========================================
	�Q�b�^�[(���C�g����)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	���C�g����
=========================================== */
Vector3<float> ObjectLight::GetDirection() const
{
	return m_pCompTransform->GetForwardVector();
}

/* ========================================
	�Q�b�^�[(���C�g�L���͈�)�֐�
	-------------------------------------
	�ߒl�Ffloat	���C�g�L���͈�
=========================================== */
float ObjectLight::GetRange() const
{
	return m_pCompLightBase->GetRange();
}

/* ========================================
	�Q�b�^�[(�X�|�b�g���C�g�p�x)�֐�
	-------------------------------------
	�ߒl�Ffloat	�X�|�b�g���C�g�p�x
=========================================== */
float ObjectLight::GetAngle() const
{
	return m_pCompLightBase->GetAngle();
}

/* ========================================
	�Z�b�^�[(�f�B�t���[�Y�J���[)�֐�
	-------------------------------------
	����1�Fcolor	�f�B�t���[�Y�J���[
=========================================== */
void ObjectLight::SetColor(const Vector3<float>& color)
{
	m_pCompLightBase->SetColor(color);
}

/* ========================================
	�Z�b�^�[(���C�g�L���͈�)�֐�
	-------------------------------------
	����1�Frange	���C�g�L���͈�
=========================================== */
void ObjectLight::SetRange(float range)
{
	m_pCompLightBase->SetRange(range);
}

/* ========================================
	�Z�b�^�[(�X�|�b�g���C�g�p�x)�֐�
	-------------------------------------
	����1�Fangle	�X�|�b�g���C�g�p�x
=========================================== */
void ObjectLight::SetAngle(float angle)
{
	m_pCompLightBase->SetAngle(angle);
}

