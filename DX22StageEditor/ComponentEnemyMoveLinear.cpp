/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(����)�pcpp
	------------------------------------
	ComponentEnemyMoveLinear.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include <format>
#include "ShapeLine.h"

// =============== �萔��` =======================
const float LIMIT_DISTANCE_SQ = 0.3f * 0.3f;	// �ړ���ɓ��B���鋗����2��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�҃I�u�W�F�N�g
========================================== */
ComponentEnemyMoveLinear::ComponentEnemyMoveLinear(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_vtWayPoints()
	, m_nCurrentWayPoint(0)
	, m_bIsReverse(false)
#ifdef _DEBUG
	, m_bDispMoveLine(true)
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
======================================= */
void ComponentEnemyMoveLinear::Init()
{
	ComponentEnemyMoveBase::Init();	// ���N���X�̏������������Ăяo��

	m_pMoveLine = std::make_unique<ShapeLine>(10);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================= */
void ComponentEnemyMoveLinear::Update()
{
	// �ړ����W���Ȃ��ꍇ�͉������Ȃ�
	if (m_vtWayPoints.empty())
	{
		return;
	}

	// �t���t���O�������Ă���ꍇ�͋t���Ɉړ�
	if (m_bIsReverse)
		ReverseMove();
	else
		Move();

	
	// �ړ����C���̍X�V
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		int nNextIndex = (i + 1) % m_vtWayPoints.size();	// �Ō�̍��W�̏ꍇ�͍ŏ��̍��W�ɖ߂�
		m_pMoveLine->UpdateLine(i + 1, m_vtWayPoints[i], m_vtWayPoints[nNextIndex], Vector3<float>(0.0f, 0.0f, 1.0f));
	}

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
======================================= */
void ComponentEnemyMoveLinear::Draw()
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
======================================= */
void ComponentEnemyMoveLinear::Move()
{	
	// ���݂̍��W�ԍ��̍��W���擾
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// �ړ���Ƃ̋������v�Z
	Vector3<float> vDistance = vCurrentWayPoint - m_pCompTransform->GetWorldPosition();

	// �ړ���ɓ��B���Ă���ꍇ
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
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
		// �ړ���Ɍ������Ĉړ�
		m_pCompRigidbody->SetVelocity(vDistance.GetNormalize() * m_fMoveSpeed);

	}

	// �ړ���̍��W������(�����͍l�����Ȃ�)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetWorldPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	�t�ړ��֐�
	-------------------------------------
	���e�F�t���Ɉړ�����
======================================= */
void ComponentEnemyMoveLinear::ReverseMove()
{
	// ���݂̍��W�ԍ��̍��W���擾
	Vector3<float> vCurrentWayPoint = m_vtWayPoints[m_nCurrentWayPoint];

	// �ړ���Ƃ̋������v�Z
	Vector3<float> vDistance = vCurrentWayPoint - m_pCompTransform->GetWorldPosition();

	// �ړ���ɓ��B���Ă���ꍇ
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
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
		// �ړ���Ɍ������Ĉړ�
		m_pCompRigidbody->SetVelocity(vDistance.GetNormalize() * m_fMoveSpeed);

	}

	// �ړ���̍��W������(�����͍l�����Ȃ�)
	Vector3<float> vLook = vCurrentWayPoint;
	vLook.y = m_pCompTransform->GetWorldPosition().y;
	m_pCompTransform->LookAt(vLook);
}


/* ========================================
	�ړ����W�ǉ��֐�
	-------------------------------------
	���e�F�ړ����W��ǉ�
	-------------------------------------
	����1�F�ړ����W
======================================= */
void ComponentEnemyMoveLinear::AddWayPoint(const Vector3<float>& vWayPoint)
{
	m_vtWayPoints.push_back(vWayPoint);

	// �ړ����W��1�̏ꍇ�͍��W��ݒ�
	if (m_vtWayPoints.size() == 1)
	{
		m_pCompTransform->SetLocalPosition(vWayPoint);
	}

#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveLinear"))
	{
		// �ړ����W���X�g�̍X�V
		WIN_OBJ_INFO["EnemyMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
		WIN_OBJ_INFO["EnemyMoveLinear"]["WayPoints"].RemoveListItemAll();
		for (int i = 0; i < m_vtWayPoints.size(); i++)
		{// ���W�𕶎���ɕϊ�
			std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
			std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
			std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

			// ���X�g�ɒǉ�
			WIN_OBJ_INFO["EnemyMoveLinear"]["WayPoints"].AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
		}
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
======================================= */
void ComponentEnemyMoveLinear::InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex)
{
	// �C���f�b�N�X���͈͊O�̏ꍇ�͒ǉ����Ȃ�
	if (nIndex < 0 || nIndex >= m_vtWayPoints.size())
	{
		return;
	}

	m_vtWayPoints.insert(m_vtWayPoints.begin() + nIndex, vWayPoint);
}




#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveLinear::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pEnemyMoveLinear = Item::CreateGroup("EnemyMoveLinear");

	pEnemyMoveLinear->AddGroupItem(Item::CreateValue("WayPointNum", Item::Kind::Text));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("CurrentWayPoint", Item::Kind::Int, &m_nCurrentWayPoint));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("Reverse", Item::Kind::Bool, &m_bIsReverse));
	pEnemyMoveLinear->AddGroupItem(Item::CreateBind("DispMoveLine", Item::Kind::Bool, &m_bDispMoveLine));

	// �ړ����W���X�g
	Item* WayList = Item::CreateList("WayPoints", nullptr,false);

	// ���X�g�ɒǉ�
	for (int i = 0; i < m_vtWayPoints.size(); i++)
	{
		// ���W�𕶎���ɕϊ�
		std::string strX = " X:" + std::format("{:.1f}", m_vtWayPoints[i].x);
		std::string strY = " Y:" + std::format("{:.1f}", m_vtWayPoints[i].y);
		std::string strZ = " Z:" + std::format("{:.1f}", m_vtWayPoints[i].z);

		// ���X�g�ɒǉ�
		WayList->AddListItem((std::to_string(i) + "=" + strX + strY + strZ).c_str());
	}

	pEnemyMoveLinear->AddGroupItem(WayList);

	window.AddItem(pEnemyMoveLinear);

	WIN_OBJ_INFO["EnemyMoveLinear"]["WayPointNum"].SetText(std::to_string(m_vtWayPoints.size()).c_str());
}

#endif // _DEBUG
