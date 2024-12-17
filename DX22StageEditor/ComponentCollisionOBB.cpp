/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(OBB)�pcpp
	------------------------------------
	ComponentCollisionOBB.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentCollisionOBB.h"
#include "ShapeBox.h"
#include "ComponentTransform.h"
#include <cmath>	// ��Βl
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ObjectBase.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentCollisionOBB::ComponentCollisionOBB(ObjectBase* pOwner)
	: ComponentCollisionBase(pOwner)
#ifdef _DEBUG
	, m_nSelectMTV(-1)	// �I�𒆂̍ŏ��ړ��x�N�g��
#endif // _DEBUG
{
	m_eColType = E_ColType::COL_OBB;	// �R���W�����̎�ސݒ�
	m_tMtvs.clear();	// MTV�\���̃��X�g�N���A
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
=========================================== */
void ComponentCollisionOBB::Init()
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
void ComponentCollisionOBB::Update()
{
#ifdef _DEBUG

	// �I�u�W�F�N�g�ڍׂ����g���AOBB�R���|�[�l���g���\������Ă���ꍇ
	if (CHECK_DISP_COMP("CollisionOBB"))
	{

		// ���ݑI�𒆂̍ŏ��ړ��x�N�g�����擾
		if (m_tMtvs.size() > 0 && m_nSelectMTV != -1)
		{
			// �ŏ��ړ��x�N�g���\���̂̕\��
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Axis"].SetVector(m_tMtvs.at(m_nSelectMTV).vAxis.ToXMFLOAT3());
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Overlap"].SetFloat(m_tMtvs.at(m_nSelectMTV).fOverlap);
			WIN_OBJ_INFO["CollisionOBB"]["MTV_IsCol"].SetBool(m_tMtvs.at(m_nSelectMTV).bIsCol);
		}
		else
		{
			// MTV�\���̃��X�g����̏ꍇ�A�����l��\��
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Axis"].SetVector(Vector3<float>(0.0f, 0.0f, 0.0f).ToXMFLOAT3());
			WIN_OBJ_INFO["CollisionOBB"]["MTV_Overlap"].SetFloat(0);
			WIN_OBJ_INFO["CollisionOBB"]["MTV_IsCol"].SetBool(false);
		}

		WIN_OBJ_INFO["CollisionOBB"]["MTVs"].RemoveListItemAll();	// MTV�\���̃��X�g���N���A
	}

#endif // _DEBUG
	// �e�N���X�̍X�V����(�g�����X�t�H�[���̎Q��)���ɍs��
	ComponentCollisionBase::Update();

	// �f�o�b�O�p�}�`�̍X�V
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	m_pShape->SetRotation(m_qRotation);
	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// �F�����ɖ߂�

	// MTV�\���̔z��N���A
	m_tMtvs.clear();

	// MTV�\���̃��X�g�ɓ����蔻������I�u�W�F�N�g��ǉ�
	for (const auto& colObj : m_bColStatesMap)
	{
		// ���O�̂ݓo�^
		T_MTV mtv = T_MTV();
		mtv.sName = colObj.first->GetName();
		m_tMtvs.push_back(mtv);

#ifdef _DEBUG
		// �I�u�W�F�N�g�ڍׂ����g���AOBB�R���|�[�l���g���\������Ă���ꍇ
		if (CHECK_DISP_COMP("CollisionOBB"))
		{
			DebugUI::Item* list = &WIN_OBJ_INFO["CollisionOBB"]["MTVs"];	// MTV���X�g�擾
			list->AddListItem(("MTV_" + mtv.sName).c_str());				// MTV���X�g�ɒǉ�
		}
#endif // _DEBUG
	}

}

/* ========================================
	�Փˊm�F����֐�
	-------------------------------------
	���e�F�Փˑ���̏Փ˔���̌`��ɂ����
		�@�����𕪊򂳂���
	-------------------------------------
	����1�F�Փˑ���R���W����
	-------------------------------------
	�߂�l�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionOBB::CheckCollision(ComponentCollisionBase* otherCol)
{
	// MTV�\���̃��X�g�ɒǉ�(�Փˊm�F���̃I�u�W�F�N�g�p)
	bool bResult = false;
	// �Փˑ���̌`����m�F
	switch (otherCol->GetColType())
	{
		// �{�b�N�X�R���W����(�C�ӎ��{�b�N�X)
	case COL_OBB:
		bResult = CheckCollisionOBB(otherCol);
		break;
		// �{�b�N�X�R���W����(�����s�{�b�N�X)
	case COL_AABB:
		bResult = CheckCollisionAABBToOBB(static_cast<ComponentCollisionAABB*>(otherCol), this);
		break;
		// ���R���W����
	case COL_SPHERE:
		bResult = CheckCollisionOBBToSphere(this, static_cast<ComponentCollisionSphere*>(otherCol), true);
		break;
	default:
		return false;
		break;
	}


	if (bResult)	m_pShape->SetColor(WIRE_COLOR_HIT);	// �Փ˂��Ă�����F��ς���

	return bResult;
}

/* ========================================
	OBB�Փ˔���֐�
	------------------------------------
	���e�FOBB�ƏՓ˂��Ă��邩���m�F����
	-------------------------------------
	����1�F����OBB�R���W����
	��CheckCollisionAABBToOBB�Ŏg�p���邽�߂�
	�@������ComponentCollisionBase�ɂ��Ă���
	�@(AABB����OBB��cast�ł��Ȃ�����)
	-------------------------------------
	�߂�l�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionOBB::CheckCollisionOBB(ComponentCollisionBase* otherCol)
{
	bool bResult = false;

	// OBB���m�̒��S�Ԃ̋���
	Vector3<float> vDist = (this->GetPosition() - otherCol->GetPosition());

	// OBB�̍\���̍쐬
	T_OBB tObb1 = CreateOBB(static_cast<ComponentCollisionBase*>(this));		// ���g��OBB
	T_OBB tObb2 = CreateOBB(otherCol);											// �����OBB

	//  2��OBB���m�𕪗����鎲�����݂��邩�m�F����
	if (CheckLocalSeparateAxis(tObb1, tObb2, vDist) ||	// ���[�J������������(���g��XYZ����������)
		CheckLocalSeparateAxis(tObb2, tObb1, vDist) ||	// ���[�J������������(�����XYZ����������)
		CheckCrossSeparateAxis(tObb1, tObb2, vDist))	// OBB���m�̌�������������(���g��XYZ���Ƒ����XYZ���̊O�ς�������)
	{
		bResult = false;	// ����������������(�Փ˂��Ă��Ȃ�)
	}
	else
	{
		bResult = true;		// ��������������Ȃ�����(�Փ�)

	}

	// MTV�\���̃��X�g�̏d�Ȃ�ʁATriger������X�V
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == otherCol->GetOwnerObject()->GetName())
		{
			mtv.bIsTrigger = otherCol->GetTrigger();
			if (!bResult) mtv.fOverlap = 0.0f;

			break;
		}
	}

	return bResult;
}

/* ========================================
	OBB�\���̍쐬�֐�
	-------------------------------------
	���e�FOBB�\���̂��쐬����
	-------------------------------------
	����1�FOBB�R���W����
	-------------------------------------
	�߂�l�FOBB�\����
=========================================== */
ComponentCollisionOBB::T_OBB ComponentCollisionOBB::CreateOBB(ComponentCollisionBase* Col)
{
	T_OBB tObb;

	// ���S���W
	tObb.vCenter = Col->GetPosition();
	// ���[�J����
	tObb.vAxis[AXIS_X] = Col->GetRotation().GetRightVector();
	tObb.vAxis[AXIS_Y] = Col->GetRotation().GetUpVector();
	tObb.vAxis[AXIS_Z] = Col->GetRotation().GetForwardVector();
	// �e���̒���(�{�b�N�X�̑傫���̔���)
	tObb.fExtent[AXIS_X] = Col->GetScale().x * 0.5f;
	tObb.fExtent[AXIS_Y] = Col->GetScale().y * 0.5f;
	tObb.fExtent[AXIS_Z] = Col->GetScale().z * 0.5f;

	// �Փˑ���I�u�W�F�N�g��
	tObb.sName = Col->GetOwnerObject()->GetName();

	return tObb;
}

/* ========================================
	���[�J������������֐�
	-------------------------------------
	���e�FOBB���m�̃��[�J��������������s��
	-------------------------------------
	����1�F���[�J����XYZ�����������ƂȂ�OBB
	����2�F�����OBB
	����3�FOBB���m�̒��S�Ԃ̃x�N�g��
	-------------------------------------
	�ߒl�Ftrue=����������������(�Փ˂��Ă��Ȃ�)
		�@false=��������������Ȃ�����(�Փ˂��Ă���)
=========================================== */
bool ComponentCollisionOBB::CheckLocalSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis)
{
	bool bResult = false;
	T_MTV tMtv = T_MTV();	// �ŏ��ړ��x�N�g���\����

	// ���gOBB�̊e���[�J�����𕪗����Ƃ��āA����OBB�Ƃ̕���������s��
	for (int i = 0; i < 3; i++)
	{
		// ��������ݒ�
		Vector3<float> vAxis = tObb1.vAxis[i];

		// ��������OBB�̒���
		float fLength1 = tObb1.fExtent[i];						// �����������[�J�����̏ꍇ�A���̎������͐����Ȃ̂ŁA���������̂܂܎g��
		float fLength2 = GetProjectionLength(vAxis, tObb2);	// ����OBB�̎ˉe���̒������擾

		float fDis = std::abs(vDis.Dot(vAxis));			// �������ɒ��S�Ԃ̋������ˉe(��������Ŕ�r���s��)

		float fOverlap = (fLength1 + fLength2) - fDis;		// �d�Ȃ��

		// �d�Ȃ�ʂ�0����������������������
		if (fOverlap < 0.0f)
		{
			tMtv.bIsCol = false;
			bResult = true;
			break;
		}
		// �d�Ȃ�ʂ��ŏ��̏ꍇ�A�ŏ��ړ��x�N�g�����X�V
		else if (fOverlap < tMtv.fOverlap)
		{
			tMtv.vAxis = (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// �Փ˕����x�N�g���̎�(���S�Ԃ̃x�N�g���̌����ɂ���Ĕ��])
			tMtv.fOverlap = fOverlap;	// �ŏ��ړ��x�N�g���̏d�Ȃ��
			tMtv.bIsCol = true;		// �Փ˂��Ă���

			bResult = false;
		}
	}

	// �������O��MTV���X�V
	for (auto& mtv : m_tMtvs)
	{
		if ((mtv.sName == tObb2.sName && m_pOwnerObj->GetName() == tObb1.sName)
			|| (mtv.sName == tObb1.sName && m_pOwnerObj->GetName() == tObb2.sName))
		{
			mtv.bIsCol = tMtv.bIsCol;
			mtv.vAxis = tMtv.vAxis;
			mtv.fOverlap = tMtv.fOverlap;
			break;
		}
	}

	return bResult;	// ��������������Ȃ�����
}

/* ========================================
	��������������֐�
	-------------------------------------
	���e�FOBB���m�̌���������������s��
		�@OBB�̃��[�J��XYZ���̊O�ς��������ƂȂ�
		  3���~3����9�ʂ�̕������𒲂ׂ�
	-------------------------------------
	����1�F���g��OBB
	����2�F�����OBB
	����3�FOBB���m�̒��S�Ԃ̃x�N�g��
	-------------------------------------
	�ߒl�Ftrue=����������������(�Փ˂��Ă��Ȃ�)
		�@false=��������������Ȃ�����(�Փ˂��Ă���)
=========================================== */
bool ComponentCollisionOBB::CheckCrossSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis)
{
	bool bResult = false;
	bool bExitLoop = false;

	T_MTV tMtv = T_MTV();	// �ŏ��ړ��x�N�g���\����

	// ���g��OBB��3����
	for (int i = 0; i < 3; i++)
	{
		// �����OBB��3����
		for (int j = 0; j < 3; j++)
		{
			// ��������ݒ�
			// ���g��OBB�̎��Ƒ����OBB�̎��̊O�ς��擾
			Vector3<float> vAxis = tObb1.vAxis[i].Cross(tObb2.vAxis[j]);

			// �O�ς�0�x�N�g����������X�L�b�v
			if (vAxis.LengthSq() < 0.0001f)	continue;

			// �O�σx�N�g���𐳋K��
			vAxis.Normalize();

			// �������Ɏˉe���Ď���̒������擾
			float fLength1 = GetProjectionLength(vAxis, tObb1);	// ���gOBB
			float fLength2 = GetProjectionLength(vAxis, tObb2);	// ����OBB

			float fDis = std::abs(vDis.Dot(vAxis));	// �������ɒ��S�Ԃ̋������ˉe(��������Ŕ�r���s��)

			float fOverlap = (fLength1 + fLength2) - fDis;		// �d�Ȃ��

			// �d�Ȃ�ʂ�0����������������������
			if (fOverlap < 0.0f)
			{
				tMtv.bIsCol = false;	// �Փ˂��Ă��Ȃ�
				bResult = true;

				bExitLoop = true;
				break;
			}
			// �d�Ȃ�ʂ��ŏ��̏ꍇ�A�ŏ��ړ��x�N�g�����X�V
			else if (fOverlap < tMtv.fOverlap)
			{
				// �Ō����MTV�\���̂��X�V
				tMtv.vAxis = (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// �Փ˕����x�N�g���̎�(���S�Ԃ̃x�N�g���̌����ɂ���Ĕ��])
				tMtv.fOverlap = fOverlap;	// �ŏ��ړ��x�N�g���̏d�Ȃ��
				tMtv.bIsCol = true;		// �Փ˂��Ă���

				bResult = false;
			}

		}
		if (bExitLoop) break;
	}

	// �������O��MTV���X�V
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == tObb2.sName)
		{
			mtv.bIsCol = tMtv.bIsCol;
			mtv.vAxis = tMtv.vAxis;
			mtv.fOverlap = tMtv.fOverlap;
			break;
		}
	}

	return bResult;	// ��������������Ȃ�����
}

