/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(����)�pcpp
	------------------------------------
	ComponentCollisionSphere.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentCollisionSphere.h"
#include "ShapeSphere.h"				// �f�o�b�O�p�}�`(��)
#include "ComponentCollisionAABB.h"	// AABB�R���W�����Ƃ̏Փ˔���
#include "ComponentCollisionOBB.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentCollisionSphere::ComponentCollisionSphere(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
	, m_fRadius(0.0f)
{
	m_eColType = COL_SPHERE;	// �R���W�����̎�ސݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================== */
void ComponentCollisionSphere::Init()
{
	m_pShape = std::make_unique<ShapeSphere>();						// �R���W�����̌`��(�f�o�b�O�\���p)
	m_pShape->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);	// ���C���[�t���[���\��
	m_pShape->SetColor(WIRE_COLOR_NORMAL);							// ���C���[�t���[���̐F�ݒ�
}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V���s��
========================================== */
void ComponentCollisionSphere::Update()
{
	// �e�N���X�̍X�V����(�g�����X�t�H�[���̎Q��)���ɍs��
	ComponentCollisionBase::Update();

	// �f�o�b�O�p�}�`�̍X�V
	m_pShape->SetPosition(m_vPosition);

	// ���L�I�u�W�F�N�g�̃g�����X�t�H�[�����Q�Ƃ���ꍇ�̓X�P�[���̍ő�l�𔼌a�Ƃ��Ďg�p
	if (m_bRefOwnerTransform)
	{
		m_fRadius = m_vScale.GetMax() / 2;
	}


	// �\���p���̃X�P�[����ݒ�
	m_pShape->SetScale({ m_fRadius * 2, m_fRadius * 2, m_fRadius * 2 });

	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// �F�����ɖ߂�

}


