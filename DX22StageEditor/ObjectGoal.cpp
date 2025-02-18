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
	, m_bOnFloatAnime(true)
	, m_vBasePos(Vector3<float>(0.0f, 0.0f, 0.0f))
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
	// �S�[���������\���ɂ���
	if (m_bIsGoal)
	{
		m_pModel->SetIsVisible(false);
	}

	if (!m_bOnFloatAnime) return;	// �A�j���[�V�����t���O�������Ă��Ȃ��ꍇ�͏������Ȃ�

	GetTransform()->RotateY(ROTATE_SPEED * DELTA_TIME);	// ��]

	// �㉺�ɂӂ�ӂ�Ɨh�炷
	m_fAnimeTimeCnt += DELTA_TIME;	// �A�j���[�V�������ԃJ�E���g

	Vector3<float> vMovePos = Vector3<float>::Zero();					// �ړ����W
	vMovePos.y += sinf(m_fAnimeTimeCnt * FLOAT_SPEED) * FLOAT_RANGE;	// �㉺�ړ�
	GetTransform()->SetLocalPosition(m_vBasePos + vMovePos);			// ���W�ݒ�
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

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectGoal::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// ��{���W
	data.vBasePos = m_vBasePos;

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectGoal::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// ��{���W
	m_vBasePos = data.vBasePos;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectGoal::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGoal = Item::CreateGroup("Goal");

	// ��{���W
	pGroupGoal->AddGroupItem(Item::CreateBind("BasePos", Item::Vector, &m_vBasePos));

	// ���V�A�j���[�V����
	pGroupGoal->AddGroupItem(Item::CreateBind("OnFloatAnime", Item::Bool, &m_bOnFloatAnime));

	window.AddItem(pGroupGoal);
}
#endif // _DEBUG