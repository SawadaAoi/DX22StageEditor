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
	std::vector<ObjectBase*> pPlayerList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Player);
	m_pPlayer = static_cast<ObjectPlayer*>(pPlayerList[0]);
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
		std::vector<ObjectBase*> pPlayerList = m_pOwnerScene->GetSceneObjectsTag(E_ObjectTag::Player);
		m_pPlayer = static_cast<ObjectPlayer*>(pPlayerList[0]);
		return;
	}

	// �Q�[����Ԃ��ʏ�ȊO�̏ꍇ�̓`�F�b�N���Ȃ�
	if (m_eGameState == E_GameState::GS_NORMAL)
	{
		CheckClear();
		CheckGameOver();
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
		m_eGameState = E_GameState::GS_GAMECLEAR;
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