/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�X�|�b�g�C�g)�pcpp
	------------------------------------
	ObjectLightSpot.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectLightSpot.h"
#include "MathUtils.h"

// =============== �萔��` =======================
const float DEFAULT_RANGE = 3.0f;	// �f�t�H���g�͈̔�
const float DEFAULT_ANGLE = MathUtils::ToRadian(30.0f);	// �f�t�H���g�̊p�x


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ObjectLightSpot::ObjectLightSpot(SceneBase* pScene)
	: ObjectLight(pScene)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏�����
========================================== */
void ObjectLightSpot::InitLocal()
{
	// �e�N���X�̏������֐����Ă�
	ObjectLight::InitLocal();

	// ���C�g��ނ�ݒ�
	m_pCompLightBase->SetLightType(ComponentLightBase::E_LightType::SPOT);
	// �f�t�H���g�͈̔͂�ݒ�
	m_pCompLightBase->SetRange(DEFAULT_RANGE);
	// �f�t�H���g�̊p�x��ݒ�
	m_pCompLightBase->SetAngle(DEFAULT_ANGLE);
}
