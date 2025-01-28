/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(�����_��)�pcpp
	------------------------------------
	ComponentEnemyMoveRandom.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ColorVec3.h"
#include "ComponentEnemyMoveRandom.h"
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"
#include "MathUtils.h"
#include "ObjectBase.h"
#include "ShapeLine.h"
#include "ShapeSphere.h"
#include <format>
#include "ColorVec3.h"

// =============== �萔��` =======================
const float DEFAULT_TARGET_SWITCH_TIME = 2.0f;			// �ړI���W�؂�ւ�����
const float DEFAULT_MOVE_RECT_SIZE = 5.0f;			// �ړ��͈͂̃T�C�Y
const float LIMIT_DISTANCE_SQ = 0.5f * 0.5f;	// �ړ���ɓ��B���鋗����2��
const int	LIMIT_RECT_POINT_NUM = 4;			// �ړ��͈͂̒��_��

// �ړI���W�`��
const Vector3<float> TARGET_COLOR = ColorVec3::BLUE;
const Vector3<float> TARGET_SPHERE_SCALE = { 0.2f, 0.2f, 0.2f };

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�҃I�u�W�F�N�g
========================================== */
ComponentEnemyMoveRandom::ComponentEnemyMoveRandom(ObjectBase* pOwner)
	:ComponentEnemyMoveBase(pOwner)
	, m_vTargetPos()
	, m_fTargetSwitchTime(DEFAULT_TARGET_SWITCH_TIME)
	, m_fTargetSwitchCnt(0.0f)
#ifdef _DEBUG
	, m_bDispMoveLimitRect(true)
	, m_bMoveStop(false)
#else
	, m_bDispMoveLimitRect(false)
