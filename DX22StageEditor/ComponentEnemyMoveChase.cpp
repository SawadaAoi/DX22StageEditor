/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(�ǐ�)�pcpp
	------------------------------------
	ComponentEnemyMoveChase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveChase.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "ShapeLine.h"
#include "ColorVec3.h"

// =============== �萔��` =======================
const float DEFAULT_CHASE_START_DIST	= 10.0f;		// �ǐՊJ�n����
const float LIMIT_DIST_SQ_TO_PLYR		= 1.0f * 1.0f;	// �ړ���ɓ��B���鋗����2��
const float LIMIT_DIST_SQ_TO_START		= 0.5f * 0.5f;	// �ړ���ɓ��B���鋗����2��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================= */
ComponentEnemyMoveChase::ComponentEnemyMoveChase(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_pTargetObj(nullptr)
	, m_fLimitDistSq(LIMIT_DIST_SQ_TO_PLYR)
	, m_fChaseStartDist(DEFAULT_CHASE_START_DIST)
#ifdef _DEBUG
	, m_bDispDistLine(true)
#else
	, m_bDispDistLine(false)
#endif // _DEBUG
	, m_pDistLine(nullptr)
	, m_vStartPos(Vector3<float>::Zero())
	, m_qStartRot()
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ComponentEnemyMoveChase::Init()
{
	ComponentEnemyMoveBase::Init();

	m_pTargetObj = m_pOwnerObj->GetOwnerScene()->GetSceneObjectTag(E_ObjectTag::Player);
	m_pDistLine = std::make_unique<ShapeLine>(1);

	m_vStartPos = m_pCompTransform->GetPosition();
	m_qStartRot = m_pCompTransform->GetRotation();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ComponentEnemyMoveChase::Update()
{
#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveChase"))
	{
		WIN_OBJ_INFO["EnemyMoveChase"]["TargetObj"].SetText(m_pTargetObj ? m_pTargetObj->GetName() : "None");
		float fDist = m_pTargetObj ? 
			(m_pTargetObj->GetTransform()->GetPosition() - m_pCompTransform->GetPosition()).Length() : 0.0f;
		WIN_OBJ_INFO["EnemyMoveChase"]["DistanceToPlayer"].SetText(std::to_string(fDist));
	}
#endif // _DEBUG

	// �v���C���[�����Ȃ��ꍇ�͉������Ȃ�
	if (!m_pTargetObj)
	{
		m_pTargetObj = m_pOwnerObj->GetOwnerScene()->GetSceneObjectTag(E_ObjectTag::Player);
		return;
	}

	// �ړ�����
	Move();

	// �������C���`��
	if (m_bDispDistLine)
		DrawPlayerDistLine();
	
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================= */
void ComponentEnemyMoveChase::Draw()
{
	if (m_bDispDistLine)
	{
		m_pDistLine->Draw();
	}
}


/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
========================================= */
void ComponentEnemyMoveChase::Move()
{
	// �ǐՑΏۂƂ̋���
	float fDistSq = m_pTargetObj->GetTransform()->GetPosition().DistanceSq(m_pCompTransform->GetPosition());

	
	// �ǐՊJ�n�������߂Â�����ǐՊJ�n
	if(fDistSq < (m_fChaseStartDist * m_fChaseStartDist))
	{
		ChaseTarget();
	}
	// �ǐՊJ�n������艓���ꍇ�͏������W�Ɍ������Ĉړ�
	else
	{
		BackToStartPos();
	}

}

/* ========================================
	�ΏۃI�u�W�F�N�g�ǐՈړ��֐�
	-------------------------------------
	���e�F�ΏۃI�u�W�F�N�g��ǐՂ���
========================================= */
void ComponentEnemyMoveChase::ChaseTarget()
{
	// �v���C���[�̍��W���擾
	Vector3<float> vTargetPos = m_pTargetObj->GetTransform()->GetPosition();

	// �v���C���[�̕���
	Vector3<float> vDirTarget = vTargetPos - m_pCompTransform->GetPosition();
	vDirTarget.y = 0.0f;

	// �ǐՑΏۂƂ̋������ǐՌ��E������艓���ꍇ�͒ǐ�
	if (vDirTarget.LengthSq() > m_fLimitDistSq)
	{
		// �ǐՑΏۂ̕����֑��x��ݒ�(Y���͍l�����Ȃ�)
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = vDirTarget.GetNormalize().x * m_fMoveSpeed;
		vVelocity.z = vDirTarget.GetNormalize().z * m_fMoveSpeed;

		m_pCompRigidbody->SetVelocity(vVelocity);

	}
	else
	{
		// �ړ���~
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = 0.0f;
		vVelocity.z = 0.0f;
		m_pCompRigidbody->SetVelocity(vVelocity);
	}

	// �ǐՑΏۂ̕���������
	Vector3<float> vLook = vTargetPos;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	�ړ��J�n���W�A�Ҋ֐�
	-------------------------------------
	���e�F�ړ��J�n���W�ɖ߂�
========================================= */
void ComponentEnemyMoveChase::BackToStartPos()
{
	// �ړ��J�n���W�̕���
	Vector3<float> vDirStart = m_vStartPos - m_pCompTransform->GetPosition();
	vDirStart.y = 0.0f;

	//
	if (vDirStart.LengthSq() > LIMIT_DIST_SQ_TO_START)
	{
		// �ړ��J�n���W�����֑��x��ݒ�(Y���͍l�����Ȃ�)
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = vDirStart.GetNormalize().x * m_fMoveSpeed;
		vVelocity.z = vDirStart.GetNormalize().z * m_fMoveSpeed;

		m_pCompRigidbody->SetVelocity(vVelocity);

		// �ړ��J�n���W�̕���������
		Vector3<float> vLook = m_vStartPos;
		vLook.y = m_pCompTransform->GetPosition().y;
		m_pCompTransform->LookAt(vLook);
	}
	else
	{
		// �ړ���~
		Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
		vVelocity.x = 0.0f;
		vVelocity.z = 0.0f;
		m_pCompRigidbody->SetVelocity(vVelocity);

		// ��������������
		m_pCompTransform->SetLocalRotation(m_qStartRot);
	}
}

/* ========================================
	�������C���X�V�֐�
	-------------------------------------
	���e�F�v���C���[�Ƃ̋������C����`��
========================================= */
void ComponentEnemyMoveChase::DrawPlayerDistLine()
{
	// �v���C���[�̍��W���擾
	Vector3<float> vTargetPos = m_pTargetObj->GetComponent<ComponentTransform>()->GetPosition();

	// �������C���`��(�^�[�Q�b�g�̕����ցA�ǐՊJ�n�������̋���)
	Vector3<float> vStart = m_pCompTransform->GetPosition();				// �����̈ʒu
	Vector3<float> vDir = vTargetPos - m_pCompTransform->GetPosition();	// �v���C���[�̕���
	vDir.Normalize();															// ���K��
	Vector3<float> vEnd = vStart + (vDir * m_fChaseStartDist);					// �ǐՊJ�n�������̈ʒu

	// ������0.5f�ɌŒ�
	vStart.y = 0.5f;
	vEnd.y	 = 0.5f;

	m_pDistLine->UpdateLine(1, vStart, vEnd, ColorVec3::BLUE);
}

/* ========================================
	�Q�b�^�[(�ǐՊJ�n����)�֐�
	-------------------------------------
	�߂�l�Ffloat	�ǐՊJ�n����
=========================================== */
float ComponentEnemyMoveChase::GetChaseStartDist() const
{
	return m_fChaseStartDist;
}

/* ========================================
	�Q�b�^�[(�������C���\���t���O)�֐�
	-------------------------------------
	�߂�l�Fbool	�������C���\���t���O
=========================================== */
bool ComponentEnemyMoveChase::GetDispDistLine() const
{
	return m_bDispDistLine;
}

/* ========================================
	�Q�b�^�[(�ǐՌ��E����)�֐�
	-------------------------------------
	�߂�l�Ffloat	�ǐՌ��E����
=========================================== */
float ComponentEnemyMoveChase::GetLimitDistSq() const
{
	return m_fLimitDistSq;
}


/* ========================================
	�Z�b�^�[(�ǐՊJ�n����)�֐�
	-------------------------------------
	�����Ffloat	�ǐՊJ�n����
=========================================== */
void ComponentEnemyMoveChase::SetChaseStartDist(float fDist)
{
	m_fChaseStartDist = fDist;
}

/* ========================================
	�Z�b�^�[(�������C���\���t���O)�֐�
	-------------------------------------
	�����Fbool	�������C���\���t���O
=========================================== */
void ComponentEnemyMoveChase::SetDispDistLine(bool bDisp)
{
	m_bDispDistLine = bDisp;
}

/* ========================================
	�Z�b�^�[(�ǐՌ��E����)�֐�
	-------------------------------------
	�����Ffloat	�ǐՌ��E����
=========================================== */
void ComponentEnemyMoveChase::SetLimitDistSq(float fDist)
{
	m_fLimitDistSq = fDist;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveChase::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pEnemyMoveChaser = Item::CreateGroup("EnemyMoveChase");

	pEnemyMoveChaser->AddGroupItem(Item::CreateBind("MoveSpeed",			Item::Kind::Float, &m_fMoveSpeed));
	pEnemyMoveChaser->AddGroupItem(Item::CreateValue("TargetObj",			Item::Kind::Text));
	pEnemyMoveChaser->AddGroupItem(Item::CreateValue("DistanceToPlayer",	Item::Kind::Text));
	pEnemyMoveChaser->AddGroupItem(Item::CreateBind("ChaseStartDist",		Item::Kind::Float, &m_fChaseStartDist));

	window.AddItem(pEnemyMoveChaser);
}

#endif // _DEBUG