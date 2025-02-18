/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�X�e�[�W�M�~�b�N�����ړ�)�pcpp
	------------------------------------
	ComponentGimmickMoveLinear.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentGimmickMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include <format>
#include "ShapeLine.h"
#include "ColorVec3.h"

// =============== �萔��` =======================
const float LIMIT_DISTANCE_SQ = 0.01f * 0.01f;	// �ړ���ɓ��B���鋗����2��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�҃I�u�W�F�N�g
========================================== */
ComponentGimmickMoveLinear::ComponentGimmickMoveLinear(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderGimmick)
	, m_pCompTransform(nullptr)
	//, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(0.1f)
	, m_vtWayPoints()
	, m_nCurrentWayPoint(0)
	, m_bIsReverse(false)
#ifdef _DEBUG
	, m_bDispMoveLine(true)
	, m_nSelectWayPointIdx(0)
#else
	, m_bDispMoveLine(false)
#endif // _DEBUG
	, m_pMoveLine(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ComponentGimmickMoveLinear::Init()
{
	m_pCompTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pMoveLine = std::make_unique<ShapeLine>(10);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ComponentGimmickMoveLinear::Update()
{
	// �ړ����W��1�ȉ��̏ꍇ�͏������s��Ȃ�
	if (m_vtWayPoints.size() <= 1)
	{
		return;
	}

	// �t���t���O�������Ă���ꍇ�͋t���Ɉړ�
	if (m_bIsReverse)
		ReverseMove();
	else
		Move();


	if (m_bDispMoveLine)
	{	// �ړ����C���̍X�V
		for (int i = 0; i < m_vtWayPoints.size(); i++)
		{
			int nNextIndex = (i + 1) % m_vtWayPoints.size();	// �Ō�̍��W�̏ꍇ�͍ŏ��̍��W�ɖ߂�
			Vector3<float> vStart = m_vtWayPoints[i];
			Vector3<float> vEnd = m_vtWayPoints[nNextIndex];
			m_pMoveLine->UpdateLine(i + 1, vStart, vEnd, ColorVec3::BLUE);
		}
	}

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================= */
void ComponentGimmickMoveLinear::Draw()
{
	if (m_bDispMoveLine)
	{
		m_pMoveLine->Draw();
	}
}

/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
========================================= */
void ComponentGimmickMoveLinear::Move()
{
	// ���݂̍��W�ԍ��̍��W���擾
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// �ړ���Ƃ̃x�N�g�����v�Z
	Vector3<float> vDir = vCurrentWayPoint - m_pCompTransform->GetPosition();

	// �ړ���ɓ��B���Ă���ꍇ
	if (vDir.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		m_nCurrentWayPoint++;

		// �Ō�̍��W�ɓ��B�����ꍇ
		if (m_nCurrentWayPoint >= m_vtWayPoints.size())
		{
			m_nCurrentWayPoint = 0;	// �ŏ��̍��W�ɖ߂�
		}
	}
	else
	{
		m_pCompTransform->Translate(vDir.GetNormalize() * m_fMoveSpeed * DELTA_TIME);
	}

}

/* ========================================
	�t�ړ��֐�
	-------------------------------------
	���e�F�t���Ɉړ�����
========================================= */
void ComponentGimmickMoveLinear::ReverseMove()
{
	// ���݂̍��W�ԍ��̍��W���擾
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// �ړ���Ƃ̃x�N�g�����v�Z
	Vector3<float> vDir = vCurrentWayPoint - m_pCompTransform->GetPosition();

	// �ړ���ɓ��B���Ă���ꍇ
	if (vDir.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		m_nCurrentWayPoint--;

		// �ŏ��̍��W�ɓ��B�����ꍇ
		if (m_nCurrentWayPoint < 0)
		{
			m_nCurrentWayPoint = m_vtWayPoints.size() - 1;	// �Ō�̍��W�ɖ߂�
		}
	}
	else
	{
		m_pCompTransform->Translate(vDir.GetNormalize() * m_fMoveSpeed * DELTA_TIME);

	}

	// �ړ���̍��W������(�����͍l�����Ȃ�)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}


/* ========================================
	�ړ����W�ǉ��֐�
	-------------------------------------
	���e�F�ړ����W��ǉ�
	-------------------------------------
	����1�F�ړ����W
========================================= */
void ComponentGimmickMoveLinear::AddWayPoint(const Vector3<float>& vWayPoint)
{
	m_vtWayPoints.push_back(vWayPoint);

	// �ړ����W��1�̏ꍇ�͍��W��ݒ�
	if (m_vtWayPoints.size() == 1)
	{
		m_pCompTransform->SetPosition(vWayPoint);
	}

	m_nCurrentWayPoint = 0;

#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// �ړ����W���X�g�̍X�V
		InitDebugWayPointList();
		m_nSelectWayPointIdx = m_nCurrentWayPoint;
	}
#endif // _DEBUG

}

/* ========================================
	�ړ����W�}���֐�
	-------------------------------------
	���e�F�w�肵���C���f�b�N�X�Ɉړ����W��ǉ�
	-------------------------------------
	����1�F�ړ����W
	����2�F�C���f�b�N�X
========================================= */
void ComponentGimmickMoveLinear::InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex)
{
	// �C���f�b�N�X���͈͊O�̏ꍇ�͒ǉ����Ȃ�
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.insert(m_vtWayPoints.begin() + nIndex, vWayPoint);
#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// �ړ����W���X�g�̍X�V
		InitDebugWayPointList();
	}
#endif // _DEBUG
}


/* ========================================
	�ړ����W�폜�֐�
	-------------------------------------
	���e�F�w�肵���C���f�b�N�X�̈ړ����W���폜
	-------------------------------------
	����1�F�C���f�b�N�X
========================================= */
void ComponentGimmickMoveLinear::RemoveWayPoint(int nIndex)
{
	// �ړ����C���̍X�V
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		m_pMoveLine->UpdateLine(i + 1, Vector3<float>::Zero(), Vector3<float>::Zero());
	}

	// �C���f�b�N�X���͈͊O�̏ꍇ�͍폜���Ȃ�
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.erase(m_vtWayPoints.begin() + nIndex);

	// �C���f�b�N�X���Ō�̍��W�̏ꍇ�͍Ō�̍��W�ɖ߂�
	if (m_nCurrentWayPoint >= m_vtWayPoints.size())
	{
		m_nCurrentWayPoint = m_vtWayPoints.size() - 1;
	}

#ifdef _DEBUG
	if (CHECK_DISP_COMP("GimmickMoveLinear"))
	{
		// �ړ����W���X�g�̍X�V
		InitDebugWayPointList();
		m_nSelectWayPointIdx = m_nCurrentWayPoint;
	}
#endif // _DEBUG
}


