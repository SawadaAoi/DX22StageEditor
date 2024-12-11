/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�|�C���g���C�g)�pcpp
	------------------------------------
	ObjectLightPoint.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectLightPoint.h"

// =============== �萔��` =======================
const float DEFAULT_RANGE = 3.0f;	// �f�t�H���g�͈̔�

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ObjectLightPoint::ObjectLightPoint(SceneBase* pScene)
	: ObjectLight(pScene)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏�����
========================================== */
void ObjectLightPoint::InitLocal()
{
	// �e�N���X�̏������֐����Ă�
	ObjectLight::InitLocal();

	// ���C�g��ނ�ݒ�
	m_pCompLightBase->SetLightType(ComponentLightBase::E_LightType::POINT);

	// �f�t�H���g�͈̔͂�ݒ�
	m_pCompLightBase->SetRange(DEFAULT_RANGE);

}