/* ========================================
	�ˉe�������擾�֐�
	-------------------------------------
	���e�FOBB�̊e���ɑ΂��Ďˉe���̒������擾����
	-------------------------------------
	����1�F�ˉe��
	����2�FOBB�\����
	-------------------------------------
	�ߒl�F�ˉe���̒���(���ɓ��e����OBB�̒���)
=========================================== */
float ComponentCollisionOBB::GetProjectionLength(Vector3<float> vAxis, T_OBB tObb)
{
	float fLength = 0.0f;	// �ˉe���̒���

	Vector3<float> vRight = tObb.vAxis[AXIS_X] * tObb.fExtent[AXIS_X];	// �E��
	Vector3<float> vUp = tObb.vAxis[AXIS_Y] * tObb.fExtent[AXIS_Y];	// �㎲
	Vector3<float> vForward = tObb.vAxis[AXIS_Z] * tObb.fExtent[AXIS_Z];	// �O��

	// �e���̒��������߁A�ˉe���̒����ɉ��Z
	fLength += std::abs(vRight.Dot(vAxis));		// ���g�̉E�����𕪗����Ƃ��Ďˉe
	fLength += std::abs(vUp.Dot(vAxis));		// ���g�̏�����𕪗����Ƃ��Ďˉe
	fLength += std::abs(vForward.Dot(vAxis));	// ���g�̑O�����𕪗����Ƃ��Ďˉe

	return fLength;
}