/* ========================================
	�Q�b�^�[(�ړ����x)�֐�
	-------------------------------------
	�ߒl�F�ړ����x
=========================================== */
float ComponentGimmickMoveLinear::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	�Q�b�^�[(�ړ����W�z��)�֐�
	-------------------------------------
	�ߒl�Fstd::vector<Vector3<float>>& �ړ����W�z��
=========================================== */
std::vector<Vector3<float>>& ComponentGimmickMoveLinear::GetWayPoints()
{
	return m_vtWayPoints;
}

/* ========================================
	�Q�b�^�[(�t���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�t���t���O
=========================================== */
bool ComponentGimmickMoveLinear::GetIsReverse()
{
	return m_bIsReverse;
}

/* ========================================
	�Z�b�^�[(�ړ����x)�֐�
	-------------------------------------
	����1�Ffloat	�ړ����x
=========================================== */
void ComponentGimmickMoveLinear::SetMoveSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}

/* ========================================
	�Z�b�^�[(�t���t���O)�֐�
	-------------------------------------
	����1�Fbool	�t���t���O
=========================================== */
void ComponentGimmickMoveLinear::SetIsReverse(bool bIsReverse)
{
	m_bIsReverse = bIsReverse;
}



#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentGimmickMoveLinear::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupMoveLinear = Item::CreateGroup("GimmickMoveLinear");

	pGroupMoveLinear->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	pGroupMoveLinear->AddGroupItem(Item::CreateBind("Reverse", Item::Kind::Bool, &m_bIsReverse));
	pGroupMoveLinear->AddGroupItem(Item::CreateBind("DispMoveLine", Item::Kind::Bool, &m_bDispMoveLine, false, true));
	pGroupMoveLinear->AddGroupItem(Item::CreateValue("WayPointNum", Item::Kind::Text));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("CurrentWayPoint", Item::Kind::Int,		// ���݂̍��W�ԍ�
		[this](bool isWrite, void* arg) { FuncWayCurrent(isWrite, arg); }));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("AddWayPoint", Item::Kind::Command,		// �ړ����W�ǉ�
		[this](bool isWrite, void* arg) { AddWayPoint(m_pCompTransform->GetPosition()); }));
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("RemoveWayPoint", Item::Kind::Command, 	// �ړ����W�폜
		[this](bool isWrite, void* arg) { RemoveWayPoint(m_nSelectWayPointIdx); }, false, true));

	// �ړ����W
	pGroupMoveLinear->AddGroupItem(Item::CreateCallBack("WayPoint", Item::Kind::Vector,
		[this](bool isWrite, void* arg) { FuncWayPoint(isWrite, arg); }));

	// �ړ����W���X�g
	pGroupMoveLinear->AddGroupItem(Item::CreateList("WayPointList", [this](const void* arg)
	{
		std::string sName = reinterpret_cast<const char*>(arg);
		m_nSelectWayPointIdx = WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].GetListNo(sName.c_str());

	}));

	window.AddItem(pGroupMoveLinear);
	InitDebugWayPointList();

	m_nSelectWayPointIdx = 0;
}

