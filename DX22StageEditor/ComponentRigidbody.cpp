/* ========================================
	DX22Base/
	------------------------------------
	���������R���|�[�l���g�pcpp
	------------------------------------
	ComponentRigidbody.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "ComponentGroundRaycast.h"
#include "ComponentCollisionBase.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionSphere.h"
#include "ObjectBase.h"


// =============== �萔��` =======================
const float GRAVITY_FORCE		= -9.8f;	// �d��
const float GROUND_CHECK_DELAY	= 0.1f;		// �n�ʐڐG����̒x������(

// ��R�����l
const float DEFAULT_AIR_DRAG	= 0.1f;  // ��C��R�̌W��
const float DEFAULT_GROUND_DRAG = 0.5f;  // �n�ʏ�ł̖��C�i�����j�W��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ComponentRigidbody::ComponentRigidbody(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderRigidBody)
	, m_pCompTransform(nullptr)
	, m_pCompGroundRay(nullptr)
	, m_pCompCollisionBase(nullptr)
	, m_vVelocity(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_vAcceleration(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_fMass(1.0f)
	, m_bUseGravity(true)
	, m_bIsGround(false)
	, m_fGroundCheckDelay(0.0f)
	, m_fAirDrag(DEFAULT_AIR_DRAG)
	, m_fGroundDrag(DEFAULT_GROUND_DRAG)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ComponentRigidbody::Init()
{
	m_pCompTransform		= m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pCompGroundRay		= m_pOwnerObj->GetComponent<ComponentGroundRaycast>();

	SetCollisionComponent();	
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ComponentRigidbody::Update()
{
	// �d�͂��g�p����ꍇ�͏d�͂����Z
	if (m_bUseGravity)
	{
		m_vAcceleration += Vector3<float>(0.0f, GRAVITY_FORCE, 0.0f);
	}

	// ��R�͂̌v�Z�i��C���ƒn�ʂňقȂ�W�����g�p�j
	float dragCoefficient		= m_bIsGround ? m_fGroundDrag : m_fAirDrag;
	Vector3<float> fDragForce	= (m_vVelocity * -1.0f) * dragCoefficient;

	// ��R�͂������x�ɔ��f
	m_vAcceleration += fDragForce / m_fMass;

	m_vVelocity		 += m_vAcceleration * DELTA_TIME;		// �����x�𑬓x�ɉ��Z
	m_pCompTransform->Translate(m_vVelocity * DELTA_TIME);	// ���x�����W�ɉ��Z


	// �Փ˔��肪����ꍇ
	if (m_pCompCollisionBase)
	{
		ResolveOverlapCollision();	// �Փˎ��̂߂荞�݉���
	}
	else
	{
		SetCollisionComponent();
	}


	// �n�ʐڐG���肪����ꍇ
	if (m_pCompGroundRay)
	{
		CheckRaycastGround();	// �n�ʐڐG����
		
	}
	else
	{
		m_pCompGroundRay = m_pOwnerObj->GetComponent<ComponentGroundRaycast>();
	}


	//DebugConsole::Printf("Velocity: %f, %f, %f", m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	//DebugConsole::Printf("HitFlg: %d", m_pGroundRaycast->GetHitFlg());

#ifdef _DEBUG
	// �����x�\���p(���t���[�����Z�b�g�����̂ŕʕϐ��ɕۑ�)
	m_vDispAccel = m_vAcceleration;

#endif // _DEBUG
	m_vAcceleration = Vector3<float>::Zero();			// �����x�����Z�b�g

}

/* ========================================
	�͉��Z�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�ɗ͂����Z����
	-------------------------------------
	����1�F�͂̃x�N�g��
	����2�F�͂̎��
========================================= */
void ComponentRigidbody::AddForce(const Vector3<float>& vForce, E_ForceMode eMode)
{
	// ���ʂ�0�̏ꍇ�͌v�Z���Ȃ�
	if (m_fMass == 0.0f) return;

	switch (eMode)
	{
	// �͂��p���I�ɉ��Z
	case E_ForceMode::FORCE:
		m_vAcceleration += vForce / m_fMass;
		break;
	// �u�ԓI�ȗ͂����Z
	case E_ForceMode::IMPULSE:
		m_vVelocity += vForce / m_fMass;
		break;
	}

	// �W�����v�̏ꍇ�͒n�ʐڐG����̒x�����Ԃ�ݒ�
	if (eMode == E_ForceMode::IMPULSE && vForce.y > 0.0f && m_pCompGroundRay)
	{
		m_fGroundCheckDelay = GROUND_CHECK_DELAY;	
	}
}

