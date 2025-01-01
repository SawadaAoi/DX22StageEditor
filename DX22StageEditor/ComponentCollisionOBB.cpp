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
{
	m_eColType = E_ColType::COL_OBB;	// �R���W�����̎�ސݒ�
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

	// �e�N���X�̍X�V����(�g�����X�t�H�[���̎Q��)���ɍs��
	ComponentCollisionBase::Update();

	// �f�o�b�O�p�}�`�̍X�V
	m_pShape->SetPosition(m_vPosition);
	m_pShape->SetScale(m_vScale);
	m_pShape->SetRotation(m_qRotation);
	m_pShape->SetColor(WIRE_COLOR_NORMAL);	// �F�����ɖ߂�

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
		bResult = CheckCollisionAABB(static_cast<ComponentCollisionAABB*>(otherCol));
		break;
	// ���R���W����
	case COL_SPHERE:	
		bResult = CheckCollisionSphere(static_cast<ComponentCollisionSphere*>(otherCol));
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
	bool bResult = false;	// �Փˌ���

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

	// MTV�\���̃��X�g�̃g���K�[����(���蔲������)���X�V
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == otherCol->GetOwnerObject()->GetName())
		{
			mtv.bIsTrigger = otherCol->GetTrigger();
			// �Փ˂��Ă��Ȃ��ꍇ�A�d�Ȃ�ʂ�0�ɂ���
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
	tObb.vCenter			= Col->GetPosition();
	// ���[�J����
	tObb.vAxis[AXIS_X]		= Col->GetRotation().GetRightVector();
	tObb.vAxis[AXIS_Y]		= Col->GetRotation().GetUpVector();
	tObb.vAxis[AXIS_Z]		= Col->GetRotation().GetForwardVector();
	// �e���̒���(�{�b�N�X�̑傫���̔���)
	tObb.fExtent[AXIS_X]	= Col->GetScale().x * 0.5f;
	tObb.fExtent[AXIS_Y]	= Col->GetScale().y * 0.5f;
	tObb.fExtent[AXIS_Z]	= Col->GetScale().z * 0.5f;

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
	bool bResult = false;	// �Փˌ���
	T_MTV tMtv = T_MTV();	// �ŏ��ړ��x�N�g���\����
	// �����OBB��MTV�\���̂��擾
	if (m_pOwnerObj->GetName() == tObb1.sName)		tMtv = GetMtv(tObb2.sName);	// ���g���������̏ꍇ
	else if (m_pOwnerObj->GetName() == tObb2.sName)	tMtv = GetMtv(tObb1.sName);	// ���肪�������̏ꍇ

	// ���gOBB�̊e���[�J�����𕪗����Ƃ��āA����OBB�Ƃ̕���������s��
	for (int i = 0; i < 3; i++)
	{
		// ��������ݒ�
		Vector3<float> vAxis = tObb1.vAxis[i];		

		// ��������OBB�̒���
		float fLength1	= tObb1.fExtent[i];						// �����������[�J�����̏ꍇ�A���̎������͐����Ȃ̂ŁA���������̂܂܎g��
		float fLength2	= GetProjectionLength(vAxis, tObb2);	// ����OBB�̎ˉe���̒������擾

		float fDis		= std::abs(vDis.Dot(vAxis));			// �������ɒ��S�Ԃ̋������ˉe(��������Ŕ�r���s��)

		float fOverlap	= (fLength1 + fLength2) - fDis;			// �d�Ȃ��

		// �d�Ȃ�ʂ�0����������������������
		if (fOverlap < 0.0f)
		{
			bResult			= true;
		}
		// �d�Ȃ�ʂ��ŏ��̏ꍇ�A�ŏ��ړ��x�N�g�����X�V
		else if (fOverlap < tMtv.fOverlap)
		{
			tMtv.vAxis		= (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// �Փ˕����x�N�g���̎�(���S�Ԃ̃x�N�g���̌����ɂ���Ĕ��])
			tMtv.fOverlap	= fOverlap;												// �ŏ��ړ��x�N�g���̏d�Ȃ��

			bResult			= false;
		}

		tMtv.bIsCol = !bResult;	// �Փˌ���(bResult��true=�Փ˂��Ă��Ȃ��̂Ŕ��])

		if (bResult) break;		// �����������������烋�[�v�𔲂���
	}

	// �����OBB��MTV�\���̂��X�V
	if		(m_pOwnerObj->GetName() == tObb1.sName)	SetColObjMTV(tMtv, tObb2.sName);	// ���g���������̏ꍇ
	else if (m_pOwnerObj->GetName() == tObb2.sName)	SetColObjMTV(tMtv, tObb1.sName);	// ���肪�������̏ꍇ

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
	bool bResult	= false;	// �Փˌ���
	bool bExitLoop	= false;	// 2�d���[�v�𔲂���t���O

	T_MTV tMtv = T_MTV();		// �ŏ��ړ��x�N�g���\����
	tMtv = GetMtv(tObb2.sName);	// �����OBB��MTV�\���̂��擾

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
				bResult		= true;
				bExitLoop	= true;
			}
			// �d�Ȃ�ʂ��ŏ��̏ꍇ�A�ŏ��ړ��x�N�g�����X�V
			else if (fOverlap < tMtv.fOverlap)
			{
				// �Ō����MTV�\���̂��X�V
				tMtv.vAxis		= (vDis.Dot(vAxis) < 0.0f) ? vAxis : (vAxis * -1.0f);	// �Փ˕����x�N�g���̎�(���S�Ԃ̃x�N�g���̌����ɂ���Ĕ��])
				tMtv.fOverlap	= fOverlap;	// �ŏ��ړ��x�N�g���̏d�Ȃ��

				bResult			= false;
			}

			tMtv.bIsCol = !bResult;	// �Փˌ���(bResult��true=�Փ˂��Ă��Ȃ��̂Ŕ��])

			if (bResult) break;		// �����������������烋�[�v�𔲂���
		}
		if (bExitLoop) break;
	}

	// �����OBB��MTV�\���̂��X�V
	SetColObjMTV(tMtv, tObb2.sName);

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

	Vector3<float> vRight	= tObb.vAxis[AXIS_X] * tObb.fExtent[AXIS_X];	// �E��
	Vector3<float> vUp		= tObb.vAxis[AXIS_Y] * tObb.fExtent[AXIS_Y];	// �㎲
	Vector3<float> vForward = tObb.vAxis[AXIS_Z] * tObb.fExtent[AXIS_Z];	// �O��

	// �e���̒��������߁A�ˉe���̒����ɉ��Z
	fLength += std::abs(vRight.Dot(vAxis));		// ���g�̉E�����𕪗����Ƃ��Ďˉe
	fLength += std::abs(vUp.Dot(vAxis));		// ���g�̏�����𕪗����Ƃ��Ďˉe
	fLength += std::abs(vForward.Dot(vAxis));	// ���g�̑O�����𕪗����Ƃ��Ďˉe

	return fLength;
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
bool ComponentCollisionOBB::CheckCollisionAABB(ComponentCollisionAABB* otherCol)
{
	// AABB��e�N���X�ɃL���X�g
	ComponentCollisionOBB* pAABBAsOBB = reinterpret_cast<ComponentCollisionOBB*>(otherCol);

	// AABB��OBB�Ƃ��Ĉ������߂ɒl��ݒ�
	pAABBAsOBB->SetPosition(otherCol->GetPosition());				// ���W��AABB�̍��W
	pAABBAsOBB->SetScale(otherCol->GetMax() - otherCol->GetMin());		// �傫����AABB�̍ő���W�ƍŏ����W�̍�
	pAABBAsOBB->SetRotation(Quaternion());						// ��]�͂Ȃ�

	return this->CheckCollisionOBB(pAABBAsOBB);
}

