/* ========================================
	DX22Base/
	------------------------------------
	���C�g�I�u�W�F�N�g(�������C�g)�pcpp
	------------------------------------
	ObjectLightDirectional.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectLightDirectional.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ObjectLightDirectional::ObjectLightDirectional(SceneBase* pScene)
	: ObjectLight(pScene)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏�����
========================================== */
void ObjectLightDirectional::InitLocal()
{
	// �e�N���X�̏������֐����Ă�
	ObjectLight::InitLocal();	

	// ���C�g��ނ�ݒ�
	m_pCompLightBase->SetLightType(ComponentLightBase::E_LightType::DIRECTIONAL);
}
