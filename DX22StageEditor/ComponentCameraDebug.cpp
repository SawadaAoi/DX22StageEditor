/* ========================================
	DX22Base/
	------------------------------------
	���C���pcpp
	------------------------------------
	main.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentCameraDebug.h"
#include "ObjectBase.h"				// �����I�u�W�F�N�g
#include "ComponentCameraBase.h"	// �J�����̊�{�@�\(�ړ��A��])
#include "ObjectCamera.h"			// �����I�u�W�F�N�g���J�����ɃL���X�g
#include "WindowAPI.h"

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
	, m_fMoveSpeed(5.0f)
	, m_fRotSpeed(50.0f)
	, m_pCameraBase(nullptr)
	, m_vMouseMove(0.0f, 0.0f)
	, m_CursorPosOld{ 0, 0 }
	, m_bRotateMouse(true)
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


	Move();


	if (m_bRotateMouse)
	{
		RotateMouse();
	}
	else
	{
		RotateKey();
	}


}


/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
=========================================== */
void ComponentCameraDebug::Move()
{
	// �ړ���Shift�L�[�������Ȃ���̂ݗL��
	if (!Input::IsKeyPress(VK_SHIFT)) return;

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
	��](�L�[����)�֐�
	-------------------------------------
	���e�F�J�����̉�]����
=========================================== */
void ComponentCameraDebug::RotateKey()
{
	// �L�[��]��Shift�L�[�������Ȃ���̂ݗL��
	if (!Input::IsKeyPress(VK_SHIFT)) return;

	float pitch = 0.0f;	// �c��]
	float yaw = 0.0f;	// ����]
	if (Input::IsKeyPress(VK_UP))		pitch = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_DOWN))		pitch = m_fRotSpeed;
	if (Input::IsKeyPress(VK_LEFT))		yaw = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_RIGHT))	yaw = m_fRotSpeed;

	// Z����]�͂��Ȃ�
	m_pCameraBase->RotateLimit(pitch * DELTA_TIME, yaw * DELTA_TIME, LIMIT_PITCH);	// ��]����(�����t��
}

/* ========================================
	��](�}�E�X)�֐�
	-------------------------------------
	���e�F�J�����̉�]����
=========================================== */
void ComponentCameraDebug::RotateMouse()
{
	if (Input::IsKeyPress(VK_RBUTTON))	// �E�N���b�N������
	{
		// �}�E�X�ړ���
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		m_vMouseMove = Vector2<float>(
			static_cast<float>(cursorPos.x - m_CursorPosOld.x),
			static_cast<float>(cursorPos.y - m_CursorPosOld.y));

		m_CursorPosOld = cursorPos;

		// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
		float angleX = 360.0f * m_vMouseMove.x / WINDOW_INST.GetScreenSize().nWidth;
		float angleY = 180.0f * m_vMouseMove.y / WINDOW_INST.GetScreenSize().nHeight;

		m_pCameraBase->RotateLimit(angleY, 0, LIMIT_PITCH);
		m_pCameraBase->RotateLimit(0, angleX, LIMIT_PITCH);
	}
	else
	{
		GetCursorPos(&m_CursorPosOld);
		m_vMouseMove = { 0.0f, 0.0f };
	}
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

	pGroupCameraDebug->AddGroupItem(Item::CreateBind("RotateMouse", Item::Kind::Bool, &m_bRotateMouse));

	window.AddItem(pGroupCameraDebug);
}

#endif // _DEBUG
