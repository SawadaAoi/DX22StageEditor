/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�Q�[����ԊǗ�)�pcpp
	------------------------------------
	ObjectGameStateManager.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGameStateManager.h"
#include "SceneBase.h"
#include "ObjectBase.h"
#include "ObjectGoal.h"
#include "ObjectPlayer.h"
#include "ObjectCameraGameClear.h"
#include "CameraManager.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "SceneManager.h"

// =============== �萔��` =======================
const float WAIT_TIME = 3.0f;	// �ҋ@����

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectGameStateManager::ObjectGameStateManager(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_eGameState(E_GameState::GS_NORMAL)
	, m_eClearType(E_ClearType::CT_GOAL)
	, m_pPlayer(nullptr)
	, m_fWaitTime(0.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
======================================== */
void ObjectGameStateManager::InitLocal()
{
	// �v���C���[�I�u�W�F�N�g���擾
	m_pPlayer = static_cast<ObjectPlayer*>(m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player));
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
======================================== */
void ObjectGameStateManager::UpdateLocal()
{
	if (!m_pPlayer)
	{
		// �v���C���[�I�u�W�F�N�g���擾
		m_pPlayer = static_cast<ObjectPlayer*>(m_pOwnerScene->GetSceneObjectTag(E_ObjectTag::Player));
		return;
	}

	// �Q�[����Ԃ��ʏ�ȊO�̏ꍇ�̓`�F�b�N���Ȃ�
	if (m_eGameState == E_GameState::GS_NORMAL)
	{
		CheckClear();
		CheckGameOver();
	}
	// �Q�[���N���A�A�Q�[���I�[�o�[���̏���
	else if (m_eGameState == E_GameState::GS_GAMECLEAR || m_eGameState == E_GameState::GS_GAMEOVER)
	{
		m_fWaitTime += DELTA_TIME;
		if (m_fWaitTime >= WAIT_TIME)
		{
			// �X�e�[�W�Z���N�g��ʂ֑J��
			SceneManager::ChangeScene("SceneStageSelect", 3.0f);
		}
	}

#ifdef _DEBUG
	if (CHECK_DISP_OBJ("GameStateMng"))
	{
		SetDebugGameState();
		SetDebugClearType();
	}

#endif // _DEBUG


}

/* ========================================
	�Q�[���N���A�`�F�b�N�֐�
	-------------------------------------
	���e�F�Q�[���N���A�������`�F�b�N
======================================== */
void ObjectGameStateManager::CheckClear()
{
	bool bClear = false;	// �N���A�t���O

	// �N���A�����ɂ���ă`�F�b�N�֐���ύX
	switch (m_eClearType)
	{
	case E_ClearType::CT_GOAL:			bClear = CheckClearGoal();		break;
	case E_ClearType::CT_KILL_ENEMY:	bClear = CheckClearKillEnemy();	break;
	}

	// �N���A�����𖞂����Ă�����Q�[���N���A
	if (bClear)
	{
		m_eGameState = E_GameState::GS_GAMECLEAR;
		ClearGame();
	}
}

/* ========================================
	�S�[���N���A�`�F�b�N�֐�
	-------------------------------------
	���e�F�S�[���I�u�W�F�N�g�̃N���A��Ԃ��`�F�b�N
	-------------------------------------
	�߂�l�Ftrue = �N���A / false = ���N���A
======================================== */
bool ObjectGameStateManager::CheckClearGoal()
{
	std::vector<ObjectBase*> pGoalList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Goal);

	if (pGoalList.size() == 0) return false;

	// �S�[���I�u�W�F�N�g���S�ăS�[�����Ă��邩�`�F�b�N
	for (auto pGoal : pGoalList)
	{
		// �S�[���I�u�W�F�N�g�ɃL���X�g
		ObjectGoal* pGoalObj = static_cast<ObjectGoal*>(pGoal);

		// ��ł��S�[�����Ă��Ȃ��I�u�W�F�N�g�������false
		if (!pGoalObj->GetIsGoal()) return false;
	}

	// �S�ăS�[�����Ă����true
	return true;
}

/* ========================================
	�G�S�ŃN���A�`�F�b�N�֐�
	-------------------------------------
	���e�F�G�I�u�W�F�N�g���S�ł��Ă��邩�`�F�b�N
	-------------------------------------
	�߂�l�Ftrue = �N���A / false = ���N���A
======================================== */
bool ObjectGameStateManager::CheckClearKillEnemy()
{
	std::vector<ObjectBase*> pEnemyList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Enemy);

	if (pEnemyList.size() == 0) return true;
	else return false;
}

