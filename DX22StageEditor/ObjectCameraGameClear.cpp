/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�N���A�����o�pcpp
	------------------------------------
	ObjectCameraGameClear.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectCameraGameClear.h"
#include "ComponentTransform.h"
#include "ComponentCameraBase.h"
#include "ObjectCameraPlayer.h"
#include "SceneBase.h"


// =============== �萔��` =======================
const float CAMERA_STAND_BY_TIME = 2.0f;

// �v���C���[�ɋ߂Â��ړ�
const Vector3<float> CAMERA_NEAR_PLAYER_POS = Vector3<float>(0.0f, 2.0f, -3.0f);
const float NEAR_PLAYER_MOVE_TIME = 0.5f;

// �Y�[���A�E�g�ړ�
const Vector3<float> CAMERA_ZOOM_OUT_POS = Vector3<float>(20.0f, 20.0f, -20.0f);	// �X�e�[�W����ՂŌ�����W
const float ZOOM_OUT_MOVE_TIME = 8.0f;

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1: �V�[���N���X�̃|�C���^
========================================== */
ObjectCameraGameClear::ObjectCameraGameClear(SceneBase* pScene)
	: ObjectCamera(pScene)
	, m_fStandByTimeCnt(0.0f)
	, m_bZoomOutStart(false)
	, m_pTarget(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectCameraGameClear::InitLocal()
{
	ObjectCamera::InitLocal();

	// �v���C���[�J�������擾
	ObjectCameraPlayer* pPlayerCamera = m_pOwnerScene->GetSceneObject<ObjectCameraPlayer>();

	// �v���C���[�J�����̏����R�s�[(�J�����̐؂�ւ������R�Ɍ����邽��)
	m_pCompTransform->SetPosition(pPlayerCamera->GetTransform()->GetLocalPosition());	// ���W
	m_pCompTransform->SetRotation(pPlayerCamera->GetTransform()->GetLocalRotation());	// ��]
	m_pCompCameraBase->SetLook(pPlayerCamera->GetCameraBase()->GetLook());				// �����_

	// �^�[�Q�b�g(�v���C���[)���擾
	m_pTarget = m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player);

	// �v���C���[�ڋ߈ړ��J�n
	EventMoveNearPlayer();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ObjectCameraGameClear::UpdateLocal()
{
	// ��Ƀv���C���[������
	m_pCompCameraBase->SetLook(m_pTarget->GetTransform()->GetPosition());
	m_pCompTransform->LookAt(m_pTarget->GetTransform()->GetPosition());

	// �ҋ@���ԃJ�E���g
	m_fStandByTimeCnt += DELTA_TIME;

	// �ҋ@���Ԃ��o�߂�����Y�[���A�E�g�J�n
	if (m_fStandByTimeCnt >= CAMERA_STAND_BY_TIME && !m_bZoomOutStart)
	{
		m_fStandByTimeCnt = 0.0f;
		m_bZoomOutStart = true;
		EventZoomOutStart();
	}
}

/* ========================================
	�v���C���[�ڋ߈ړ��֐�
	-------------------------------------
	���e�F�v���C���[��Z�����������猩����W�Ɉړ�
=========================================== */
void ObjectCameraGameClear::EventMoveNearPlayer()
{
	Vector3<float> playerPos = m_pTarget->GetTransform()->GetPosition();	// �v���C���[�̍��W
	Vector3<float> vTargetPos = (playerPos + CAMERA_NEAR_PLAYER_POS);			// �v���C���[�̍��W����Z���������Ɉړ�

	// �ړI���W�֎w�莞�Ԃňړ�
	m_pCompTransform->MoveTo(vTargetPos, NEAR_PLAYER_MOVE_TIME);
}

/* ========================================
	�Y�[���A�E�g�J�n�֐�
	-------------------------------------
	���e�F�X�e�[�W����ՂŌ�����W�Ɉړ����J�n
=========================================== */
void ObjectCameraGameClear::EventZoomOutStart()
{
	// �ړI���W�֎w�莞�Ԃňړ�
	m_pCompTransform->MoveTo(CAMERA_ZOOM_OUT_POS, 8.0f);
}
