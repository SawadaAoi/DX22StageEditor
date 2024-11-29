/* ========================================
	DX22Base/
	------------------------------------
	���C���pcpp
	------------------------------------
	main.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentCameraDebug.h"
#include "Input.h"
#include "ObjectBase.h"				// �����I�u�W�F�N�g
#include "ComponentCameraBase.h"	// �J�����̊�{�@�\(�ړ��A��])
#include "ObjectCamera.h"			// �����I�u�W�F�N�g���J�����ɃL���X�g

// =============== �萔��` =======================
const float LIMIT_PITCH = 90.0f;	// �c��]����

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
=========================================== */
ComponentCameraDebug::ComponentCameraDebug(ObjectBase* pObject)
	: ComponentBase(pObject, OrderCameraDebug)
	, m_fMoveSpeed(2.0f)
	, m_fRotSpeed(50.0f)
	, m_pCameraBase(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ComponentCameraDebug::Init()
{
	m_pCameraBase = m_pOwnerObj->GetComponent<ComponentCameraBase>();	// �J�����̊�{�@�\��ϐ���
}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentCameraDebug::Update()
{
	// �J�������A�N�e�B�u�łȂ��ꍇ�͏������Ȃ�
	if (static_cast<ObjectCamera*>(m_pOwnerObj)->GetActive() == false) return;	

	if (!Input::IsKeyPress(VK_SHIFT)) return;

	Move();
	Rotate();	
}


/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
=========================================== */
void ComponentCameraDebug::Move()
{
	Vector3<float> vFowrard = m_pCameraBase->GetForward();	// �J�����̑O����
	Vector3<float> vRight = m_pCameraBase->GetRight();		// �J�����̉E����
	Vector3<float> vUp = m_pCameraBase->GetUp();			// �J�����̏����

	Vector3<float> fMove;	// �ړ���

	if (Input::IsKeyPress('W')) fMove = vFowrard * m_fMoveSpeed;
	if (Input::IsKeyPress('S')) fMove = vFowrard * -m_fMoveSpeed;
	if (Input::IsKeyPress('A')) fMove = vRight * -m_fMoveSpeed;
	if (Input::IsKeyPress('D')) fMove = vRight * m_fMoveSpeed;
	if (Input::IsKeyPress('Q')) fMove = vUp * -m_fMoveSpeed;
	if (Input::IsKeyPress('E')) fMove = vUp * m_fMoveSpeed;

	m_pCameraBase->Move(fMove * DELTA_TIME);	// �ړ�����
}

/* ========================================
	��]�֐�
	-------------------------------------
	���e�F��]����
=========================================== */
void ComponentCameraDebug::Rotate()
{
	float pitch = 0.0f;	// �c��]
	float yaw = 0.0f;	// ����]
	if (Input::IsKeyPress(VK_UP))		pitch = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_DOWN))		pitch = m_fRotSpeed;
	if (Input::IsKeyPress(VK_LEFT))		yaw = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_RIGHT))	yaw = m_fRotSpeed;

	// Z����]�͂��Ȃ�
	m_pCameraBase->RotateLimit(pitch * DELTA_TIME, yaw * DELTA_TIME, LIMIT_PITCH);	// ��]����(�����t��
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentCameraDebug::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCameraDebug = Item::CreateGroup("CameraDebug");

	pGroupCameraDebug->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	pGroupCameraDebug->AddGroupItem(Item::CreateBind("RotSpeed", Item::Kind::Float, &m_fRotSpeed));

	window.AddItem(pGroupCameraDebug);
}

#endif // _DEBUG
