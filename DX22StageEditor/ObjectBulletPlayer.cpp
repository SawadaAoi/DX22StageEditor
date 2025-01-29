/* ========================================
	CatRobotGame/
	------------------------------------
	�e�I�u�W�F�N�g(���)�pcpp
	------------------------------------
	ObjectBullet.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectBulletPlayer.h"
#include "ComponentTransform.h"
#include "ComponentBillboard.h"
#include "TextureManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
=========================================== */
ObjectBulletPlayer::ObjectBulletPlayer(SceneBase* pScene)
	: ObjectBullet(pScene)
{
	SetTag(E_ObjectTag::PlayerBullet);
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void ObjectBulletPlayer::InitLocal()
{
	ObjectBullet::InitLocal();
	AddComponent<ComponentBillboard>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::PLAYER_BULLET));

	m_pCompTransform->SetScale({ 0.5f, 0.5f, 0.5f });
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	����1�F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectBulletPlayer::OnCollisionEnter(ObjectBase* pHit)
{
	if (pHit->GetTag() == E_ObjectTag::Player
		|| pHit->GetTag() == E_ObjectTag::PlayerBullet)
	{
		return;
	}

	if (pHit->GetTag() == E_ObjectTag::Enemy)
	{
		pHit->OnCollisionStay(this);
	}
	
	SetState(E_State::STATE_DEAD);
}

