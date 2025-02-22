/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�t�@�C���[�o�[)�pcpp
	------------------------------------
	ObjectFireBar.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectFireBar.h"
#include "SceneBase.h"
#include "ObjectBlockGroundAABB.h"
#include "ComponentBillboardAnime.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "TextureManager.h"


// =============== �萔��` =======================
const int DEFAULT_FIREBALL_NUM = 5;	// �΂̋ʂ̐�
const float DEFAULT_FIREBALL_COL_SCALE = 0.35f;	// �΂̋ʂ̃X�P�[��
const float DEFAULT_FIREBALL_SPEED = 30.0f;	// �΂̋ʂ̉�]���x
const float DEFAULT_FIREBALL_SCALE = 1.5f;	// �΂̋ʂ̃X�P�[��

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
=========================================== */
ObjectFireBar::ObjectFireBar(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCenterBlock(nullptr)
	, m_pFireBallParent(nullptr)
	, m_nFireBallNum(DEFAULT_FIREBALL_NUM)
	, m_fRotateSpeed(DEFAULT_FIREBALL_SPEED)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
=========================================== */
void ObjectFireBar::InitLocal()
{
	InitCenterBlock();
	InitFireBall();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectFireBar::UpdateLocal()
{
	m_pFireBallParent->GetTransform()->RotateY(m_fRotateSpeed * DELTA_TIME);
}

/* ========================================
	���S�u���b�N�������֐�
	-------------------------------------
	���e�F���S�u���b�N�̏��������s��
=========================================== */
void ObjectFireBar::InitCenterBlock()
{
	m_pCenterBlock = m_pOwnerScene->AddSceneObject<ObjectBlockGroundAABB>("CenterBlock");
	m_pCenterBlock->SetIsSave(false);	// �q�I�u�W�F�N�g�̓Z�[�u���Ȃ�
	m_pCenterBlock->GetComponent<ComponentGeometry>()->SetTextureAll(GET_TEXTURE_DATA(TEX_KEY::BLOCK_SIMPLE));

	AddChildObject(m_pCenterBlock);
}

/* ========================================
	�΂̋ʏ������֐�
	-------------------------------------
	���e�F�΂̋ʂ̏��������s��
=========================================== */
void ObjectFireBar::InitFireBall()
{
	m_pFireBallParent = m_pOwnerScene->AddSceneObject<ObjectBase>("FireBallParent");
	m_pFireBallParent->SetIsSave(false);	// �q�I�u�W�F�N�g�̓Z�[�u���Ȃ�

	// �΂̋ʐ���
	for (int i = 1; i <= m_nFireBallNum; i++)
	{
		ObjectBase* pFireBall = m_pOwnerScene->AddSceneObject<ObjectBase>("FireBall");
		pFireBall->SetTag(E_ObjectTag::Enemy);
		pFireBall->SetIsSave(false);	// �q�I�u�W�F�N�g�̓Z�[�u���Ȃ�
		pFireBall->AddComponent<ComponentBillboardAnime>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_FIREBALL));
		pFireBall->GetComponent<ComponentBillboardAnime>()->Play(4, { 4, 1 }, 0.5f, true);
		pFireBall->AddComponent<ComponentCollisionSphere>()->SetRadius(DEFAULT_FIREBALL_COL_SCALE);
		pFireBall->GetComponent<ComponentCollisionSphere>()->SetTrigger(true);
		pFireBall->GetComponent<ComponentCollisionSphere>()->SetRefOwnerTransform(false);
		pFireBall->GetTransform()->SetPosition({ 0.0f + (1.0f * i), 0.0f, 0.0f });
		pFireBall->GetTransform()->Scale(DEFAULT_FIREBALL_SCALE);
		m_pFireBall.push_back(pFireBall);
		m_pFireBallParent->AddChildObject(pFireBall);
	}

	AddChildObject(m_pFireBallParent);
}

/* ========================================
	�R�s�[�֐�(�ʏ���)
	-------------------------------------
	���e�F�I�u�W�F�N�g�ʂ̃R�s�[����
	-------------------------------------
	����1�F�R�s�[���ꂽ�I�u�W�F�N�g
=========================================== */
void ObjectFireBar::CopyLocal(ObjectBase* pObject)
{
	ObjectFireBar* pCopyObj = static_cast<ObjectFireBar*>(pObject);

	// �΂̋ʐ�
	pCopyObj->m_nFireBallNum = m_nFireBallNum;
	// ��]���x
	pCopyObj->m_fRotateSpeed = m_fRotateSpeed;
}


/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectFireBar::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// �΂̋ʐ�
	data.nFireBallNum = m_nFireBallNum;
	// ��]���x
	data.fRotateSpeed = m_fRotateSpeed;

	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectFireBar::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	file.read((char*)&data, sizeof(S_SaveData));

	// �΂̋ʐ�
	m_nFireBallNum = data.nFireBallNum;
	// ��]���x
	m_fRotateSpeed = data.fRotateSpeed;
}

/* ========================================
	�Z�b�^�[(�΂̋ʐ�)�֐�
	-------------------------------------
	�����Fint �΂̋ʐ�
=========================================== */
void ObjectFireBar::SetFireBallNum(int nFireBallNum)
{
	m_nFireBallNum = nFireBallNum;
}

/* ========================================
	�Z�b�^�[(��]���x)�֐�
	-------------------------------------
	�����Ffloat ��]���x
=========================================== */
void ObjectFireBar::SetRotateSpeed(float fRotateSpeed)
{
	m_fRotateSpeed = fRotateSpeed;
}

