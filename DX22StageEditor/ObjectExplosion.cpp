/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�����G�t�F�N�g)�pcpp
	------------------------------------
	ObjectExplosion.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectExplosion.h"
#include "ComponentBillboardAnime.h"
#include "ComponentTransform.h"
#include "TextureManager.h"

// =============== �萔��` =======================
const Vector3<float>	DEFAULT_SCALE	= Vector3<float>(2.0f, 2.0f, 2.0f);	// �f�t�H���g�X�P�[��
const int				ANIME_FRAME_MAX = 12;								// �A�j���[�V�����t���[����
const Vector2<int>		ANIME_COL_ROW	= Vector2<int>(4, 3);				// �A�j���[�V�����̗�ƍs
const float				ANIME_SPEED		= 0.5f;								// �A�j���[�V�������x

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectExplosion::ObjectExplosion(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompBillboardAnime(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
========================================== */
void ObjectExplosion::InitLocal()
{
	m_pCompBillboardAnime = AddComponent<ComponentBillboardAnime>();
	m_pCompBillboardAnime->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_EXPLOSION));

	// �A�j���[�V�����Đ�
	m_pCompBillboardAnime->Play(ANIME_FRAME_MAX, ANIME_COL_ROW, ANIME_SPEED, false);
	// �X�P�[���ݒ�
	m_pCompTransform->SetScale(DEFAULT_SCALE);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ObjectExplosion::UpdateLocal()
{
	// �A�j���[�V�����I����
	if (m_pCompBillboardAnime->GetAnimeEnd())
	{
		SetState(E_State::STATE_DEAD);
	}
}

/* ========================================
	�Q�b�^�[(�r���{�[�h(�A�j���[�V�����L��)�R���|�[�l���g)�֐�
	-------------------------------------
	�ߒl�FComponentBillboardAnime*�@�r���{�[�h(�A�j���[�V�����L��)�R���|�[�l���g
=========================================== */
ComponentBillboardAnime* ObjectExplosion::GetCompBillboardAnime()
{
	return nullptr;
}
