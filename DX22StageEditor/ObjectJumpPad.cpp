/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�W�����v�o�l)�pcpp
	------------------------------------
	ObjectJumpPad.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectJumpPad.h"
#include "ObjectJumpSwitch.h"
#include "SceneBase.h"

#include "ComponentModelStatic.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGroundBox.h"

#include "ModelStaticManager.h"

// =============== �萔��` =======================
const float				STRETCH_TIME	= 0.2f;						// �L�k����
const Vector3<float>	SPRING_SCALE	= { 0.8f, 0.8f, 0.8f };		// �o�l�̑傫��
const float				SWITCH_SCALE_XZ = 0.8f;						// �X�C�b�`��XZ�X�P�[��
const float				SWITCH_SCALE_Y	= 0.1f;						// �X�C�b�`��Y�X�P�[��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
========================================== */
ObjectJumpPad::ObjectJumpPad(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pSpringModelObj(nullptr)
	, m_pJumpSwitchObj(nullptr)
	, m_eSpringState(E_SpringState::Stay)
	, m_fStretchTime(0.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
========================================== */
void ObjectJumpPad::InitLocal()
{
	InitSpringModel();	// �o�l���f���̏�����
	InitJumpSwitch();	// �W�����v�X�C�b�`�̏�����
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V�������s��
========================================== */
void ObjectJumpPad::UpdateLocal()
{
	// �o�l�̐L�k����
	switch (m_eSpringState)
	{
	case ObjectJumpPad::Stay:		// ���̂܂�
		m_pSpringModelObj->GetTransform()->SetScale(SPRING_SCALE);
		m_pSpringModelObj->GetTransform()->SetPosition(GetTransform()->GetPosition());
		m_fStretchTime = STRETCH_TIME;
		return;
	case ObjectJumpPad::Stretch:	// �L�т�
		m_fStretchTime += DELTA_TIME;
		if (m_fStretchTime >= STRETCH_TIME)
		{
			m_eSpringState = ObjectJumpPad::Stay;
		}
		break;
	case ObjectJumpPad::Shrink:		// �k��
		m_fStretchTime -= DELTA_TIME;
		if (m_fStretchTime <= 0.0f)
		{
			m_fStretchTime = 0.0f;
		}
		break;
	}

	// �L�k�����v�Z
	float fRate = m_fStretchTime / STRETCH_TIME;

	// �����̔����͈͓̔��Ŋg�k
	float fScaleHalf	= SPRING_SCALE.y * 0.5f;		
	float fScaleY		= fScaleHalf + (fScaleHalf * fRate);
	m_pSpringModelObj->GetTransform()->SetScale({ SPRING_SCALE.x, fScaleY, SPRING_SCALE.z });

	// �΂˂̒�ʈʒu���ς��Ȃ��悤�ɍ��W�𒲐�
	float fOffsetY = (SPRING_SCALE.y - fScaleY) * 0.5f;	// �傫����ύX����������������������
	Vector3<float> vPos = GetTransform()->GetPosition();
	vPos.y -= fOffsetY;
	m_pSpringModelObj->GetTransform()->SetPosition(vPos);

}

/* ========================================
	�Q�b�^�[(�L�k��)�֐�
	-------------------------------------
	�ߒl�Ffloat �L�k��
=========================================== */
float ObjectJumpPad::GetStretchRate()
{
	return m_fStretchTime / STRETCH_TIME;
}

/* ========================================
	�Z�b�^�[(�o�l���)�֐�
	-------------------------------------
	�����FE_SpringState �o�l���
=========================================== */
void ObjectJumpPad::SetSpringState(E_SpringState eState)
{
	m_eSpringState = eState;
}

/* ========================================
	�o�l���f���������֐�
	-------------------------------------
	���e�F�o�l���f���̏��������s��
=========================================== */
void ObjectJumpPad::InitSpringModel()
{
	m_pSpringModelObj = m_pOwnerScene->AddSceneObject<ObjectBase>("SpringModel");
	m_pSpringModelObj->AddComponent<ComponentGroundBox>();

	// �ÓI���f��
	m_pSpringModelObj->AddComponent<ComponentModelStatic>()
		->SetModel(GET_MODEL_STATIC(MODEL_KEY::GIMMICK_SPRING));

	// �Փ˔���
	m_pSpringModelObj->AddComponent<ComponentCollisionAABB>();

	m_pSpringModelObj->GetTransform()->SetScale(SPRING_SCALE);
	m_pSpringModelObj->SetIsSave(false);
	m_pSpringModelObj->SetTag(E_ObjectTag::Ground);

	AddChildObject(m_pSpringModelObj);
}

/* ========================================
	�W�����v�X�C�b�`�������֐�
	-------------------------------------
	���e�F�W�����v�X�C�b�`�̏��������s��
=========================================== */
void ObjectJumpPad::InitJumpSwitch()
{
	m_pJumpSwitchObj = m_pOwnerScene->AddSceneObject<ObjectJumpSwitch>("JumpSwitchCol");
	m_pJumpSwitchObj->SetIsSave(false);
	
	Vector3<float> vScale = m_pSpringModelObj->GetTransform()->GetScale();
	vScale *= SWITCH_SCALE_XZ;
	vScale.y = SWITCH_SCALE_Y;
	m_pJumpSwitchObj->GetTransform()->SetScale(vScale);

	float fOffset = this->GetTransform()->GetScale().y * 0.5f;
	m_pJumpSwitchObj->GetTransform()->TranslateY(fOffset);
	m_pSpringModelObj->AddChildObject(m_pJumpSwitchObj);
}