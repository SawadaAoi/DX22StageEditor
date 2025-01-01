/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g���pcpp
	------------------------------------
	ComponentCollisionBase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentCollisionBase.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionSphere.h"
#include <algorithm>

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
=========================================== */
ComponentCollisionBase::ComponentCollisionBase(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderCollisionBase)
	, m_pOwnerTransform(nullptr)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vPositionOffset(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation(Quaternion())
	, m_bRefOwnerTransform(true)
	, m_eColType(COL_NONE)
	, m_pShape(nullptr)
	, m_bIsDynamic(true)
	, m_bIsEnabled(true)
	, m_bIsDispCol(true)
	, m_bIsTrigger(false)
#ifdef _DEBUG
	, m_pColObjList(nullptr)
	, m_nSelectMTV(-1)	// �I�𒆂̍ŏ��ړ��x�N�g��
#endif // _DEBUG

{
	m_bColStatesMap.clear();

	// �����I�u�W�F�N�g�̏����V�[���̏Փ˔���Ǘ����X�g�ɒǉ�
	m_pOwnerObj->GetOwnerScene()->AddObjectCollision(this);

	m_pOwnerTransform = m_pOwnerObj->GetComponent<ComponentTransform>();


	m_tMtvs.clear();	// MTV�\���̃��X�g�N���A
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�I������
=========================================== */
ComponentCollisionBase::~ComponentCollisionBase()
{
	// �����I�u�W�F�N�g�̏����V�[���̏Փ˔���Ǘ����X�g����폜
	m_pOwnerObj->GetOwnerScene()->RemoveObjectCollision(this);
}



/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentCollisionBase::Update()
{
	// �����I�u�W�F�N�g�̃g�����X�t�H�[�������݂��Ȃ��ꍇ�͏������Ȃ�
	if (!m_pOwnerTransform) return;

	// ���L�I�u�W�F�N�g�̍��W�{�������W
	m_vPosition = m_pOwnerTransform->GetWorldPosition() + m_vPositionOffset;

	// �����I�u�W�F�N�g�̃g�����X�t�H�[�����Q�Ƃ���ꍇ
	if (m_bRefOwnerTransform)
	{
		// ��]�A�X�P�[����e�I�u�W�F�N�g����擾
		m_qRotation = m_pOwnerTransform->GetWorldRotation();
		m_vScale = m_pOwnerTransform->GetWorldScale();
	}

#ifdef _DEBUG
	if (CHECK_DISP_COMP(m_sColCompName.c_str()))
	{
		UpdateDebugMTVList();	// �ŏ��ړ��x�N�g�����X�g�X�V

		// �Փ˃I�u�W�F�N�g���X�g�̒ǉ��A�폜
		if (m_pColObjList != nullptr)
		{
			for (auto& colObj : m_bColStatesMap)
			{
				if (colObj.second == true)
					m_pColObjList->AddListItem(colObj.first->GetName().c_str());
				else
					m_pColObjList->RemoveListItem(colObj.first->GetName().c_str());
			}
		}
	}
	// �S�I�u�W�F�N�g�����蔻����\���t���O�X�V
	m_bIsDispColAll = WIN_BASIC_SETTING["DispCollision"].GetBool();
#endif // _DEBUG

	UpdateMTVList();	// �ŏ��ړ��x�N�g�����X�g�X�V
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ComponentCollisionBase::Draw()
{
	if (!m_bIsDispColAll) return;	// �S�R���W�����\���t���O��true�̏ꍇ�͏������Ȃ�

	if (!m_bIsDispCol) return;	// �R���W�����\���t���O��false�̏ꍇ�͏������Ȃ�

	// �f�o�b�O�p�}�`���Z�b�g���Ă���ꍇ
	if (m_pShape)
	{
		m_pShape->Draw();
	}
}

/* ========================================
	�Փ˔���X�V�֐�
	-------------------------------------
	���e�F�Փ˔���̍X�V����
	-------------------------------------
	����1�F�Փˑ���̓����蔻��R���|�[�l���g
=========================================== */
void ComponentCollisionBase::UpdateCollision(ComponentCollisionBase* otherCol)
{
	ObjectBase* pOtherObj = otherCol->GetOwnerObject();	// ����̃I�u�W�F�N�g���擾

	if (pOtherObj->GetState() == ObjectBase::STATE_DEAD) return;	// ����̃I�u�W�F�N�g�����S��ԂȂ珈�����Ȃ�
	if (!m_bIsEnabled) return;										// ���̃R���W�����������Ȃ珈�����Ȃ�
	if (!otherCol->GetEnable()) return;								// ����̃R���W�����������Ȃ珈�����Ȃ�
	if (!m_bIsDynamic && !otherCol->GetDynamic()) return;			// �����̃R���W�������ÓI�Ȃ珈�����Ȃ�

	// �Փ˃I�u�W�F�N�g���ꗗ�ɂȂ��ꍇ�A�ǉ�
	if (m_bColStatesMap.find(pOtherObj) == m_bColStatesMap.end())
	{
		m_bColStatesMap.emplace(pOtherObj, false);	// �Փˏ�Ԃ�false�ɐݒ�(�Փ˂��Ă��Ȃ�)
	}

	bool bColPrev = m_bColStatesMap.at(pOtherObj);	// �Փˏ��(�O�t���[��)
	bool bColCurrent = CheckCollision(otherCol);	// �Փ˔���(���݂̃t���[��)
	
	// �ՓˊJ�n
	// �O�t���[�����Փ˂��Ă��Ȃ��A���݂̃t���[�����Փ˂��Ă���ꍇ
	if (!bColPrev && bColCurrent)
	{
		m_pOwnerObj->OnCollisionEnter(pOtherObj);
	}
	// �Փ˒�
	// �O�t���[�������݂̃t���[�����Փ˂��Ă���ꍇ
	else if (bColPrev && bColCurrent)
	{
		m_pOwnerObj->OnCollisionStay(pOtherObj);
	}
	// �ՓˏI��
	// �O�t���[�����Փ˂��Ă��āA���݂̃t���[�����Փ˂��Ă��Ȃ��ꍇ
	else if (bColPrev && !bColCurrent)
	{
		m_pOwnerObj->OnCollisionExit(pOtherObj);
	}

	// �Փˏ�Ԃ��X�V
	m_bColStatesMap.at(pOtherObj) = bColCurrent;

}

/* ========================================
	�Փˏ�ԃ}�b�v�X�V�֐�
	-------------------------------------
	���e�F�Փˏ�ԃ}�b�v�̍X�V����
		�@�Փ˔���̌��ʁA����̃I�u�W�F�N�g���ς�鎖������̂ŁA
		  �Փ˔���X�V��ɌĂяo���āA�}�b�v���X�V����
	-------------------------------------
	����1�F�Փˑ���̓����蔻��R���|�[�l���g
=========================================== */
void ComponentCollisionBase::UpdateCollisionMap(ComponentCollisionBase* otherCol)
{
	ObjectBase* pOtherObj = otherCol->GetOwnerObject();	// ����̃I�u�W�F�N�g���擾

	// ���S��Ԃ̏ꍇ�̓}�b�v����폜
	if (pOtherObj->GetState() == ObjectBase::STATE_DEAD)
	{
		m_bColStatesMap.erase(otherCol->GetOwnerObject());

#ifdef _DEBUG
		// �f�o�b�O�Փ˃I�u�W�F�N�g���X�g����폜
		if (CHECK_DISP_COMP(m_sColCompName.c_str()))
		{
			if (!m_pColObjList)
				WIN_OBJ_INFO["CollObjectList"].RemoveListItem(pOtherObj->GetName().c_str());
		}
#endif // _DEBUG

		return;
	}
}

/* ========================================
	�Փˑ���MTV�X�V�֐�
	-------------------------------------
	���e�FMTV���X�g����Փˑ���̖��ƈ�v������̂�T���A
		  MTV�\���̂̒l���X�V����
	-------------------------------------
	����1�F�Փˑ�������Z�b�g����MTV�\����
	����2�F�Փˑ���̖��O
=========================================== */
void ComponentCollisionBase::SetColObjMTV(T_MTV tMtv, std::string sName)
{
	for (auto& mtv : m_tMtvs)
	{
		if (mtv.sName == sName)
		{
			mtv = tMtv;
			break;
		}
	}
}

/* ========================================
	�ŏ��ړ��x�N�g�����X�g�X�V�֐�
	-------------------------------------
	���e�F�ŏ��ړ��x�N�g�����X�g���X�V����
=========================================== */
void ComponentCollisionBase::UpdateMTVList()
{
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
		// �f�o�b�O�p�̏Փ˃I�u�W�F�N�g���X�g�ɒǉ�
		if (CHECK_DISP_COMP(m_sColCompName.c_str()))
		{
			DebugUI::Item* list = &WIN_OBJ_INFO[m_sColCompName.c_str()]["MTVs"];	// MTV���X�g�擾
			list->AddListItem(("MTV_" + mtv.sName).c_str());				// MTV���X�g�ɒǉ�
		}
#endif // _DEBUG
	}
}

/* ========================================
	�Q�b�^�[(���W)�֐�
	-------------------------------------
	�ߒl�F���W
=========================================== */
Vector3<float> ComponentCollisionBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	�Q�b�^�[(���W�I�t�Z�b�g)�֐�
	-------------------------------------
	�ߒl�F���W�I�t�Z�b�g
=========================================== */
Vector3<float> ComponentCollisionBase::GetPositionOffset()
{
	return m_vPositionOffset;
}

/* ========================================
	�Q�b�^�[(�X�P�[��)�֐�
	-------------------------------------
	�ߒl�F�X�P�[��
=========================================== */
Vector3<float> ComponentCollisionBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	�Q�b�^�[(��])�֐�
	-------------------------------------
	�ߒl�F��]
=========================================== */
Quaternion ComponentCollisionBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	�Q�b�^�[(�e�g�����X�t�H�[���Q�ƃt���O)�֐�
	-------------------------------------
	�ߒl�F�g�����X�t�H�[���Q�ƃt���O
=========================================== */
bool ComponentCollisionBase::GetRefOwnerTransform()
{
	return m_bRefOwnerTransform;
}

/* ========================================
	�Q�b�^�[(�R���W�������)�֐�
	-------------------------------------
	�ߒl�F�R���W�������
=========================================== */
ComponentCollisionBase::E_ColType ComponentCollisionBase::GetColType()
{
	return m_eColType;
}

/* ========================================
	�Q�b�^�[(���I�R���W�����t���O)�֐�
	-------------------------------------
	�ߒl�F���I�R���W�����t���O
=========================================== */
bool ComponentCollisionBase::GetDynamic()
{
	return m_bIsDynamic;
}

/* ========================================
	�Q�b�^�[(�L���t���O)�֐�
	-------------------------------------
	�ߒl�F�L���t���O
=========================================== */
bool ComponentCollisionBase::GetEnable()
{
	return m_bIsEnabled;
}

/* ========================================
	�Q�b�^�[(�g���K�[�t���O)�֐�
	-------------------------------------
	�ߒl�F�g���K�[�t���O
=========================================== */
bool ComponentCollisionBase::GetTrigger()
{
	return m_bIsTrigger;
}

/* ========================================
	�Q�b�^�[(�ŏ��ړ��x�N�g���\���̔z��)�֐�
	-------------------------------------
	�ߒl�Fvector<T_MTV>	�ŏ��ړ��x�N�g���\���̔z��
=========================================== */
std::vector<ComponentCollisionBase::T_MTV> ComponentCollisionBase::GetMtvs()
{
	return m_tMtvs;
}

/* ========================================
	�Q�b�^�[(�ŏ��ړ��x�N�g���\����)�֐�
	-------------------------------------
	����1�F����I�u�W�F�N�g��
	-------------------------------------
	�ߒl�FT_MTV	�ŏ��ړ��x�N�g���\����
=========================================== */
ComponentCollisionBase::T_MTV ComponentCollisionBase::GetMtv(std::string sName)
{
	for (const auto& mtv : m_tMtvs)
	{
		if (mtv.sName == sName)
		{
			return mtv;
		}
	}

	return T_MTV();
}


/* ========================================
	�Z�b�^�[(���W)�֐�
	-------------------------------------
	����1�F���W
=========================================== */
void ComponentCollisionBase::SetPosition(Vector3<float> vPos)
{
	m_vPosition = vPos;
}

/* ========================================
	�Z�b�^�[(���W�I�t�Z�b�g)�֐�
	-------------------------------------
	����1�F���W�I�t�Z�b�g
=========================================== */
void ComponentCollisionBase::SetPositionOffset(Vector3<float> vOffset)
{
	m_vPositionOffset = vOffset;
}

/* ========================================
	�Z�b�^�[(�X�P�[��)�֐�
	-------------------------------------
	����1�F�X�P�[��
=========================================== */
void ComponentCollisionBase::SetScale(Vector3<float> vScale)
{
	m_vScale = vScale;
}

/* ========================================
	�Z�b�^�[(��])�֐�
	-------------------------------------
	����1�F��]
=========================================== */
void ComponentCollisionBase::SetRotation(Quaternion qRot)
{
	m_qRotation = qRot;
}

/* ========================================
	�Z�b�^�[(�e�g�����X�t�H�[���Q�ƃt���O)�֐�
	-------------------------------------
	����1�F�g�����X�t�H�[���Q�ƃt���O
=========================================== */
void ComponentCollisionBase::SetRefOwnerTransform(bool bRef)
{
	m_bRefOwnerTransform = bRef;
}

/* ========================================
	�Z�b�^�[(�R���W�������)�֐�
	-------------------------------------
	����1�F�R���W�������
=========================================== */
void ComponentCollisionBase::SetColType(E_ColType eType)
{
	m_eColType = eType;
}

/* ========================================
	�Z�b�^�[(���I�R���W�����t���O)�֐�
	-------------------------------------
	����1�F���I�R���W�����t���O
=========================================== */
void ComponentCollisionBase::SetDynamic(bool bDynamic)
{
	m_bIsDynamic = bDynamic;
}

/* ========================================
	�Z�b�^�[(�L���t���O)�֐�
	-------------------------------------
	����1�F�L���t���O
=========================================== */
void ComponentCollisionBase::SetEnable(bool bEnable)
{
	m_bIsEnabled = bEnable;
}

/* ========================================
	�Z�b�^�[(�g���K�[�t���O)�֐�
	-------------------------------------
	����1�F�g���K�[�t���O
=========================================== */
void ComponentCollisionBase::SetTrigger(bool bTrigger)
{
	m_bIsTrigger = bTrigger;
}

/* ========================================
	�Z�b�^�[(�ŏ��ړ��x�N�g���\���̔z��)�֐�
	-------------------------------------
	�����F�ŏ��ړ��x�N�g���\���̔z��
=========================================== */
void ComponentCollisionBase::SetMtvs(std::vector<T_MTV> mtvs)
{
	m_tMtvs = mtvs;
}



#ifdef _DEBUG
/* ========================================
	�����蔻����f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�O���[�v����
	����2�F�R���|�[�l���g��(�f�o�b�O���j���[�\���m�F�p)
=========================================== */
void ComponentCollisionBase::DebugColBase(DebugUI::Item* pGroupItem, std::string sCompName)
{
	using namespace DebugUI;

	m_sColCompName = sCompName;

	// �g�����X�t�H�[��------------------------
	// �����I�u�W�F�N�g�g�����X�t�H�[���Q��
	pGroupItem->AddGroupItem(Item::CreateBind("IsOwnerRefTransform", Item::Kind::Bool, &m_bRefOwnerTransform));
	// ���W�I�t�Z�b�g
	pGroupItem->AddGroupItem(Item::CreateBind("PosOffset", Item::Kind::Vector, &m_vPositionOffset));
	// ��]
	// �\�������I�C���[�p�ɕϊ�(�N�H�[�^�j�I���͒��ړ��͂ł��Ȃ�����)
	pGroupItem->AddGroupItem(Item::CreateCallBack("Rotation  ", Item::Kind::Vector,
		[this](bool isWrite, void* arg)	// isWrite:���͂����邩�ǂ��� arg:���͒l
	{
		// ���͒l�A�\���l�̓f�O���[�A�����̓��W�A���ň���
		Vector3<float>* pDispDeg = static_cast<Vector3<float>*>(arg);

		// �l�ύX��
		if (isWrite)
		{
			// �N�H�[�^�j�I���ɕϊ����A�ϐ��Ɋi�[
			m_qRotation = Quaternion::FromEulerAngle(
				Vector3<float>(
					MathUtils::ToRadian(pDispDeg->x),
					MathUtils::ToRadian(pDispDeg->y),
					MathUtils::ToRadian(pDispDeg->z)
				));
		}
		else
		{
			// �N�H�[�^�j�I�����I�C���[�p�ɕϊ����A�\��
			*pDispDeg = Vector3<float>(
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().x),
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().y),
				MathUtils::ToDegree(m_qRotation.ToEulerAngle().z)
			);
		}
	}));
	// �傫��
	pGroupItem->AddGroupItem(Item::CreateBind("Scale  ", Item::Kind::Vector, &m_vScale));

	// --------------------------------------

	pGroupItem->AddGroupItem(Item::CreateBind("IsDispCollision",Item::Kind::Bool, &m_bIsDispCol));	// �R���W�����\���t���O
	pGroupItem->AddGroupItem(Item::CreateBind("IsDynamic",		Item::Kind::Bool, &m_bIsDynamic));	// ���I�R���W�����t���O
	pGroupItem->AddGroupItem(Item::CreateBind("IsEnabled",		Item::Kind::Bool, &m_bIsEnabled));	// �L���t���O
	pGroupItem->AddGroupItem(Item::CreateBind("IsTrigger",		Item::Kind::Bool, &m_bIsTrigger));	// �g���K�[�t���O

	// --------------------------------------

	// �ŏ��ړ��x�N�g���\����
	pGroupItem->AddGroupItem(Item::CreateValue("MTV_Name", Item::Kind::Text));	// �Փˑ���I�u�W�F�N�g��
	pGroupItem->AddGroupItem(Item::CreateValue("MTV_Axis", Item::Kind::Vector));	// �ŏ��ړ��x�N�g���̎�
	pGroupItem->AddGroupItem(Item::CreateValue("MTV_Overlap", Item::Kind::Float));	// �ŏ��ړ��x�N�g���̏d�Ȃ��
	pGroupItem->AddGroupItem(Item::CreateValue("MTV_IsCol", Item::Kind::Bool));		// �Փ˂��Ă��邩

	// �Փˍ\���̃��X�g�̃R�[���o�b�N�֐�
	Item::ConstCallback callback = [this](const void* arg)
	{
		std::string sObjName = reinterpret_cast<const char*>(arg);	// ���X�g���ږ�

		// ���X�g�ԍ��擾
		Item* pList = &WIN_OBJ_INFO[m_sColCompName.c_str()]["MTVs"];
		m_nSelectMTV = pList->GetListNo(sObjName.c_str());

		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_Name"].SetText(m_tMtvs.at(m_nSelectMTV).sName.c_str());	// �Փˑ���I�u�W�F�N�g��
	};

	// �ŏ��ړ��x�N�g���\���̔z��̕\��
	pGroupItem->AddGroupItem(Item::CreateList("MTVs", callback));

	m_pColObjList = Item::CreateList("CollObjectList", nullptr);	// �Փ˃I�u�W�F�N�g���X�g
	m_pColObjList->AddListItem("None");								// �f�t�H���g��None��ǉ�(��̏ꍇ�\������Ȃ�����
	pGroupItem->AddGroupItem(m_pColObjList);

}

/* ========================================
	�ŏ��ړ��x�N�g�����X�g�X�V�֐�
	-------------------------------------
	���e�F�ŏ��ړ��x�N�g�����X�g�̍X�V����
=========================================== */
void ComponentCollisionBase::UpdateDebugMTVList()
{
	// ���ݑI�𒆂̍ŏ��ړ��x�N�g�����擾
	if (m_tMtvs.size() > 0 && m_nSelectMTV != -1)
	{
		// �ŏ��ړ��x�N�g���\���̂̕\��
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_Axis"].SetVector(m_tMtvs.at(m_nSelectMTV).vAxis.ToXMFLOAT3());
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_Overlap"].SetFloat(m_tMtvs.at(m_nSelectMTV).fOverlap);
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_IsCol"].SetBool(m_tMtvs.at(m_nSelectMTV).bIsCol);
	}
	else
	{
		// MTV�\���̃��X�g����̏ꍇ�A�����l��\��
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_Axis"].SetVector(Vector3<float>(0.0f, 0.0f, 0.0f).ToXMFLOAT3());
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_Overlap"].SetFloat(0);
		WIN_OBJ_INFO[m_sColCompName.c_str()]["MTV_IsCol"].SetBool(false);
	}

	WIN_OBJ_INFO[m_sColCompName.c_str()]["MTVs"].RemoveListItemAll();	// MTV�\���̃��X�g���N���A

}
#endif // _DEBUG
