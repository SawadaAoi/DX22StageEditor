/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�v���C���[)�pcpp
	------------------------------------
	ObjectCameraPlayer.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectCameraPlayer.h"
#include "SceneBase.h"

#include "ObjectPlayer.h"

#include "ComponentTransform.h"
#include "ComponentCameraBase.h"

#include "Input.h"
#include "MathUtils.h"

// =============== �萔��` =======================
const float DEFAULT_ROT_SPEED	= 1.0f;							// �f�t�H���g��]���x
const float DEFAULT_DISTANCE	= 8.0f;							// �f�t�H���g����
const float DEFAULT_RAD_Y		= MathUtils::ToRadian(45.0f);	// �f�t�H���gY����]�p�x
const float DEFAULT_RAD_XZ		= MathUtils::ToRadian(180.0f);	// �f�t�H���gXZ���ʉ�]�p�x

// �p�x����
const float LIMIT_RAD_Y_MAX = MathUtils::ToRadian(89.0f);	// Y����]�p�x�ő�l
const float LIMIT_RAD_Y_MIN = MathUtils::ToRadian(20.0f);	// Y����]�p�x�ŏ��l

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1: �V�[���N���X�̃|�C���^
=========================================== */
ObjectCameraPlayer::ObjectCameraPlayer(SceneBase* pScene)
	: ObjectCamera(pScene)
	, m_pPlayer(nullptr)
	, m_fRotSpeed(0.0f)
	, m_fDistance(DEFAULT_DISTANCE)
	, m_fRadY(DEFAULT_RAD_Y)
	, m_fRadXZ(DEFAULT_RAD_XZ)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ObjectCameraPlayer::InitLocal()
{
	ObjectCamera::InitLocal();
	// �v���C���[�I�u�W�F�N�g���擾
	m_pPlayer = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectCameraPlayer::UpdateLocal()
{
	// �J�������A�N�e�B�u�łȂ��ꍇ�͏������Ȃ�
	if (!m_bActive) return;

	// �v���C���[�I�u�W�F�N�g���擾�ł��Ă��Ȃ��ꍇ�͎擾
	if (!m_pPlayer)
	{
		m_pPlayer = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);
		return;
	}


	// �v���C���[�̍��W���擾
	Vector3<float> playerPos = m_pPlayer->GetComponent<ComponentTransform>()->GetPosition();

	// �����_���v���C���[�̍��W�ɐݒ�
	m_pCompCameraBase->SetLook(playerPos);
	m_pCompTransform->LookAt(playerPos);		// �ړ����ɃJ�����̐��ʃx�N�g�����g�p����̂ŉ�]���ύX����

	// ��]���x
	m_fRotSpeed = DEFAULT_ROT_SPEED * DELTA_TIME;

	if (Input::IsKeyPress(VK_UP))		m_fRadY += m_fRotSpeed;
	if (Input::IsKeyPress(VK_DOWN))		m_fRadY -= m_fRotSpeed;
	if (Input::IsKeyPress(VK_LEFT))		m_fRadXZ -= m_fRotSpeed;
	if (Input::IsKeyPress(VK_RIGHT))	m_fRadXZ += m_fRotSpeed;

	CheckLimitRadY();	// Y����]�p�x����

	Vector3<float> cameraPos;
	cameraPos.x = m_fDistance * sinf(m_fRadXZ) * cosf(m_fRadY) + m_pCompCameraBase->GetLook().x;
	cameraPos.y = m_fDistance * sinf(m_fRadY) + m_pCompCameraBase->GetLook().y;
	cameraPos.z = m_fDistance * cosf(m_fRadY) * cosf(m_fRadXZ) + m_pCompCameraBase->GetLook().z;

	m_pCompTransform->SetPosition(cameraPos);
}

/* ========================================
	Y����]�p�x�����֐�
	-------------------------------------
	���e�FY����]�p�x�̐���
=========================================== */
void ObjectCameraPlayer::CheckLimitRadY()
{
	if (m_fRadY > LIMIT_RAD_Y_MAX) m_fRadY = LIMIT_RAD_Y_MAX;
	if (m_fRadY < LIMIT_RAD_Y_MIN) m_fRadY = LIMIT_RAD_Y_MIN;
}


/* ========================================
	�Q�b�^�[(�^�[�Q�b�g����)�֐�
	-------------------------------------
	�ߒl�Ffloat �^�[�Q�b�g����
=========================================== */
float ObjectCameraPlayer::GetDistance()
{
	return m_fDistance;
}

/* ========================================
	�Q�b�^�[(�J����Y����]�p�x)�֐�
	-------------------------------------
	�ߒl�Ffloat �J����Y����]�p�x
=========================================== */
float ObjectCameraPlayer::GetRadY()
{
	return m_fRadY;
}

/* ========================================
	�Q�b�^�[(�J����XZ���ʉ�]�p�x)�֐�
	-------------------------------------
	�ߒl�Ffloat �J����XZ���ʉ�]�p�x
=========================================== */
float ObjectCameraPlayer::GetRadXZ()
{
	return 0.0f;
}

/* ========================================
	�Z�b�^�[(�^�[�Q�b�g����)�֐�
	-------------------------------------
	����1�Ffloat �^�[�Q�b�g����
=========================================== */
void ObjectCameraPlayer::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

/* ========================================
	�Z�b�^�[(�J����Y����]�p�x)�֐�
	-------------------------------------
	����1�Ffloat �J����Y����]�p�x
=========================================== */
void ObjectCameraPlayer::SetRadY(float fRadY)
{
	m_fRadY = fRadY;
}

/* ========================================
	�Z�b�^�[(�J����XZ���ʉ�]�p�x)�֐�
	-------------------------------------
	����1�Ffloat �J����XZ���ʉ�]�p�x
=========================================== */
void ObjectCameraPlayer::SetRadXZ(float fRadXZ)
{
	m_fRadXZ = fRadXZ;
}
