/* ========================================
	CatRobotGame/
	------------------------------------
	�v���C���[����R���|�[�l���g�pcpp
	------------------------------------
	ComponentPlayerController.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentPlayerController.h"
#include "ObjectBase.h"		// �����I�u�W�F�N�g�̎擾
#include "ObjectBulletPlayer.h"
#include "ObjectCameraPlayer.h"	// �ړ������v�Z�p

#include "ComponentRigidbody.h"	// ���W�b�h�{�f�B�R���|�[�l���g
#include "ComponentTransform.h"	// �g�����X�t�H�[���R���|�[�l���g
#include "ComponentModelAnime.h"	// �A�j���[�V�������f���R���|�[�l���g

#include "Input.h"
#include "SceneManager.h"
#include "unordered_map"
#include "ModelAnimeManager.h"

#include <cmath>

// =============== �萔��` =======================
// ComponentRigidbody::E_ForceMode���ȗ�
using E_ForceMode = ComponentRigidbody::E_ForceMode;

const float DEFAULT_MOVE_SPEED		= 5.0f;		// �ړ����x
const float DEFAULT_ROTATE_SPEED	= 15.0f;	// ��]���x
const float DEFAULT_JUMP_POWER		= 5.0f;		// �W�����v��


// �A�j���[�V����
const float ANIME_SPEED_IDLE = 1.0f;	// �ҋ@�A�j���[�V�������x
const float ANIME_SPEED_WALK = 1.0f;	// �ړ��A�j���[�V�������x
const float ANIME_SPEED_SHOT = 3.0f;	// �V���b�g�A�j���[�V�������x

const float ANIME_BLEND_TIME = 0.2f;	// �A�j���[�V�����u�����h����
const float ANIME_SHOT_OK_RATIO = 0.5f;		// �V���b�g�\�ȃA�j���Đ���

// ���̓L�[�}�b�v
const std::unordered_map<std::string, BYTE> KEY_MAP
{
	{ "SPACE"	, VK_SPACE	 },
	{ "UP"		, VK_UP		 },
	{ "DOWN"	, VK_DOWN	 },
	{ "LEFT"	, VK_LEFT	 },
	{ "RIGHT"	, VK_RIGHT	 },
	{ "CONTROL" , VK_CONTROL },
	{ "ENTER"	, VK_RETURN  },
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
	, m_pObjCamera(nullptr)
	, m_pCompTran(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_fMoveSpeed(DEFAULT_MOVE_SPEED)
	, m_fRotateSpeed(DEFAULT_ROTATE_SPEED)
	, m_fJumpPower(DEFAULT_JUMP_POWER)
	, m_MoveKey{ 'W', 'S', 'A', 'D', VK_SPACE , VK_RETURN }
	, m_bIsInputEnable(true)
	, m_bUseJump(true)
	, m_bShot(false)

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
	m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();	// �A�j���[�V�������f�����擾

	m_pObjCamera = SceneManager::GetScene()->GetSceneObject<ObjectCameraPlayer>();	// �J�����I�u�W�F�N�g���擾

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
	// �J�����I�u�W�F�N�g���擾�ł��Ă��Ȃ��ꍇ�͎擾
	if (!m_pObjCamera)
	{
		m_pObjCamera = SceneManager::GetScene()->GetSceneObject<ObjectCameraPlayer>();
		return;
	}

	if (!m_pCompModelAnime)
	{
		m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();
		return;
	}

	// ���͖���
	if (!m_bIsInputEnable) return;	
	// �V�t�g�L�[�������Ă���Ԃ͈ړ��Ȃ�
	if (Input::IsKeyPress(VK_SHIFT)) return;

	Move();		// �ړ�
	Jump();		// �W�����v
	Shot();		// �V���b�g

}



/* ========================================
	�ړ��֐�
	-------------------------------------
	���e�F�ړ�����
========================================== */
void ComponentPlayerController::Move()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// �ړ�����

	// �L�[�{�[�h����
	moveDir = MoveKeybord();
	
	m_pCompRigidbody->AddForce(moveDir * m_fMoveSpeed);

	RotateToMoveDir(moveDir);	// �ړ������ɉ�]

	// �ړ��A�j���[�V����
	MoveAnime(moveDir);
}

/* ========================================
	�W�����v�֐�
	-------------------------------------
	���e�F�W�����v����
========================================== */
void ComponentPlayerController::Jump()
{
	if (!m_bUseJump) return;	// �W�����v�L���t���O�������Ȃ珈�����Ȃ�

	// �W�����v
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::JUMP]))
	{
		m_pCompRigidbody->AddForce(Vector3<float>::Up() * m_fJumpPower, E_ForceMode::IMPULSE);
		m_pCompTran->TranslateY(1.1f);	// ������������(�n�ʔ���̈ʒu�C�����l������)
	}
}