#endif // _DEBUG
	, m_vRectCenter()
	, m_vtMoveLimitRectLocal()
	, m_vtMoveLimitRectWorld()
	, m_pRectLine(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================= */
void ComponentEnemyMoveRandom::Init()
{
	ComponentEnemyMoveBase::Init();

	// �ړ��͈͂̏�����
	SetMoveLimitRectSquare(DEFAULT_MOVE_RECT_SIZE);

	// �ړ��͈͕`��p�̃��C���𐶐�
	m_pRectLine = std::make_unique<ShapeLine>(4);

	// �ړI���W�`��p�̋��𐶐�
	m_pTargetSphere = std::make_unique<ShapeSphere>();
	m_pTargetSphere->SetScale(TARGET_SPHERE_SCALE);
	m_pTargetSphere->SetColor(TARGET_COLOR);
	m_pTargetSphere->SetDrawMode(ShapeSphere::E_DrawMode::DRAW_NORMAL);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================= */
void ComponentEnemyMoveRandom::Update()
{
#ifdef _DEBUG
	if (m_bMoveStop) return;
#endif // _DEBUG

	// �ړI���W�؂�ւ����Ԃ��J�E���g
	m_fTargetSwitchCnt += DELTA_TIME;

	// �ړI���W�؂�ւ����ԂɂȂ�����ړI���W��ύX
	if (m_fTargetSwitchCnt >= m_fTargetSwitchTime)
	{
		SwitchTargetPos();
	}

	// �ړ�����
	Move();

	// �ړ��͈͍��W�̍X�V
	UpdateMoveLimitRectWorldPos();

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
======================================= */
void ComponentEnemyMoveRandom::Draw()
{
	if (m_bDispMoveLimitRect)
	{
		m_pRectLine->Draw();
		m_pTargetSphere->Draw();
	}
}

/* ========================================
	�ړI���W�������֐�
	-------------------------------------
	���e�F�ړI���W��������
======================================= */
void ComponentEnemyMoveRandom::InitTargetPos()
{
	// �ړ��͈͍��W(���[���h)�̍X�V
	UpdateMoveLimitRectWorldPos();

	// �����ړI���W��ݒ�
	m_vTargetPos = GetRandomPoint(
		m_vtMoveLimitRectWorld[0], m_vtMoveLimitRectWorld[1], m_vtMoveLimitRectWorld[2], m_vtMoveLimitRectWorld[3]);

	// �ړI���W�`��p�̋����W��ݒ�
	m_pTargetSphere->SetPosition(m_vTargetPos);
}

/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
======================================= */
void ComponentEnemyMoveRandom::Move()
{
	// �ړI���W�Ɍ������Ĉړ�
	Vector3<float> vDistance = m_vTargetPos - m_pCompTransform->GetPosition();

	// �ړI���W�ɓ��B���Ă���ꍇ�͖ړI���W��ύX
	if (vDistance.LengthSq() < LIMIT_DISTANCE_SQ)
	{
		SwitchTargetPos();
	}

	// �ړ���Ɍ������Ĉړ�
	Vector3<float> vVelocity = m_pCompRigidbody->GetVelocity();
	vVelocity.x = vDistance.GetNormalize().x * m_fMoveSpeed;
	vVelocity.z = vDistance.GetNormalize().z * m_fMoveSpeed;
	m_pCompRigidbody->SetVelocity(vVelocity);

	// �ړ���̍��W������(�����͍l�����Ȃ�)
	Vector3<float> vLook = m_vTargetPos;
	vLook.y = m_pCompTransform->GetPosition().y;
	m_pCompTransform->LookAt(vLook);
}

/* ========================================
	�ړI���W�؂�ւ��֐�
	-------------------------------------
	���e�F�ړI���W��؂�ւ���
======================================= */
void ComponentEnemyMoveRandom::SwitchTargetPos()
{
	m_fTargetSwitchCnt = 0.0f;

	// �ړ��͈͓��̃����_���ȍ��W��ړI���W�ɐݒ�
	m_vTargetPos = GetRandomPoint(
		m_vtMoveLimitRectWorld[0], m_vtMoveLimitRectWorld[1], m_vtMoveLimitRectWorld[2], m_vtMoveLimitRectWorld[3]);

	m_pTargetSphere->SetPosition(m_vTargetPos);

#ifdef _DEBUG
	if (CHECK_DISP_COMP("EnemyMoveRandom"))
	{
		WIN_OBJ_INFO["EnemyMoveRandom"]["TargetPos"].SetText(
			std::format("X:{:.1f} Y:{:.1f} Z:{:.1f}", m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z).c_str());
	}
#endif // _DEBUG
}

/* ========================================
	�����_�����W�擾�֐�
	-------------------------------------
	���e�F�l�p�`���̃����_���ȍ��W���擾
	-------------------------------------
	����1�F�l�p�`�̍�����W
	����2�F�l�p�`�̉E����W
	����3�F�l�p�`�̉E�����W
	����4�F�l�p�`�̍������W
	-------------------------------------
	�߂�l�F�l�p�`���̃����_���ȍ��W
======================================= */
Vector3<float> ComponentEnemyMoveRandom::GetRandomPoint(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const Vector3<float>& v4)
{
	float areaABC = GetTriangleArea(v1, v2, v3);
	float areaCDA = GetTriangleArea(v3, v4, v1);

	// �O�p�`ABC�ƎO�p�`CDA�̖ʐϔ���v�Z���A�����_���ȍ��W���擾
	// ���ʐς��傫��3�p�`�̕����I�΂�₷���悤�ɂ���
	float randomValue = MathUtils::Random(0.0f, 1.0f);
	if (randomValue < areaABC / (areaABC + areaCDA)) {
		return GetRandomPointInTriangle(v1, v2, v3);
	}
	else {
		return GetRandomPointInTriangle(v3, v4, v1);
	}
}

/* ========================================
	�����_�����W�擾(�O�p�`)�֐�
	-------------------------------------
	���e�F�O�p�`���̃����_���ȍ��W���擾
	-------------------------------------
	����1�F�O�p�`�̒��_1
	����2�F�O�p�`�̒��_2
	����3�F�O�p�`�̒��_3
	-------------------------------------
	�߂�l�F�O�p�`���̃����_���ȍ��W
======================================= */
Vector3<float> ComponentEnemyMoveRandom::GetRandomPointInTriangle(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3)
{
	float u = MathUtils::Random(0.0f, 1.0f);
	float v = MathUtils::Random(0.0f, 1.0f);

	// u + v <= 1 �ƂȂ�悤����
	if (u + v > 1.0f) {
		u = 1.0f - u;
		v = 1.0f - v;
	}

	// �o���Z���g���b�N���W���v�Z
	Vector3<float> P;
	P.x = v1.x + u * (v2.x - v1.x) + v * (v3.x - v1.x);
	P.y = v1.y + u * (v2.y - v1.y) + v * (v3.y - v1.y);
	P.z = v1.z + u * (v2.z - v1.z) + v * (v3.z - v1.z);

	return P;
}

/* ========================================
	�O�p�`�ʐώ擾�֐�
	-------------------------------------
	���e�F�O�p�`�̖ʐς��擾
	-------------------------------------
	����1�F�O�p�`�̒��_1
	����2�F�O�p�`�̒��_2
	����3�F�O�p�`�̒��_3
	-------------------------------------
	�߂�l�F�O�p�`�̖ʐ�
======================================= */
float ComponentEnemyMoveRandom::GetTriangleArea(const Vector3<float>& P1, const Vector3<float>& P2, const Vector3<float>& P3)
{
	// �s�񎮂𗘗p��������
	return 0.5f *
		std::abs(
			P1.x * (P2.z - P3.z) +
			P2.x * (P3.z - P1.z) +
			P3.x * (P1.z - P2.z)
		);
}

/* ========================================
	�ړ��͈͍��W(���[���h)�X�V�֐�
	-------------------------------------
	���e�F�ړ��͈͍��W���X�V
======================================= */
void ComponentEnemyMoveRandom::UpdateMoveLimitRectWorldPos()
{
	// �ړ��͈͍��W(���[���h)���X�V
	m_vtMoveLimitRectWorld.clear();
	for (int i = 0; i < m_vtMoveLimitRectLocal.size(); i++)
	{
		Vector3<float> vRectWorld = m_vtMoveLimitRectLocal[i] + m_vRectCenter;
		m_vtMoveLimitRectWorld.push_back(vRectWorld);
	}

	// ���W�͈͐��̍X�V
	for (int i = 0; i < LIMIT_RECT_POINT_NUM; i++)
	{
		int nNextIndex = (i + 1) % LIMIT_RECT_POINT_NUM;
		m_pRectLine->UpdateLine(i + 1, m_vtMoveLimitRectWorld[i], m_vtMoveLimitRectWorld[nNextIndex], ColorVec3::BLUE);
	}
}

/* ========================================
	�Q�b�^�[(�ړI���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�ړI���W
=========================================== */
Vector3<float> ComponentEnemyMoveRandom::GetTargetPos() const
{
	return m_vTargetPos;
}

/* ========================================
	�Q�b�^�[(�ړI���W�؂�ւ�����)�֐�
	-------------------------------------
	�ߒl�Ffloat	�ړI���W�؂�ւ�����
=========================================== */
float ComponentEnemyMoveRandom::GetTargetSwitchTime() const
{
	return m_fTargetSwitchTime;
}

/* ========================================
	�Q�b�^�[(�ړ��͈͍��W)�֐�
	-------------------------------------
	�ߒl�Fstd::vector<Vector3<float>>	�ړ��͈͍��W
=========================================== */
std::vector<Vector3<float>>& ComponentEnemyMoveRandom::GetMoveLimitRect()
{
	return m_vtMoveLimitRectLocal;
}

/* ========================================
	�Q�b�^�[(�ړ��͈͒��S���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�ړ��͈͒��S���W
=========================================== */
Vector3<float> ComponentEnemyMoveRandom::GetMoveLimitRectCenter()
{
	return m_vRectCenter;
}

/* ========================================
	�Z�b�^�[(�ړI���W�؂�ւ�����)�֐�
	-------------------------------------
	�����Ffloat	�ړI���W�؂�ւ�����
=========================================== */
void ComponentEnemyMoveRandom::SetTargetSwitchTime(float fTime)
{
	m_fTargetSwitchTime = fTime;
}

/* ========================================
	�Z�b�^�[(�ړ��͈͍��W(���[�J��))�֐�
	-------------------------------------
	�����Fstd::vector<Vector3<float>>	�ړ��͈͍��W
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRect(const std::vector<Vector3<float>>& vtRect)
{
	m_vtMoveLimitRectLocal = vtRect;
}

/* ========================================
	�ړ��͈͍��W(���[�J��)�ݒ�֐�(�����`)�֐�
	-------------------------------------
	�����Ffloat	��ӂ̒���
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectSquare(float fSize)
{
	m_vtMoveLimitRectLocal.clear();

	float fHalfSize = fSize * 0.5f;

	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfSize, 0.0f, fHalfSize));	// �E��
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfSize, 0.0f, fHalfSize));	// ����
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfSize, 0.0f, -fHalfSize));	// ����
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfSize, 0.0f, -fHalfSize));	// �E��
}

/* ========================================
	�ړ��͈͍��W(���[�J��)�ݒ�֐�(XZ����)�֐�
	-------------------------------------
	�����FVector2<float>	XZ���ʂ̃T�C�Y
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectXZ(const Vector2<float>& vSize)
{
	m_vtMoveLimitRectLocal.clear();

	float fHalfX = vSize.x * 0.5f;
	float fHalfZ = vSize.y * 0.5f;

	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfX, 0.0f, fHalfZ));		// �E��
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfX, 0.0f, fHalfZ));		// ����
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(-fHalfX, 0.0f, -fHalfZ));	// ����
	m_vtMoveLimitRectLocal.push_back(Vector3<float>(fHalfX, 0.0f, -fHalfZ));		// �E��
}


/* ========================================
	�ړ��͈͕\���t���O�ݒ�֐�
	-------------------------------------
	�����Fbool	�\���t���O
=========================================== */
void ComponentEnemyMoveRandom::SetDispMoveLimitRect(bool bDisp)
{
	m_bDispMoveLimitRect = bDisp;
}

/* ========================================
	�ړ��͈͒��S���W�ݒ�֐�
	-------------------------------------
	�����FVector3<float>	�ړ��͈͒��S���W
=========================================== */
void ComponentEnemyMoveRandom::SetMoveLimitRectCenter(const Vector3<float>& vCenter)
{
	m_vRectCenter = vCenter;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentEnemyMoveRandom::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* EnemyMoveRandom = Item::CreateGroup("EnemyMoveRandom");

	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveStop", Item::Kind::Bool, &m_bMoveStop));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	EnemyMoveRandom->AddGroupItem(Item::CreateValue("TargetPos", Item::Kind::Text));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("SwitchTime", Item::Kind::Float, &m_fTargetSwitchTime));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("SwitchCnt", Item::Kind::Float, &m_fTargetSwitchCnt));
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("DispMoveLimitRect", Item::Kind::Bool, &m_bDispMoveLimitRect));

	EnemyMoveRandom->AddGroupItem(Item::CreateBind("RectCenter", Item::Kind::Vector, &m_vRectCenter));

	// �ړ��͈͍Đݒ�
	EnemyMoveRandom->AddGroupItem(Item::CreateCallBack("ResetRect", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		m_vRectCenter = m_pCompTransform->GetPosition();
		InitTargetPos();
	}));
	// �ړ��͈̓��X�g
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_1", Item::Kind::Vector, &m_vtMoveLimitRectLocal[0]));	// �E��
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_2", Item::Kind::Vector, &m_vtMoveLimitRectLocal[1]));	// ����
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_3", Item::Kind::Vector, &m_vtMoveLimitRectLocal[2]));	// ����
	EnemyMoveRandom->AddGroupItem(Item::CreateBind("MoveLimitRect_4", Item::Kind::Vector, &m_vtMoveLimitRectLocal[3]));	// �E��

	window.AddItem(EnemyMoveRandom);

	WIN_OBJ_INFO["EnemyMoveRandom"]["TargetPos"].SetText(
		std::format("X:{:.1f} Y:{:.1f} Z:{:.1f}", m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z).c_str());

}

#endif // _DEBUG