/* ========================================
	�Փˊm�F����֐�
	-------------------------------------
	���e�F�Փˑ���̏Փ˔���̌`��ɂ����
		�@�����𕪊򂳂���
	-------------------------------------
	����1�F����I�u�W�F�N�g�̓����蔻��
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionSphere::CheckCollision(ComponentCollisionBase* otherCol)
{
	// �Փ˔��茋��
	bool bResult = false;	

	// �Փˑ���̌`����m�F
	switch (otherCol->GetColType())
	{
	// ���R���W����
	case COL_SPHERE:	
		bResult = CheckCollisionSphere(static_cast<ComponentCollisionSphere*>(otherCol));
		break;
	// �{�b�N�X�R���W����(�����s�{�b�N�X)
	case COL_AABB:		
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// �{�b�N�X�R���W����(�C�ӎ��{�b�N�X)
	case COL_OBB:
		bResult = CheckCollisionOBB(static_cast<ComponentCollisionOBB*>(otherCol));
		break;
	default:
		bResult = false;
		break;
	}

	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// �Փ˂��Ă�����F��ς���

	return bResult;
}

/* ========================================
	���Փ˔���֐�
	-------------------------------------
	���e�F�����m�̏Փ˔�����s��
	-------------------------------------
	����1�F����I�u�W�F�N�g���R���W����
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionSphere::CheckCollisionSphere(ComponentCollisionSphere* otherCol)
{

	bool bResult = false;	// �Փˌ���

	// 2�̋��̒��S�Ԃ̋���(�������x����̂��߁A������2����g�p)
	float fDistanceSq = (m_vPosition - otherCol->GetPosition()).LengthSq();	

	// �I�u�W�F�N�g�̔��a�̘a(2��)
	float fRadiusSumSq = (m_fRadius + otherCol->GetRadius()) * (m_fRadius + otherCol->GetRadius());

	if (fDistanceSq < fRadiusSumSq)
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	T_MTV tMtv = T_MTV();
	tMtv.bIsTrigger = otherCol->GetTrigger();
	tMtv.bIsCol = bResult;

	if (bResult)
	{
		tMtv.vAxis = (otherCol->GetPosition() - m_vPosition).GetNormalize();
		tMtv.fOverlap = otherCol->GetRadius() + m_fRadius - std::sqrt(fDistanceSq);
	}
	else
	{
		tMtv.fOverlap = 0.0f;
	}

	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());


	return bResult;
}

/* ========================================
	AABB�Փ˔���֐�
	-------------------------------------
	���e�FAABB�ƏՓ˂��Ă��邩���m�F����
	-------------------------------------
	����1�F���I�u�W�F�N�gAABB�R���W����
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionSphere::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	bool bResult	= false;	// �Փˌ���
	T_MTV tMtv		= T_MTV();	// MTV�\����

	Vector3<float> vClosestPoint;	// AABB��ŋ��ɍł��߂��_

	// �e���ł̍ł��߂��_�����߂�
	vClosestPoint.x = std::clamp(this->GetPosition().x, otherCol->GetMin().x, otherCol->GetMax().x);
	vClosestPoint.y = std::clamp(this->GetPosition().y, otherCol->GetMin().y, otherCol->GetMax().y);
	vClosestPoint.z = std::clamp(this->GetPosition().z, otherCol->GetMin().z, otherCol->GetMax().z);

	// AABB��̍ł��߂��_���狅�̒��S�ւ̃x�N�g��
	Vector3<float> vSphCenToCloAABB = vClosestPoint - this->GetPosition();

	float fDistanceSq = vSphCenToCloAABB.LengthSq();						// ������2��
	float fRadiusSq = this->GetRadius() * this->GetRadius();	// ���̔��a��2��

	// ���̔��a�����������߂��ꍇ�͏Փ˂��Ă���
	if (fDistanceSq < fRadiusSq)
	{
		tMtv.vAxis		= vSphCenToCloAABB.GetNormalize();				// �Փ˕���	
		tMtv.fOverlap	= this->GetRadius() - std::sqrt(fDistanceSq);	// �d�Ȃ��

		bResult = true;
	}
	else
	{
		tMtv.fOverlap = 0.0f;	// �d�Ȃ��

		bResult = false;
	}

	tMtv.bIsCol		= bResult;					// �Փ˔��茋��
	tMtv.bIsTrigger = otherCol->GetTrigger();	// �g���K�[����(���蔲������)

	// �Փˑ����MTV��ݒ�
	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
}

/* ========================================
	OBB�Փ˔���֐�
	-------------------------------------
	���e�FOBB�ƏՓ˂��Ă��邩���m�F����
	-------------------------------------
	����1�F���I�u�W�F�N�gOBB�R���W����
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionSphere::CheckCollisionOBB(ComponentCollisionOBB* otherCol)
{
	bool bResult	= false;	// �Փˌ���
	T_MTV tMtv		= T_MTV();	// MTV�\����

	// OBB�̍\���̍쐬
	ComponentCollisionOBB::T_OBB tObb = otherCol->CreateOBB(otherCol);
	// ���̒��S����OBB�̒��S�܂ł̃x�N�g�����v�Z
	Vector3<float> vDis = this->GetPosition() - otherCol->GetPosition();
	// OBB��̍ł��߂��_�����߂�
	Vector3<float> vClosestOBB = otherCol->GetPosition();	// OBB�̒��S�������l�Ƃ���


	// OBB�̊e���[�J�����ɑ΂��āA���̒��S����OBB�̒��S�܂ł̃x�N�g�����ˉe
	for (int i = 0; i < 3; i++)
	{
		// OBB�̃��[�J�������擾
		Vector3<float> vAxis = tObb.vAxis[i];

		// ���̒��S����OBB�̒��S�܂ł̃x�N�g����OBB�̃��[�J�����Ɏˉe
		float fDis = vDis.Dot(vAxis);

		// OBB�̃��[�J�����Ɏˉe�����x�N�g����OBB�̃��[�J�����ɖ߂�
		// OBB�̕\�ʏ�̍��W�ɕϊ����AOBB�̒��S���W�ɉ��Z
		// ���̏�����OBB�\�ʏ�̍ł��߂��_�����߂�
		vClosestOBB += vAxis * std::clamp(fDis, -tObb.fExtent[i], tObb.fExtent[i]);

	}

	Vector3<float> vSphCenToCloOBB = vClosestOBB - this->GetPosition();	// ���̒��S����OBB��̍ł��߂��_�܂ł̃x�N�g��
	// �ł��߂��_�Ƌ��̒��S�Ƃ̋��������߂�(�v�Z�ʂ����炷���߂�2�拗���Ŕ�r)
	float fDistanceSq = vSphCenToCloOBB.LengthSq();
	float fRadiusSq = this->GetRadius() * this->GetRadius();	// ���̔��a��2��

	// ���̔��a�����������߂��ꍇ�͏Փ˂��Ă���
	if (fDistanceSq < fRadiusSq)
	{
		tMtv.vAxis		= vSphCenToCloOBB.GetNormalize();				// �Փ˕���
		tMtv.fOverlap	= this->GetRadius() - std::sqrt(fDistanceSq);	// �d�Ȃ��

		bResult = true;
	}
	else
	{
		tMtv.fOverlap = 0.0f;	// �d�Ȃ��

		bResult = false;
	}

	tMtv.bIsCol		= bResult;					// �Փ˔��茋��
	tMtv.bIsTrigger = otherCol->GetTrigger();	// �g���K�[����(���蔲������)

	// �Փˑ����MTV��ݒ�
	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
}


/* ========================================
	�Q�b�^�[(���a)�֐�
	-------------------------------------
	�ߒl�F���a�@float
=========================================== */
float ComponentCollisionSphere::GetRadius()
{
	return m_fRadius;
}

/* ========================================
	�Z�b�^�[(���a)�֐�
	-------------------------------------
	����1�F���a�@float
=========================================== */
void ComponentCollisionSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentCollisionSphere::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColSphere = Item::CreateGroup("CollisionSphere");

	ComponentCollisionBase::DebugColBase(pGroupColSphere, "CollisionSphere");

	pGroupColSphere->AddGroupItem(Item::CreateBind("Radius", Item::Kind::Float, &m_fRadius));

	window.AddItem(pGroupColSphere);
}

#endif // _DEBUG