/* ========================================
	�e���ˊ֐�
	-------------------------------------
	���e�F�i�s�����ɒe�𔭎˂���
		�@�L�[���͂ŁA�A�j���Đ�
		  �A�j���̓K�؂ȃ^�C�~���O�Œe�𔭎�
========================================== */
void ComponentPlayerController::Shot()
{
	// �V���b�g�L�[�������ꂽ��@���@�V���b�g�A�j�����Đ�����Ă��Ȃ��ꍇ
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::SHOT]) &&
		!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT))
	{
		m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_SHOT, ANIME_BLEND_TIME, false, ANIME_SPEED_SHOT);
		m_bShot = true;		// �A�˖h�~
	}

	// �V���b�g�A�j�����Đ�����Ă���ꍇ ���� �V���b�g�A�j��������グ��Ƃ���܂ōĐ����ꂽ��
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT)
		&& m_pCompModelAnime->GetAnimePlaybackRatio(ANIME_KEY_PLAYER::PLYR_SHOT) >= ANIME_SHOT_OK_RATIO
		&& m_bShot)	// ���ɒe�𔭎˂��Ă��Ȃ���
	{

		ObjectBullet* pBullet = m_pOwnerObj->GetOwnerScene()->AddSceneObject<ObjectBulletPlayer>("PlayerBullet");
		pBullet->GetTransform()->SetPosition(m_pCompTran->GetPosition());
		pBullet->GetTransform()->SetRotation(m_pCompTran->GetRotation());
		m_bShot = false;	// �A�˖h�~
	}
}

/* ========================================
	�L�[�{�[�h���͊֐�
	-------------------------------------
	���e�F�L�[�{�[�h���͏���
	-------------------------------------
	�ߒl�FVector3<float>		�ړ�����
=========================================== */
Vector3<float> ComponentPlayerController::MoveKeybord()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// �ړ�����

	Vector3<float> cameraForward = m_pObjCamera->GetTransform()->GetForwardVector();	// �J�����̑O���x�N�g��
	Vector3<float> cameraRight = m_pObjCamera->GetTransform()->GetRightVector();	// �J�����̉E�����x�N�g��

	// �㉺�����̈ړ��𖳌���
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	// ���K��
	cameraRight.Normalize();
	cameraForward.Normalize();

	float stickAngleRadians = 0.0f;	// �X�e�B�b�N�̊p�x

	// �ړ�(���[���h����)
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::FORWARD]))	moveDir += cameraForward;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::BACK]))		moveDir -= cameraForward;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::LEFT]))		moveDir -= cameraRight;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RIGHT]))		moveDir += cameraRight;

	return moveDir;
}

/* ========================================
	�R���g���[���[���͊֐�
	-------------------------------------
	���e�F�R���g���[���[���͏���
	-------------------------------------
	�ߒl�FVector3<float>		�ړ�����
=========================================== */
Vector3<float> ComponentPlayerController::MoveController()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// �ړ�����

	Vector3<float> cameraForward = m_pObjCamera->GetTransform()->GetForwardVector();	// �J�����̑O���x�N�g��
	Vector3<float> cameraRight = m_pObjCamera->GetTransform()->GetRightVector();	// �J�����̉E�����x�N�g��

	// �㉺�����̈ړ��𖳌���
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	// ���K��
	cameraRight.Normalize();
	cameraForward.Normalize();

	float stickAngleRadians = 0.0f;	// �X�e�B�b�N�̊p�x


	// �X�e�B�b�N�̊p�x�Ɋ�Â��Ĉړ��������v�Z
	float moveX = cos(MathUtils::ToRadian(stickAngleRadians)); // �X�e�B�b�N�p�x��X����
	float moveZ = sin(MathUtils::ToRadian(stickAngleRadians)); // �X�e�B�b�N�p�x��Z����

	// �J�����̌����ɍ��킹���ړ��x�N�g�����v�Z
	moveDir = cameraForward * moveZ + cameraRight * moveX;

	return moveDir;
}

