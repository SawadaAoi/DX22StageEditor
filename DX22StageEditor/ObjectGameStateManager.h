/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�Q�[����ԊǗ�)�p�w�b�_
	------------------------------------
	�����F�Q�[���V�[���̏�Ԃ��Ǘ�����
		�@���Q�[�����A�|�[�Y���A�Q�[���I�[�o�[�A�Q�[���N���A
	------------------------------------
	ObjectGameStateManager.h
========================================== */
#pragma once

// ToDo : �|�[�Y���̏�����ǉ�����

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ObjectPlayer;

// =============== �N���X��` =====================
class ObjectGameStateManager :
    public ObjectBase
{
public:
	// �Q�[�����
	enum E_GameState
	{
		GS_NORMAL,		// �Q�[����
		GS_PAUSE,		// �|�[�Y��
		GS_GAMEOVER,	// �Q�[���I�[�o�[
		GS_GAMECLEAR,	// �Q�[���N���A
	};

	// �N���A����
	enum E_ClearType
	{
		CT_GOAL,		// �S�[��
		CT_KILL_ENEMY,	// �G�S��
	};
public:
	ObjectGameStateManager(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// �Q�b�^�[
	E_GameState GetGameState();
	E_ClearType GetClearType();

	// �Z�b�^�[
	void SetGameState(E_GameState eState);
	void SetClearType(E_ClearType eType);

	DEFINE_OBJECT_TYPE(ObjectGameStateManager)	// �I�u�W�F�N�g�̎��ID�擾�֐�
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window) override;
	void SetDebugGameState();
	void SetDebugClearType();
#endif // _DEBUG

private:
	void CheckClear();
	bool CheckClearGoal();
	bool CheckClearKillEnemy();
	void CheckGameOver();
private:
	E_GameState m_eGameState;	// �Q�[�����
	E_ClearType m_eClearType;	// �N���A����

	ObjectPlayer* m_pPlayer;	// �v���C���[�I�u�W�F�N�g


};

