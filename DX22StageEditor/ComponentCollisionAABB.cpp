/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(AABB)�pcpp
	------------------------------------
	ComponentCollisionAABB.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentCollisionAABB.h"
#include "ShapeBox.h"					// �f�o�b�O�p�}�`(�{�b�N�X)
#include "ComponentCollisionSphere.h"	// ���R���W�����Ƃ̏Փ˔���
#include "ComponentCollisionOBB.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentCollisionAABB::ComponentCollisionAABB(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
	, m_vMin(Vector3<float>(-0.5f, -0.5f, -0.5f))
	, m_vMax(Vector3<float>(0.5f, 0.5f, 0.5f))
{
	m_eColType = COL_AABB;	// �R���W�����̎�ސݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
=========================================== */
void ComponentCollisionAABB::Init()
{
	m_pShape = std::make_unique<ShapeBox>();						// �R���W�����̌`��(�f�o�b�O�\���p)
	m_pShape->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);	// ���C���[�t���[���\��
	m_pShape->SetColor(WIRE_COLOR_NORMAL);							// ���C���[�t���[���̐F�ݒ�
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V���s��
=========================================== */
void ComponentCollisionAABB::Update()
{
	// �e�N���X�̍X�V����(�g�����X�t�H�[���̎Q��)���ɍs��
	ComponentCollisionBase::Update();

	// �ŏ��A�ő���W�X�V(�����蔻��̈ʒu���X�V����)
	m_vMin = m_vPosition - m_vScale * 0.5f;
	m_vMax = m_vPosition + m_vScale * 0.5f;

	// �f�o�b�O�p�}�`�̍X�V
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	// AABB�͏�Ƀ��[���h���ɕ��s�Ȃ̂ŉ�]�͍l�����Ȃ�

	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// �F�����ɖ߂�

}

/* ========================================
	�Փˊm�F����֐�
	-------------------------------------
	���e�F�Փˑ���̏Փ˔���̌`��ɂ����
		�@�����𕪊򂳂���
	-------------------------------------
	����1�F����I�u�W�F�N�g�R���W����
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionAABB::CheckCollision(ComponentCollisionBase* otherCol)
{
	bool bResult = false;
	// �Փˑ���̌`����m�F
	switch (otherCol->GetColType())
	{
	// �{�b�N�X�R���W����(�����s�{�b�N�X)
	case COL_AABB:		
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// �{�b�N�X�R���W����(�C�ӎ��{�b�N�X)
	case COL_OBB:		
		bResult = CheckCollisionAABBToOBB(this, static_cast<ComponentCollisionOBB*>(otherCol));
		break;
	// ���R���W����
	case COL_SPHERE:	
		bResult = CheckCollisionAABBToSphere(this, static_cast<ComponentCollisionSphere*>(otherCol));
		break;

	default:
		return false;
		break;
	}

	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// �Փ˂��Ă�����F��ς���

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
bool ComponentCollisionAABB::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	// �ȉ��̏�������������Ă�����Փ˂��Ă���
		// ���g�̉E�ʁ�����̍��� && ���g�̍��ʁ�����̉E��
	if ((this->GetMax().x >= otherCol->GetMin().x && this->GetMin().x <= otherCol->GetMax().x) &&
		// ���g�̏�ʁ�����̉��� && ���g�̉��ʁ�����̏��
		(this->GetMax().y >= otherCol->GetMin().y && this->GetMin().y <= otherCol->GetMax().y) &&	
		// ���g�̉��ʁ�����̎�O�� && ���g�̎�O�ʁ�����̉���
		(this->GetMax().z >= otherCol->GetMin().z && this->GetMin().z <= otherCol->GetMax().z))		
	{
		return true;	
	}

	return false;	// �Փ˂��Ă��Ȃ�
}



/* ========================================
	�Q�b�^�[(�ŏ����W(���A���A��O)�֐�
	-------------------------------------
	�ߒl�F�ŏ����W�@Vector3<float>
=========================================== */
Vector3<float> ComponentCollisionAABB::GetMin()
{
	return m_vMin;
}


/* ========================================
	�Q�b�^�[(�ő���W(�E�A��A��)�֐�
	-------------------------------------
	�ߒl�F�ő���W�@Vector3<float>
=========================================== */
Vector3<float> ComponentCollisionAABB::GetMax()
{
	return m_vMax;
}


/* ========================================
	�Z�b�^�[(�ŏ����W(���A���A��O))�֐�
	-------------------------------------
	�����F�ŏ����W�@Vector3<float>
=========================================== */
void ComponentCollisionAABB::SetMin(Vector3<float> vMin)
{
	m_vMin = vMin;
}

/* ========================================
	�Z�b�^�[(�ő���W(�E�A��A��))�֐�
	-------------------------------------
	�����F�ő���W�@Vector3<float>
=========================================== */
void ComponentCollisionAABB::SetMax(Vector3<float> vMax)
{
	m_vMax = vMax;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentCollisionAABB::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColAABB = Item::CreateGroup("CollisionAABB");

	ComponentCollisionBase::DebugColBase(pGroupColAABB);

	pGroupColAABB->AddGroupItem(Item::CreateBind("MinPos", Item::Kind::Vector, &m_vMin));
	pGroupColAABB->AddGroupItem(Item::CreateBind("MaxPos", Item::Kind::Vector, &m_vMax));

	window.AddItem(pGroupColAABB);
}

#endif // _DEBUG
