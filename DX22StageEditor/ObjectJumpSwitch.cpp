/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�W�����v�o�l�X�C�b�`)�pcpp
	------------------------------------
	ObjectJumpSwitch.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectJumpSwitch.h"
#include "ObjectJumpPad.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionAABB.h"
#include "ComponentPlayerController.h"

// =============== �萔��` =======================
const float POWER_RATE = 1.0f;

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================= */
ObjectJumpSwitch::ObjectJumpSwitch(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pJumpPad(nullptr)
	, m_pCompColAABB(nullptr)
	, m_fObjJumpPower(0.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================= */
void ObjectJumpSwitch::InitLocal()
{
	m_pCompColAABB = AddComponent<ComponentCollisionAABB>();
	m_pCompColAABB->SetTrigger(true);

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V�������s��
========================================= */
void ObjectJumpSwitch::UpdateLocal()
{
	// �W�����v�p�b�h�̎擾
	if (!m_pJumpPad)
		m_pJumpPad = static_cast<ObjectJumpPad*>(m_pParentObj->GetParentObject());

	// �W�����v�p�b�h�̏�ɔz�u
	float fOffset = m_pParentObj->GetTransform()->GetScale().y * 0.5f;
	Vector3<float> vPos = m_pParentObj->GetTransform()->GetPosition();
	vPos.y += fOffset;

	GetTransform()->SetPosition(vPos);
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectJumpSwitch::OnCollisionEnter(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	m_fObjJumpPower = pHit->GetComponent<ComponentPlayerController>()->GetJumpPower();
}

/* ========================================
	�Փ˔���(�p����)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�p����)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectJumpSwitch::OnCollisionStay(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	// �W�����v�͂̕ύX(�W�����v�o�l�̐L�k���ɉ����ĕω�)
	float fJumpBoost = m_fObjJumpPower * (POWER_RATE * (1.0f - m_pJumpPad->GetStretchRate()));
	pHit->GetComponent<ComponentPlayerController>()->SetJumpPower(m_fObjJumpPower + fJumpBoost);
	// �W�����v�p�b�h�I�u�W�F�N�g�̏�Ԃ��k�ނɕύX
	m_pJumpPad->SetSpringState(ObjectJumpPad::E_SpringState::Shrink);
}

/* ========================================
	�Փ˔���(�I����)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�I����)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectJumpSwitch::OnCollisionExit(ObjectBase* pHit)
{
	if (!pHit->CheckTag(E_ObjectTag::Player)) return;

	// �W�����v�͂����ɖ߂�
	pHit->GetComponent<ComponentPlayerController>()->SetJumpPower(m_fObjJumpPower);
	// �W�����v�p�b�h�I�u�W�F�N�g�̏�Ԃ�L�т�ɕύX
	m_pJumpPad->SetSpringState(ObjectJumpPad::E_SpringState::Stretch);
}