/* ========================================
	�Q�[���I�[�o�[�`�F�b�N�֐�
	-------------------------------------
	���e�F�Q�[���I�[�o�[�������`�F�b�N
======================================== */
void ObjectGameStateManager::CheckGameOver()
{
	// �v���C���[��HP��0�ȉ��ɂȂ�����Q�[���I�[�o�[
	if (m_pPlayer->GetHp() <= 0)
	{
		m_eGameState = E_GameState::GS_GAMEOVER;
	}
}

/* ========================================
	�Q�[���N���A�����֐�
	-------------------------------------
	���e�F�Q�[���N���A���̏���
======================================== */
void ObjectGameStateManager::ClearGame()
{
	// �Q�[���N���A�p�J������ǉ�
	ObjectCameraGameClear* pCamera = m_pOwnerScene->AddSceneObject<ObjectCameraGameClear>("CameraGameClear");

	// �J�������A�N�e�B�u��
	CAMERA_MNG_INST.SwitchCamera(pCamera);

	// �v���C���[�̃X�e�[�W�N���A���̏��������s
	m_pPlayer->GameClear();
}


/* ========================================
	�Q�b�^�[(�Q�[�����)�֐�
	-------------------------------------
	�ߒl�FE_GameState	�Q�[�����
=========================================== */
ObjectGameStateManager::E_GameState ObjectGameStateManager::GetGameState()
{
	return m_eGameState;
}

/* ========================================
	�Q�b�^�[(�N���A����)�֐�
	-------------------------------------
	�ߒl�FE_ClearType	�N���A����
=========================================== */
ObjectGameStateManager::E_ClearType ObjectGameStateManager::GetClearType()
{
	return m_eClearType;
}

/* ========================================
	�Z�b�^�[(�Q�[�����)�֐�
	-------------------------------------
	����1�FE_GameState	�Q�[�����
=========================================== */
void ObjectGameStateManager::SetGameState(E_GameState eState)
{
	m_eGameState = eState;
}

/* ========================================
	�Z�b�^�[(�N���A����)�֐�
	-------------------------------------
	����1�FE_ClearType	�N���A����
=========================================== */
void ObjectGameStateManager::SetClearType(E_ClearType eType)
{
	m_eClearType = eType;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectGameStateManager::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGameStateMng = Item::CreateGroup("GameStateMng");

	pGroupGameStateMng->AddGroupItem(Item::CreateValue("GameState", Item::Kind::Text));
	pGroupGameStateMng->AddGroupItem(Item::CreateValue("ClearType", Item::Kind::Text));

	window.AddItem(pGroupGameStateMng);
	
	SetDebugGameState();
	SetDebugClearType();
}

/* ========================================
	�f�o�b�O�Q�[����ԃZ�b�g�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̃Q�[����Ԃ�ݒ�
======================================== */
void ObjectGameStateManager::SetDebugGameState()
{
	using namespace DebugUI;

	Window& window = WIN_OBJ_INFO;

	// �Q�[�����
	switch (m_eGameState)
	{
	case E_GameState::GS_NORMAL:	window["GameStateMng"]["GameState"].SetText("Normal");	break;
	case E_GameState::GS_PAUSE:		window["GameStateMng"]["GameState"].SetText("Pause");		break;
	case E_GameState::GS_GAMEOVER:	window["GameStateMng"]["GameState"].SetText("GameOver");	break;
	case E_GameState::GS_GAMECLEAR:	window["GameStateMng"]["GameState"].SetText("GameClear");	break;
	}
}

/* ========================================
	�f�o�b�O�N���A�����Z�b�g�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̃N���A������ݒ�
======================================== */
void ObjectGameStateManager::SetDebugClearType()
{
	using namespace DebugUI;

	Window& window = WIN_OBJ_INFO;

	// �N���A����
	switch (m_eClearType)
	{
	case E_ClearType::CT_GOAL:			window["GameStateMng"]["ClearType"].SetText("Goal");		break;
	case E_ClearType::CT_KILL_ENEMY:	window["GameStateMng"]["ClearType"].SetText("KillEnemy");	break;
	}
}
#endif // _DEBUG