/* ========================================
	�Q�b�^�[(�ŏ��ړ��x�N�g���\���̔z��)�֐�
	-------------------------------------
	�ߒl�Fvector<T_MTV>	�ŏ��ړ��x�N�g���\���̔z��
=========================================== */
std::vector<ComponentCollisionOBB::T_MTV> ComponentCollisionOBB::GetMtvs()
{
	return m_tMtvs;
}

/* ========================================
	�Z�b�^�[(�ŏ��ړ��x�N�g���\���̔z��)�֐�
	-------------------------------------
	�����F�ŏ��ړ��x�N�g���\���̔z��
=========================================== */
void ComponentCollisionOBB::SetMtvs(std::vector<T_MTV> mtvs)
{
	m_tMtvs = mtvs;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentCollisionOBB::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupColOBB = Item::CreateGroup("CollisionOBB");

	ComponentCollisionBase::DebugColBase(pGroupColOBB, "CollisionOBB");

	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Name", Item::Kind::Text));	// �Փˑ���I�u�W�F�N�g��
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Axis", Item::Kind::Vector));	// �ŏ��ړ��x�N�g���̎�
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_Overlap", Item::Kind::Float));	// �ŏ��ړ��x�N�g���̏d�Ȃ��
	pGroupColOBB->AddGroupItem(Item::CreateValue("MTV_IsCol", Item::Kind::Bool));		// �Փ˂��Ă��邩

	// �Փˍ\���̃��X�g�̃R�[���o�b�N�֐�
	Item::ConstCallback callback = [this](const void* arg)
	{
		std::string sObjName = reinterpret_cast<const char*>(arg);	// ���X�g���ږ�

		// ���X�g�ԍ��擾
		Item* pList = &WIN_OBJ_INFO["CollisionOBB"]["MTVs"];
		m_nSelectMTV = pList->GetListNo(sObjName.c_str());

		WIN_OBJ_INFO["CollisionOBB"]["MTV_Name"].SetText(m_tMtvs.at(m_nSelectMTV).sName.c_str());	// �Փˑ���I�u�W�F�N�g��
	};

	// �ŏ��ړ��x�N�g���\���̔z��̕\��
	pGroupColOBB->AddGroupItem(Item::CreateList("MTVs", callback));

	window.AddItem(pGroupColOBB);
}

#endif // _DEBUG