/* ========================================
	���Փ˔���֐�
	-------------------------------------
	���e�F���ƏՓ˂��Ă��邩���m�F����
	-------------------------------------
	����1�F���I�u�W�F�N�g���R���W����
	-------------------------------------
	�ߒl�Ftrue=�Փ˂��Ă���/false=�Փ˂��Ă��Ȃ�
=========================================== */
bool ComponentCollisionOBB::CheckCollisionSphere(ComponentCollisionSphere* otherCol)
{
	bool bResult = false;	// �Փˌ���

	// OBB�̍\���̍쐬
	ComponentCollisionOBB::T_OBB tObb = CreateOBB(this);
	// ���̒��S����OBB�̒��S�܂ł̃x�N�g�����v�Z
	Vector3<float> vDis = otherCol->GetPosition() - this->GetPosition();
	// OBB��̍ł��߂��_�����߂�
	Vector3<float> vClosestOBB = this->GetPosition();	// OBB�̒��S�������l�Ƃ���


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

	Vector3<float> vSphCenToCloOBB = otherCol->GetPosition() - vClosestOBB;	// ���̒��S����OBB��̍ł��߂��_�܂ł̃x�N�g��
	// �ł��߂��_�Ƌ��̒��S�Ƃ̋��������߂�(�v�Z�ʂ����炷���߂�2�拗���Ŕ�r)
	float fDistanceSq = vSphCenToCloOBB.LengthSq();
	float fRadiusSq = otherCol->GetRadius() * otherCol->GetRadius();	// ���̔��a��2��

	// ���̔��a�����������߂��ꍇ�͏Փ˂��Ă���
	if (fDistanceSq < fRadiusSq)
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	T_MTV tMtv = T_MTV();	// MTV�\����
	tMtv.sName		= otherCol->GetOwnerObject()->GetName();
	tMtv.bIsTrigger = otherCol->GetTrigger();
	tMtv.bIsCol		= bResult;

	if (bResult)
	{
		tMtv.vAxis = vSphCenToCloOBB.GetNormalize();
		tMtv.fOverlap = otherCol->GetRadius() - std::sqrt(fDistanceSq);
	}
	else
	{
		tMtv.fOverlap = 0.0f;
	}

	SetColObjMTV(tMtv, otherCol->GetOwnerObject()->GetName());

	return bResult;
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



	window.AddItem(pGroupColOBB);
}

#endif // _DEBUG
