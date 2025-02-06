/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�ړ������G�t�F�N�g)�pcpp
	------------------------------------
	ComponentSmokeEffect.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentSmokeEffect.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "ObjectBase.h"
#include "SceneBase.h"
#include "TextureManager.h"
#include "MathUtils.h"

// =============== �萔��` =======================
const float SMOKE_CREATE_INTERVAL	= 0.1f;	// �������Ԋu
const float SMOKE_LIFE_TIME			= 0.5f;	// ���̎���

// �������p�����[�^
// �傫��
const float SMOKE_SCALE_MIN = 0.2f;
const float SMOKE_SCALE_MAX = 0.8f;

// ���W
const float SMOKE_POS_ADJUST_MIN = -0.25f;
const float SMOKE_POS_ADJUST_MAX = 0.25f;

// �I�u�W�F�N�g�Ɖ������ʒu�̋���
const float SMOKE_CREATE_POS_DIST_XZ	= 0.5f;
const float SMOKE_CREATE_POS_DIST_Y		= -0.25f;

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
=========================================== */
ComponentSmokeEffect::ComponentSmokeEffect(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderSmoke)
	, m_pScene(nullptr)
	, m_fCreateItvTimeCnt(0.0f)
	, m_vMoveDir(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_vCreatePosDist(Vector3<float>(SMOKE_CREATE_POS_DIST_XZ, SMOKE_CREATE_POS_DIST_Y, SMOKE_CREATE_POS_DIST_XZ))
	, m_fLifeTime(SMOKE_LIFE_TIME)
	, m_fCreateInterval(SMOKE_CREATE_INTERVAL)
	, m_fScaleMin(SMOKE_SCALE_MIN)
	, m_fScaleMax(SMOKE_SCALE_MAX)
	, m_fPosAdjustMin(SMOKE_POS_ADJUST_MIN)
	, m_fPosAdjustMax(SMOKE_POS_ADJUST_MAX)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void ComponentSmokeEffect::Init()
{
	m_pScene = m_pOwnerObj->GetOwnerScene();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ComponentSmokeEffect::Update()
{
	// ���̍X�V
	for (auto& smoke : m_Smokes)
	{
		smoke.second += DELTA_TIME;

		// ��������Ă����莞�Ԍo�߂�����폜
		if (smoke.second >= m_fLifeTime)
		{
			smoke.first->SetState(ObjectBase::E_State::STATE_DEAD);
			smoke.first = nullptr;
		}
	}

	DeleteSmoke();	// �Y���̉��폜

	// �I�u�W�F�N�g���ړ����Ă��Ȃ��ꍇ�͏������Ȃ�
	if (m_vMoveDir == Vector3<float>::Zero()) return;

	// ���Ԋu�ŉ�����
	m_fCreateItvTimeCnt += DELTA_TIME;
	if (m_fCreateItvTimeCnt >= m_fCreateInterval)
	{
		CreateSmoke();
		m_fCreateItvTimeCnt = 0.0f;
	}


}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g����(�r���{�[�h)
		�@�傫���A���W�������_��
=========================================== */
void ComponentSmokeEffect::CreateSmoke()
{
	ObjectBase* pSmoke = m_pScene->AddSceneObject<ObjectBase>("Smoke");

	// �傫���ݒ�
	float scale = MathUtils::Random(m_fScaleMin, m_fScaleMax);
	pSmoke->GetTransform()->SetScale(Vector3<float>(scale, scale, scale));

	// ���W�ݒ�
	// �ړ������̋t�����ɂ��炷
	Vector3<float> vMoveDirInv	= m_vMoveDir * -1.0f;							// �ړ��t����
	Vector3<float> vCreatePos	= m_pOwnerObj->GetTransform()->GetPosition();	// �I�u�W�F�N�g�̍��W���擾

	vCreatePos.x += vMoveDirInv.x * m_vCreatePosDist.x;
	vCreatePos.y += m_vCreatePosDist.y;
	vCreatePos.z += vMoveDirInv.z * m_vCreatePosDist.z;

	// ���W�̂��炵�l
	Vector3<float> vAdjustPos = Vector3<float>::Zero();
	vAdjustPos.x = MathUtils::Random(m_fPosAdjustMin, m_fPosAdjustMax);
	vAdjustPos.z = MathUtils::Random(m_fPosAdjustMin, m_fPosAdjustMax);

	// ���炵�����W�Ƀ����_���Ȓl�����Z
	vCreatePos += vAdjustPos;	

	// ���W�ݒ�
	pSmoke->GetTransform()->SetPosition(vCreatePos);

	// �e�N�X�`���ݒ�
	pSmoke->AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_SMOKE));

	// ���������������X�g�ɒǉ�
	m_Smokes.push_back(std::make_pair(pSmoke, 0.0f));
}

/* ========================================
	���폜�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�폜
=========================================== */
void ComponentSmokeEffect::DeleteSmoke()
{
	// �������������̂��폜
	// nullptr ���w���Ă���v�f���폜����
	auto newEnd = std::remove_if(m_Smokes.begin(), m_Smokes.end(),
		[](const std::pair<ObjectBase*, float>& smoke)
	{
		return smoke.first == nullptr;
	});

	// remove_if �ɂ���ĕs�v�ȗv�f�������Ɉړ�����Ă��邽�߁A������폜����
	m_Smokes.erase(newEnd, m_Smokes.end());
}



/* ========================================
	�Z�b�^�[(�ړ�����)�֐�
	-------------------------------------
	�����FVector3<float> vMoveDir
=========================================== */
void ComponentSmokeEffect::SetMoveDir(Vector3<float> vMoveDir)
{
	m_vMoveDir = vMoveDir;
}

/* ========================================
	�Z�b�^�[(�����ʒu����)�֐�
	-------------------------------------
	�����FVector3<float> vCreatePosDist
=========================================== */
void ComponentSmokeEffect::SetCreatePosDist(Vector3<float> vCreatePosDist)
{
	m_vCreatePosDist = vCreatePosDist;
}

/* ========================================
	�Z�b�^�[(�����Ԋu)�֐�
	-------------------------------------
	�����Ffloat fCreateInterval
=========================================== */
void ComponentSmokeEffect::SetCreateInterval(float fCreateInterval)
{
	m_fCreateInterval = fCreateInterval;
}

/* ========================================
	�Z�b�^�[(���̎���)�֐�
	-------------------------------------
	�����Ffloat fLifeTime
=========================================== */
void ComponentSmokeEffect::SetLifeTime(float fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

/* ========================================
	�Z�b�^�[(�X�P�[��)�֐�
	-------------------------------------
	����1�Ffloat fScaleMin
	����2�Ffloat fScaleMax
=========================================== */
void ComponentSmokeEffect::SetScale(float fScaleMin, float fScaleMax)
{
	m_fScaleMin = fScaleMin;
	m_fScaleMax = fScaleMax;
}

/* ========================================
	�Z�b�^�[(�ʒu����)�֐�
	-------------------------------------
	����1�Ffloat fPosAdjustMin
	����2�Ffloat fPosAdjustMax
=========================================== */
void ComponentSmokeEffect::SetPosAdjust(float fPosAdjustMin, float fPosAdjustMax)
{
	m_fPosAdjustMin = fPosAdjustMin;
	m_fPosAdjustMax = fPosAdjustMax;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentSmokeEffect::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupSmokeEffect = Item::CreateGroup("SmokeEffect");

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreateInterval", Item::Kind::Float, &m_fCreateInterval));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreateItvTimeCnt", Item::Kind::Float, &m_fCreateItvTimeCnt));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("LifeTime", Item::Kind::Float, &m_fLifeTime));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("CreatePosDist", Item::Kind::Vector, &m_vCreatePosDist));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("MoveDir", Item::Kind::Vector, &m_vMoveDir));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("ScaleMin", Item::Kind::Float, &m_fScaleMin));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("ScaleMax", Item::Kind::Float, &m_fScaleMax));

	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("PosAdjustMin", Item::Kind::Float, &m_fPosAdjustMin));
	pGroupSmokeEffect->AddGroupItem(Item::CreateBind("PosAdjustMax", Item::Kind::Float, &m_fPosAdjustMax));

	window.AddItem(pGroupSmokeEffect);

}
#endif // _DEBUG