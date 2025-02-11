/* ========================================
	CatRobotGame/
	------------------------------------
	�v���C���[�J�n�n�_�pcpp
	------------------------------------
	ObjectPlayerStart.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectPlayerStart.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "ComponentCollisionAABB.h"
#include "TextureManager.h"
#include "ObjectPlayer.h"
#include "SceneBase.h"

// =============== �萔��` =======================
const std::string PLAYER_OBJECT_NAME = "Player";


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
========================================== */
ObjectPlayerStart::ObjectPlayerStart(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_bIsPlayerStart(false)
{
	// �^�O�ݒ�
	SetTag(E_ObjectTag::PlayerStart);
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================= */
void ObjectPlayerStart::InitLocal()
{
	// �ʒu�ݒ�
	GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

#ifdef _DEBUG
	// �r���{�[�h�ݒ�
	ComponentBillboard* pBillboard = AddComponent<ComponentBillboard>();
	pBillboard->SetTexture(GET_TEXTURE_DATA(TEX_KEY::DEBUG_PLAYER_START));

	// �����蔻��(�ʒu�����F���₷�����邽��)
	ComponentCollisionAABB* pCollision = AddComponent<ComponentCollisionAABB>();
	pCollision->SetTrigger(true);
#endif // _DEBUG
	
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================= */
void ObjectPlayerStart::UpdateLocal()
{
	// �J�n�t���O�������Ă���ꍇ�̓v���C���[�𐶐�
	if (m_bIsPlayerStart)
	{
		ObjectBase* pPlayer = m_pOwnerScene->AddSceneObject<ObjectPlayer>(PLAYER_OBJECT_NAME);
		pPlayer->GetTransform()->SetPosition(GetTransform()->GetLocalPosition());			// ���g�̈ʒu�Ƀv���C���[�𐶐�

		SetState(E_State::STATE_DEAD);	// ���g���폜
	}
}

/* ========================================
	�Q�b�^�[(�v���C���[�J�n�t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�v���C���[�J�n�t���O
=========================================== */
bool ObjectPlayerStart::GetIsPlayerStart() const
{
	return m_bIsPlayerStart;
}

/* ========================================
	�Z�b�^�[(�v���C���[�J�n�t���O)�֐�
	-------------------------------------
	����1�Fbool	�v���C���[�J�n�t���O
=========================================== */
void ObjectPlayerStart::SetIsPlayerStart(bool bIsPlayerStart)
{
	m_bIsPlayerStart = bIsPlayerStart;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectPlayerStart::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupPlayerStart = Item::CreateGroup("PlayerStart");

	pGroupPlayerStart->AddGroupItem(Item::CreateBind("IsPlayerStart", Item::Kind::Bool, &m_bIsPlayerStart));

	window.AddItem(pGroupPlayerStart);
}

#endif // _DEBUG