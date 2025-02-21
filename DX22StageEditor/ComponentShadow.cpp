/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�ۉe)�pcpp
	------------------------------------
	ComponentShadow.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentShadow.h"
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include "ShadowManager.h"
#include "MathUtils.h"

// =============== �萔��` =======================
const float DEFAULT_RANGE = 0.6f;

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ComponentShadow::ComponentShadow(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderShadow)
	, m_fCircleSize(DEFAULT_RANGE)
	, m_vPosOffset({0.0f,0.0f,0.0f})
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ComponentShadow::Init()
{
	SHADOW_MNG_INST.AddShadow(this);
}


/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void ComponentShadow::Uninit()
{
	SHADOW_MNG_INST.RemoveShadow(this);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ComponentShadow::Update()
{
	// �I�u�W�F�N�g�̑傫���ɂ���āA�e�̑傫����ς���
	m_fCircleSize = DEFAULT_RANGE * m_pOwnerObj->GetTransform()->GetScale().x;
}

/* ========================================
	�Q�b�^�[(���W)�֐�
	-------------------------------------
	�ߒl�FvVector3<float> ���W
=========================================== */
Vector3<float> ComponentShadow::GetPosition() const
{
	return  m_pOwnerObj->GetTransform()->GetPosition() + m_vPosOffset;
}

/* ========================================
	�Q�b�^�[(�~�T�C�Y)�֐�
	-------------------------------------
	�ߒl�Ffloat �~�̑傫��
=========================================== */
float ComponentShadow::GetCircleSize() const
{
	return m_fCircleSize;
}

/* ========================================
	�Z�b�^�[(�~�T�C�Y)�֐�
	-------------------------------------
	�����Ffloat �~�̑傫��
=========================================== */
void ComponentShadow::SetCircleSize(float range)
{
	m_fCircleSize = range;
}

/* ========================================
	�Z�b�^�[(����W�����l)�֐�
	-------------------------------------
	�����FVector3<float> �����l
=========================================== */
void ComponentShadow::SetPosOffset(Vector3<float> vOffset)
{
	m_vPosOffset = vOffset;
}

