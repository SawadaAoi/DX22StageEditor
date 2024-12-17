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

	m_fRadius += 0.003f;	// �����]�T����������(���b�V���ɏd�Ȃ�Ȃ��悤��)

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
		bResult = CheckCollisionAABBToSphere(static_cast<ComponentCollisionAABB*>(otherCol), this);
		break;
	// �{�b�N�X�R���W����(�C�ӎ��{�b�N�X)
	case COL_OBB:
		bResult = CheckCollisionOBBToSphere(static_cast<ComponentCollisionOBB*>(otherCol), this);
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
	// 2�̋��̒��S�Ԃ̋���(�������x����̂��߁A������2����g�p)
	float fDistanceSq = (m_vPosition - otherCol->GetPosition()).LengthSq();	

	// �I�u�W�F�N�g�̔��a�̘a(2��)
	float fRadiusSumSq = (m_fRadius + otherCol->GetRadius()) * (m_fRadius + otherCol->GetRadius());

	return fDistanceSq < fRadiusSumSq;
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