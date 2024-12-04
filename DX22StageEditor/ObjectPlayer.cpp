/* ========================================
	DX22Base/
	------------------------------------
	�v���C���[�I�u�W�F�N�g�pcpp
	------------------------------------
	ObjectPlayer.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectPlayer.h"
#include "ComponentTransform.h"
#include "ComponentGeometry.h"
#include "MathUtils.h"
#include "CameraManager.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ShapeLine.h"
#include "Input.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionOBB.h"
#include "ComponentModelStatic.h"
#include "ComponentModelAnime.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectPlayer::ObjectPlayer(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pTransform(nullptr)
	, m_pGeometry(nullptr)
	, m_pGroundRaycast(nullptr)
	, m_pRigidbody(nullptr)
{

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ObjectPlayer::InitLocal()
{
	m_pTransform = GetComponent<ComponentTransform>();


	m_pGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pGroundRaycast->SetRayLength(0.2f);

	AddComponent<ComponentCollisionOBB>();

	m_pRigidbody = AddComponent<ComponentRigidbody>();
	m_pRigidbody->SetUseGravity(true);
	m_pRigidbody->SetGroundDrag(0.9f);

	AddComponent<ComponentPlayerController>();

	AddComponent<ComponentModelAnime>();
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================= */
void ObjectPlayer::UpdateLocal()
{
	//m_pLine->UpdateLine(1,m_pTransform->GetWorldPosition(), m_pTransform->GetWorldPosition()+(m_pTransform->GetForwardVector()));

	// �n�ʂɐݒu���Ă���ꍇ�@���@�W�����v���łȂ��ꍇ
	if (m_pGroundRaycast->GetHitFlg() && m_pRigidbody->GetVelocity().y <= 0.0f)
	{
		Vector3<float> vPos = m_pTransform->GetWorldPosition();
		vPos.y = m_pGroundRaycast->GetHitPos().y + 0.5f;
		m_pTransform->SetLocalPosition(vPos);
	}

}

void ObjectPlayer::DrawLocal()
{
	//m_pLine->Draw();
}
