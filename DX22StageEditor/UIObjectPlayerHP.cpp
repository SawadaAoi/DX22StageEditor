/* ========================================
	CatRobotGame/
	------------------------------------
	UI�I�u�W�F�N�g(�v���C���[HP)�pcpp
	------------------------------------
	UIObjectPlayerHP.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectPlayer.h"
#include "SceneBase.h"
#include "TextureManager.h"
// �R���|�[�l���g
#include "UIComponentSprite.h"
#include "UIComponentTransform.h"
#include "UIObjectPlayerHP.h"

// =============== �萔��` =======================
const Vector2<float> DEFAULT_SCALE = Vector2<float>(100.0f, 100.0f);

const std::string HEART_OBJECT_NAME			= "Heart_";
const Vector2<float> HEART_TEXTURE_RATIO	= Vector2<float>(140.0f, 100.0f);

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
========================================== */
UIObjectPlayerHP::UIObjectPlayerHP(SceneBase* pScene)
	: UIObjectBase(pScene)
	, m_pPlayer(nullptr)
{
}


/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void UIObjectPlayerHP::InitLocal()
{
	m_pPlayer = static_cast<ObjectPlayer*>(m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player));

	m_pCompTransform->SetLocalScale(DEFAULT_SCALE);
	m_pCompSprite->SetIsVisible(false);

	// �v���C���[�����݂���ꍇ��HP�𐶐�
	if (m_pPlayer) CreateHP();
}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void UIObjectPlayerHP::UpdateLocal()
{
	// �v���C���[�����݂��Ȃ��ꍇ�͎擾����
	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<ObjectPlayer*>(m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player));
		if (m_pPlayer) CreateHP();		// �v���C���[�����݂���ꍇ��HP�𐶐�

		return;
	}

	int nHP		= m_pPlayer->GetHp();		// TODO: �v���C���[��HP���擾����
	int nHPMax	= m_pPlayer->GetMaxHp();	// TODO: �v���C���[�̍ő�HP���擾����

	// HP�̕\�������X�V
	for (int i = 0; i < nHPMax; i++)
	{
		m_pUIObjectHP[i]->GetComponent<UIComponentSprite>()->SetIsVisible(i < nHP);	
	}
}

/* ========================================
	HP�����֐�
	-------------------------------------
	���e�FHP�𐶐�����
========================================== */
void UIObjectPlayerHP::CreateHP()
{
	int nHP = m_pPlayer->GetHp();		// TODO: �v���C���[��HP���擾����

	// HP�����̃n�[�g�𐶐�
	for (int i = 0; i < nHP; i++)
	{
		// �n�[�g�I�u�W�F�N�g�𐶐�(���O��"Heart_" + i)
		UIObjectBase* pUIObject = m_pOwnerScene->AddSceneUI<UIObjectBase>(HEART_OBJECT_NAME + std::to_string(i));

		// �g�����X�t�H�[��
		UIComponentTransform* pCompTrans = pUIObject->GetComponent<UIComponentTransform>();
		Vector2<float> vPos = GetTransform()->GetWorldPosition() + Vector2<float>(i * DEFAULT_SCALE.x, 0.0f);
		pCompTrans->SetLocalPosition(vPos);
		pCompTrans->SetLocalScale(HEART_TEXTURE_RATIO);
		// �X�v���C�g
		UIComponentSprite* pCompSprite = pUIObject->GetComponent<UIComponentSprite>();
		pCompSprite->SetTexture(GET_TEXTURE_DATA(TEX_KEY::PLAYER_HP));

		AddChildUI(pUIObject);
		m_pUIObjectHP.push_back(pUIObject);
	}
}