/* ========================================
	�n�ʐڐG����֐�
	-------------------------------------
	���e�F�n�ʂɎ��g�������C���ڐG���Ă��邩�ǂ����𔻒�
========================================= */
void ComponentRigidbody::CheckRaycastGround()
{
	// �n�ʐڐG����̒x��
	// ������ɗ͂������Ă����t���[���ł܂��ڒn���肪�L���ׁ̈A�㑬�x��0���Z�b�g����Ă��܂��̂�
	// �͂������Ă����莞�Ԃ͐ڒn����𖳌��ɂ���
	if (m_fGroundCheckDelay > 0.0f)
	{
		m_fGroundCheckDelay -= DELTA_TIME;
	}
	// �n�ʂɐڐG���Ă���ꍇ�͑��x��0�ɂ���
	else if (m_pCompGroundRay->GetHitFlg())
	{
		m_vVelocity.y = 0.0f;
		m_bIsGround = true;
	}
	else
	{
		m_bIsGround = false;
	}
}

/* ========================================
	�߂荞�݉����֐�
	-------------------------------------
	���e�F�Փˎ��̂߂荞�݂����ɖ߂�
========================================= */
void ComponentRigidbody::ResolveOverlapCollision()
{
	using T_MTV = ComponentCollisionOBB::T_MTV;

	// �߂荞�ݏ��z����擾
	std::vector<T_MTV> mtvs = m_pCompCollisionBase->GetMtvs();

	// �I�u�W�F�N�g�������[�v
	for (const T_MTV& mtv : mtvs)
	{
		if (!mtv.bIsCol) continue;										// �Փ˂��Ă��Ȃ��ꍇ�͎��̃I�u�W�F�N�g��
		if (mtv.bIsTrigger) continue;									// �g���K�[�̏ꍇ�͎��̃I�u�W�F�N�g��
		if (m_pCompCollisionBase->GetTrigger()) continue;				// �g���K�[�̏ꍇ�͎��̃I�u�W�F�N�g��
		// ���C�Փ˒n�ʂ͖���
		// ���n�ʃu���b�N���΂߂̏ꍇ�ɁA�߂荞�݉���������ƁA���藎����̂�h������
		if (mtv.sName == m_pCompGroundRay->GetHitObjName()) continue;	

		// �߂荞�ݕ���
		Vector3<float> vReturnDir	= mtv.vAxis * -1.0f;	// �Փˎ��̔��Ε���
		float fOverlapDis			= mtv.fOverlap;			// �d�Ȃ��

		m_pCompTransform->Translate(vReturnDir * fOverlapDis);

		// �Փˎ��ɉ��������x�̌���
		float fDot = m_vVelocity.Dot(vReturnDir);	// �ړ��������ǂꂾ���߂荞�ݕ����ɉ����Ă��邩

		// �ړ������Ƃ߂荞�ݕ������t�̏ꍇ
		if (fDot < 0.0f)
		{
			m_vVelocity -= (vReturnDir * fDot);	// �߂荞�ݕ����ɑ΂��鑬�x�̐��������Z
		}
	}
}

