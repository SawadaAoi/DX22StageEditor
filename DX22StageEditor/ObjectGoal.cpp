/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�S�[��)�pcpp
	------------------------------------
	ObjectGoal.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGoal.h"
#include "ComponentCollisionSphere.h"
#include "ComponentModelStatic.h"
#include "ComponentTransform.h"

#include "ModelStaticManager.h"

// =============== �萔��` =======================
const float LIGHT_DIFFUSE	= 1.0f;	// �g�U���˗�
const float LIGHT_SPECULAR	= 2.5f;	// ���ʔ��˗�
const float LIGHT_SHININESS = 0.3f;	// �P���W��

const Vector3<float> POS_OFFSET = { 0.0f, 0.0f, 0.0f };	// ���f���I�t�Z�b�g

// �ړ��A�j��
const float ROTATE_SPEED	= 50.0f;	// ��]���x(1�b�Ԃ̉�]�p�x)
const float FLOAT_SPEED		= 2.0f;		// ���V���x
const float FLOAT_RANGE		= 0.1f;		// ���V�͈�


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectGoal::ObjectGoal(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_bIsGoal(false)
	, m_pModel(nullptr)
	, m_fAnimeTimeCnt(0.0f)
	, m_bIsSetBasePos(false)
{
	SetTag(E_ObjectTag::Goal);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ObjectGoal::InitLocal()
{
	AddComponent<ComponentCollisionSphere>()->SetTrigger(true);

	m_pModel = AddComponent<ComponentModelStatic>();
	m_pModel->SetModel(GET_MODEL_STATIC(MODEL_KEY::GOAL_NIKUKYU));
	m_pModel->SetPosOffset(POS_OFFSET);

	// �}�e���A���ݒ�
	SetLightMaterial(LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_SHININESS);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ObjectGoal::UpdateLocal()
{
	// ����W�ݒ�
	if (!m_bIsSetBasePos) 
	{
		m_vBasePos = GetTransform()->GetPosition();	// ����W�ݒ�
		m_bIsSetBasePos = true;
	}

	// �S�[���������\���ɂ���
	if (m_bIsGoal)
	{
		m_pModel->SetIsVisible(false);
	}

	GetTransform()->RotateY(ROTATE_SPEED * DELTA_TIME);	// ��]

	// �㉺�ɂӂ�ӂ�Ɨh�炷
	m_fAnimeTimeCnt += DELTA_TIME;	// �A�j���[�V�������ԃJ�E���g

	Vector3<float> vPos = m_vBasePos;		// ���݂̍��W�擾
	vPos.y += sinf(m_fAnimeTimeCnt * FLOAT_SPEED) * FLOAT_RANGE;	// �㉺�ړ�
	GetTransform()->SetPosition(vPos);							// ���W�ݒ�
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectGoal::OnCollisionEnter(ObjectBase* pHit)
{
	// �v���C���[�ƐڐG������S�[���t���O�𗧂Ă�
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		m_bIsGoal = true;
	}
}

/* ========================================
	�Q�b�^�[(�S�[���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�S�[���t���O
=========================================== */
bool ObjectGoal::GetIsGoal()
{
	return m_bIsGoal;
}

/* ========================================
	�Z�b�^�[(�S�[���t���O)�֐�
	-------------------------------------
	�����Fbool	�S�[���t���O
=========================================== */
void ObjectGoal::SetIsGoal(bool bIsGoal)
{
	m_bIsGoal = bIsGoal;
}