/* ========================================
	�ړ�������]�֐�
	-------------------------------------
	���e�F���g���ړ������ɉ�]����
	-------------------------------------
	����1�FVector3<float>		�ړ�����
=========================================== */
void ComponentPlayerController::RotateToMoveDir(Vector3<float> moveDir)
{
	// �ړ����Ă��Ȃ��ꍇ�͏������Ȃ�
	if (moveDir.LengthSq() <= 0.01f) return;

	moveDir.Normalize();	// ���K��

	float targetRad			= atan2(moveDir.x, moveDir.z);								// �ڕW�p�x
	Quaternion qTargetRot	= Quaternion::FromEulerAngle({ 0.0f, targetRad, 0.0f });	// �ڕW��]
	Quaternion qSelfRot		= m_pCompTran->GetLocalRotation();							// ���g��]

	// ��]�������ɂȂ�Ȃ��悤�ɒ���
	// ���N�H�[�^�j�I�����m�̓��ς����̏ꍇ�͍ŒZ�����ŉ�]���Ȃ����߁A��]�������t�ɂ���
	if (qSelfRot.Dot(qTargetRot) < 0.0f)
	{
		qTargetRot = -qTargetRot;
	}

	Quaternion qRot = Quaternion::Slerp(qSelfRot, qTargetRot, 0.2f);	// �X���[�Y�ɉ�]

	m_pCompTran->SetRotation(qRot);	// ��]��ݒ�

}

/* ========================================
	�ړ��A�j���[�V�����֐�
	-------------------------------------
	���e�F�ړ��A�j���[�V��������
	-------------------------------------
	����1�FVector3<float>		�ړ�����
=========================================== */
void ComponentPlayerController::MoveAnime(Vector3<float> vMoveDir)
{
	// �Q�[���N���A�A�Q�[���I�[�o�[�A�j���Đ����͏������Ȃ�
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_GAMECLEAR) ||
		m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_GAMEOVER)) return;

	// �V���b�g�A�j�����Đ�����Ă���ꍇ�A�ړ��A�j���͍Đ����Ȃ�
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT))		return;

	// �ړ����Ă���ꍇ
	if (std::abs(vMoveDir.x) > 0.0f || std::abs(vMoveDir.z) > 0.0f)
	{
		// ���ɕ��s�A�j�����Đ�����Ă���ꍇ�͍Đ����Ȃ�
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_WALK))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_WALK, ANIME_BLEND_TIME, true, ANIME_SPEED_WALK);
	}
	else
	{
		// ���ɑҋ@�A�j�����Đ�����Ă���ꍇ�͍Đ����Ȃ�
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_IDLE, ANIME_BLEND_TIME, true, ANIME_SPEED_IDLE);
	}
}


/* ========================================
	�Q�b�^�[(�ړ����x)�֐�
	-------------------------------------
	�ߒl�Ffloat		�ړ����x
=========================================== */
float ComponentPlayerController::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	�Q�b�^�[(��]���x)�֐�
	-------------------------------------
	�ߒl�Ffloat		��]���x
=========================================== */
float ComponentPlayerController::GetRotateSpeed()
{
	return m_fRotateSpeed;
}

/* ========================================
	�Q�b�^�[(�W�����v�L���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool		�W�����v�L���t���O
=========================================== */
bool ComponentPlayerController::GetUseJump()
{
	return m_bUseJump;
}

/* ========================================
	�Z�b�^�[(�ړ����x)�֐�
	-------------------------------------
	�ߒl�Ffloat		�ړ����x
=========================================== */
void ComponentPlayerController::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

/* ========================================
	�Z�b�^�[(��]���x)�֐�
	-------------------------------------
	�ߒl�Ffloat		��]���x
=========================================== */
void ComponentPlayerController::SetRotateSpeed(float fRotateSpeed)
{
	m_fRotateSpeed = fRotateSpeed;
}

/* ========================================
	�Z�b�^�[(�W�����v�L���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool		�W�����v�L���t���O
=========================================== */
void ComponentPlayerController::SetUseJump(bool bUseJump)
{
	m_bUseJump = bUseJump;
}

/* ========================================
	�Z�b�^�[(���͗L���t���O)�֐�
	-------------------------------------
	����1�Fbool		���͗L���t���O
=========================================== */
void ComponentPlayerController::SetInputEnable(bool bEnable)
{
	m_bIsInputEnable = bEnable;
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

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("IsInputEnable", Item::Kind::Bool, &m_bIsInputEnable));	// ���͗L���t���O

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("MoveSpeed",		Item::Kind::Float, &m_fMoveSpeed));		// �ړ����x
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("RotateSpeed",	Item::Kind::Float, &m_fRotateSpeed));	// ��]���x
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("JumpPower",		Item::Kind::Float, &m_fJumpPower));		// �W�����v��

	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Forward",		&m_MoveKey[E_MoveKey::FORWARD]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Back",			&m_MoveKey[E_MoveKey::BACK]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Right",		&m_MoveKey[E_MoveKey::RIGHT]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Left",			&m_MoveKey[E_MoveKey::LEFT]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Jump",			&m_MoveKey[E_MoveKey::JUMP]));

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