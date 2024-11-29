/* ========================================
	DX22Base/
	------------------------------------
	�v���C���[����R���|�[�l���g�pcpp
	------------------------------------
	ComponentPlayerController.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentPlayerController.h"
#include "ObjectBase.h"	// �����I�u�W�F�N�g�̎擾
#include "ComponentTransform.h"	// �g�����X�t�H�[���R���|�[�l���g
#include "ComponentRigidbody.h"	// ���W�b�h�{�f�B�R���|�[�l���g
#include "Input.h"
#include "unordered_map"

// =============== �萔��` =======================
// ComponentRigidbody::E_ForceMode���ȗ�
using E_ForceMode = ComponentRigidbody::E_ForceMode;


const float DEFAULT_MOVE_SPEED		= 5.0f;	// �ړ����x
const float DEFAULT_ROTATE_SPEED	= 15.0f;	// ��]���x
const float DEFAULT_JUMP_POWER		= 5.0f;	// �W�����v��


const std::unordered_map<std::string, BYTE> KEY_MAP
{
	{ "SPACE"	, VK_SPACE	 },
	{ "UP"		, VK_UP		 },
	{ "DOWN"	, VK_DOWN	 },
	{ "LEFT"	, VK_LEFT	 },
	{ "RIGHT"	, VK_RIGHT	 },
	{ "CONTROL" , VK_CONTROL },
	{ "A", 'A' },
	{ "B", 'B' },
	{ "C", 'C' },
	{ "D", 'D' },
	{ "E", 'E' },
	{ "F", 'F' },
	{ "G", 'G' },
	{ "H", 'H' },
	{ "I", 'I' },
	{ "J", 'J' },
	{ "K", 'K' },
	{ "L", 'L' },
	{ "M", 'M' },
	{ "N", 'N' },
	{ "O", 'O' },
	{ "P", 'P' },
	{ "Q", 'Q' },
	{ "R", 'R' },
	{ "S", 'S' },
	{ "T", 'T' },
	{ "U", 'U' },
	{ "V", 'V' },
	{ "W", 'W' },
	{ "X", 'X' },
	{ "Y", 'Y' },
	{ "Z", 'Z' },

};

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
=========================================== */
ComponentPlayerController::ComponentPlayerController(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderPlayerControl)
	, m_pCompTran(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(DEFAULT_MOVE_SPEED)
	, m_fRotateSpeed(DEFAULT_ROTATE_SPEED)
	, m_fJumpPower(DEFAULT_JUMP_POWER)
	, m_MoveKey{ 'W', 'S', 'A', 'D', VK_SPACE, VK_LEFT, VK_RIGHT }
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ComponentPlayerController::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();	// �g�����X�t�H�[�����擾
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();	// ���W�b�h�{�f�B���擾
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
========================================== */
void ComponentPlayerController::Uninit()
{
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ComponentPlayerController::Update()
{
	// �V�t�g�L�[�������Ă���Ԃ͈ړ��Ȃ�
	if (Input::IsKeyPress(VK_SHIFT)) return;

	Vector3<float> moveDir = Vector3<float>::Zero();	// �ړ�����

	// �ړ�(���[���h����)
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Forward]))
	{
		moveDir += Vector3<float>::Forward();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Back]))
	{
		moveDir += Vector3<float>::Back();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Left]))
		{
		moveDir += Vector3<float>::Left();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Right]))
	{
		moveDir += Vector3<float>::Right();
	}

	m_pCompRigidbody->AddForce(moveDir * m_fMoveSpeed);

	// �W�����v
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::Jump]))
	{
		m_pCompRigidbody->AddForce(Vector3<float>::Up() * m_fJumpPower, E_ForceMode::IMPULSE);
	}

	// Y����]
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RotateLeft]))
	{
		m_pCompTran->RotateY(-m_fRotateSpeed * DELTA_TIME);
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RotateRight]))
	{
		m_pCompTran->RotateY(m_fRotateSpeed * DELTA_TIME);
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
void ComponentPlayerController::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupPlayerCtr = Item::CreateGroup("PlayerController");

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("MoveSpeed",		Item::Kind::Float, &m_fMoveSpeed));		// �ړ����x
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("RotateSpeed",	Item::Kind::Float, &m_fRotateSpeed));	// ��]���x
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("JumpPower",		Item::Kind::Float, &m_fJumpPower));		// �W�����v��

	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Forward",		&m_MoveKey[E_MoveKey::Forward]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Back",			&m_MoveKey[E_MoveKey::Back]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Right",		&m_MoveKey[E_MoveKey::Right]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Left",			&m_MoveKey[E_MoveKey::Left]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Jump",			&m_MoveKey[E_MoveKey::Jump]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_RotateLeft",	&m_MoveKey[E_MoveKey::RotateLeft]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_RotateRight",	&m_MoveKey[E_MoveKey::RotateRight]));

	window.AddItem(pGroupPlayerCtr);

}

/* ========================================
	�L�[�I�����X�g�쐬�֐�
	-------------------------------------
	���e�F�L�[�I�����X�g���쐬
	-------------------------------------
	����1�F���ږ�
	����2�F�L�[�����蓖�Ă�ϐ�
	-------------------------------------
	�߂�l�FItem*		�A�C�e��
=========================================== */
DebugUI::Item* ComponentPlayerController::CreateKeyList(std::string sName, BYTE* moveKey)
{
	using namespace DebugUI;

	// ���X�g�쐬
	DebugUI::Item* reItem =  Item::CreateList(sName.c_str(), [this, moveKey](const void* arg)
	{
		std::string key = reinterpret_cast<const char*>(arg);
		*moveKey = KEY_MAP.at(key);	// ���̓L�[��ݒ�(BYTE)

	}, false, true);

	// ���X�g�����\����ݒ�
	int i = 0; // ���X�g�ʒu
	for (auto& key : KEY_MAP)
	{
		reItem->AddListItem(key.first.c_str());
		
		// �����l�ƈ�v���镶�����T��
		if (key.second == *moveKey)
		{
			reItem->SetListNo(i);
		}
		
		i++;
	}

	

	return reItem;
}
#endif // _DEBUG