/* ========================================
	�ړ����W���X�g�������֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̈ړ����W���X�g��������
=========================================== */
void ComponentGimmickMoveLinear::InitDebugWayPointList()
{
	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].RemoveListItemAll();
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		// ���W�𕶎���ɕϊ�
		std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
		std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
		std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

		// ���X�g�ɒǉ�
		WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
	}

	WIN_OBJ_INFO["GimmickMoveLinear"]["WayPointList"].SetListNo(m_nSelectWayPointIdx);
}

/* ========================================
	�f�o�b�O���ڃN���b�N(���W�ԍ�)�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̍��W�ԍ����N���b�N�������̏���
	-------------------------------------
	����1�F�������݃t���O
	����2�F����
=========================================== */
void ComponentGimmickMoveLinear::FuncWayCurrent(bool isWrite, void* arg)
{
	int nWayPoint = *reinterpret_cast<int*>(arg);	// �����̍��W�ԍ����擾
	if (isWrite)
	{
		m_nCurrentWayPoint = nWayPoint;
		// �͈͊O�̏ꍇ�͔͈͓��Ɏ��߂�
		if (m_nCurrentWayPoint >= m_vtWayPoints.size())
			m_nCurrentWayPoint = m_vtWayPoints.size() - 1;
		else if (m_nCurrentWayPoint < 0)
			m_nCurrentWayPoint = 0;
	}
	else
	{
		*reinterpret_cast<int*>(arg) = m_nCurrentWayPoint;
	}
}

/* ========================================
	�f�o�b�O���ڃN���b�N(���W)�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̍��W���N���b�N�������̏���
	-------------------------------------
	����1�F�������݃t���O
	����2�F����
=========================================== */
void ComponentGimmickMoveLinear::FuncWayPoint(bool isWrite, void* arg)
{
	if (m_vtWayPoints.empty()) return;
	Vector3<float>* pvPos = reinterpret_cast<Vector3<float>*>(arg);

	if (isWrite)
	{
		m_vtWayPoints[m_nSelectWayPointIdx] = *pvPos;	// �ύX�������W��ݒ�
		InitDebugWayPointList();						// �ړ����W���X�g�̍X�V

	}
	else
	{
		*pvPos = m_vtWayPoints[m_nSelectWayPointIdx];
	}
}

#endif // _DEBUG
