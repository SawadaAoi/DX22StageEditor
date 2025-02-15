/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�R�C��)�pcpp
	------------------------------------
	ObjectCoin.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectCoin.h"

#include "ComponentBillboardAnime.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionSphere.h"
#include "ComponentGroundRaycast.h"
#include "ComponentTransform.h"

#include "TextureManager.h"

// =============== �萔 =======================
const int			ANIME_FRAME_MAX = 18;	// �A�j���[�V�����t���[����
const Vector2<int>	ANIME_ROW_COL	= Vector2<int>(6, 3);	// �A�j���[�V�����s��
const float			ANIME_SPEED		= 0.5f;	// �A�j���[�V�������x

const float			COIN_SCALE		= 0.6f;	// �R�C���̊g�嗦

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectCoin::ObjectCoin(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompBillboardAnime(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompCollisionSphere(nullptr)
	, m_pCompGroundRaycast(nullptr)
{
	SetTag(E_ObjectTag::Coin);
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏�����
=========================================== */
void ObjectCoin::InitLocal()
{
	m_pCompBillboardAnime	= AddComponent<ComponentBillboardAnime>();
	m_pCompRigidbody		= AddComponent<ComponentRigidbody>();
	m_pCompCollisionSphere	= AddComponent<ComponentCollisionSphere>();
	m_pCompGroundRaycast	= AddComponent<ComponentGroundRaycast>();

	// �e�N�X�`���ݒ�
	m_pCompBillboardAnime->SetTexture(GET_TEXTURE_DATA(TEX_KEY::ITEM_COIN));
	m_pCompBillboardAnime->Play(ANIME_FRAME_MAX, ANIME_ROW_COL, ANIME_SPEED, true);

	// �Փ˔���ݒ�
	m_pCompCollisionSphere->SetTrigger(true);
	m_pCompCollisionSphere->SetDynamic(false);

	// �傫���ݒ�
	m_pCompTransform->Scale(Vector3<float>(COIN_SCALE, COIN_SCALE, COIN_SCALE));
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̍X�V����
=========================================== */
void ObjectCoin::UpdateLocal()
{
	// �n�ʔ���
	// �n�ʂɐݒu���Ă���ꍇ
	if (m_pCompGroundRaycast->GetHitFlg())
	{
		// �n�ʂɐݒu���Ă���ʒu�ɍ��������킹��
		Vector3<float> vPos = m_pCompTransform->GetPosition();
		float fAdjustHeight = m_pCompGroundRaycast->GetHitPos().y + (m_pCompTransform->GetScale().y * 0.5f) - vPos.y;
		m_pCompTransform->TranslateY(fAdjustHeight);
	}
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectCoin::OnCollisionEnter(ObjectBase* pHit)
{
	// �v���C���[�ƐڐG������
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		Destroy();	// �R�C���폜
	}
}