/* ========================================
	�Փ˃R���|�[�l���g�ݒ�֐�
	-------------------------------------
	���e�F���L�҃I�u�W�F�N�g�ɐݒ肳��Ă���R���W�����R���|�[�l���g���擾
		�@�`��ɂ���ĈقȂ�R���W�����R���|�[�l���g���擾
========================================= */
void ComponentRigidbody::SetCollisionComponent()
{
	// AABB
	if (m_pOwnerObj->GetComponent<ComponentCollisionAABB>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionAABB>();
	}
	// OBB
	else if (m_pOwnerObj->GetComponent<ComponentCollisionOBB>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionOBB>();
	}
	// ��
	else if (m_pOwnerObj->GetComponent<ComponentCollisionSphere>())
	{
		m_pCompCollisionBase = m_pOwnerObj->GetComponent<ComponentCollisionSphere>();
	}
}


/* ========================================
	�Z�b�^�[(���x)�֐�
	-------------------------------------
	�����FVector3<float>	���x
=========================================== */
void ComponentRigidbody::SetVelocity(const Vector3<float>& vVelocity)
{
	m_vVelocity = vVelocity;
}

/* ========================================
	�Z�b�^�[(�����x)�֐�
	-------------------------------------
	�����FVector3<float>	�����x
=========================================== */
void ComponentRigidbody::SetAcceleration(const Vector3<float>& vAcceleration)
{
	m_vAcceleration = vAcceleration;
}

/* ========================================
	�Z�b�^�[(����)�֐�
	-------------------------------------
	�����Ffloat	����
=========================================== */
void ComponentRigidbody::SetMass(float fMass)
{
	m_fMass = fMass;
}

/* ========================================
	�Z�b�^�[(�d�͗L���t���O)�֐�
	-------------------------------------
	�����Fbool	�d�͗L���t���O
=========================================== */
void ComponentRigidbody::SetUseGravity(bool bUseGravity)
{
	m_bUseGravity = bUseGravity;

}

/* ========================================
	�Z�b�^�[(��C��R)�֐�
	-------------------------------------
	�����Ffloat	��C��R
=========================================== */
void ComponentRigidbody::SetAirDrag(float fAirDrag)
{
	m_fAirDrag = fAirDrag;
}

/* ========================================
	�Z�b�^�[(�n�ʖ��C)�֐�
	-------------------------------------
	�����Ffloat	�n�ʖ��C
=========================================== */
void ComponentRigidbody::SetGroundDrag(float fGroundDrag)
{
	m_fGroundDrag = fGroundDrag;
}


/* ========================================
	�Q�b�^�[(���x)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	���x
=========================================== */
Vector3<float> ComponentRigidbody::GetVelocity() const
{
	return m_vVelocity;
}

/* ========================================
	�Q�b�^�[(�����x)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�����x
=========================================== */
Vector3<float> ComponentRigidbody::GetAcceleration() const
{
	return m_vAcceleration;
}

/* ========================================
	�Q�b�^�[(����)�֐�
	-------------------------------------
	�ߒl�Ffloat	����
=========================================== */
float ComponentRigidbody::GetMass() const
{
	return m_fMass;
}

/* ========================================
	�Z�b�^�[(�d�͗L���t���O)�֐�
	-------------------------------------
	�����Fbool	�d�͗L���t���O
=========================================== */
bool ComponentRigidbody::GetUseGravity() const
{
	return m_bUseGravity;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentRigidbody::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupRig = Item::CreateGroup("Rigidbody");

	pGroupRig->AddGroupItem(Item::CreateBind("Velocity",	Item::Kind::Vector, &m_vVelocity));
	pGroupRig->AddGroupItem(Item::CreateBind("Acceleration",Item::Kind::Vector, &m_vDispAccel));
	pGroupRig->AddGroupItem(Item::CreateBind("Mass",		Item::Kind::Float,	&m_fMass));
	pGroupRig->AddGroupItem(Item::CreateBind("UseGravity",	Item::Kind::Bool,	&m_bUseGravity));
	pGroupRig->AddGroupItem(Item::CreateBind("AirDrag",		Item::Kind::Float,	&m_fAirDrag));
	pGroupRig->AddGroupItem(Item::CreateBind("GroundDrag",	Item::Kind::Float,	&m_fGroundDrag));

	window.AddItem(pGroupRig);
}

#endif // _DEBUG