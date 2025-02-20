/* ========================================
	DX22Base/
	------------------------------------
	�V�[���Ǘ��pcpp
	------------------------------------
	SceneManager.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "SceneManager.h"
#include "SceneBase.h"

#include "RegisterAllScene.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
void SceneManager::Init()
{
#ifndef _DEBUG
	ChangeScene("SceneStageSave_1", 0.0f);
#endif
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�폜��
=========================================== */
void SceneManager::Uninit()
{
	m_pScene->Uninit();	// �I������
}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void SceneManager::Update()
{
	m_pScene->Update();	// �X�V����
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void SceneManager::Draw()
{
	m_pScene->Draw();	// �`�揈��

	// �V�[���ύX�������w������Ă�����
	if (m_isSceneChange)	CommitSceneChange();
}
/* ========================================
	�V�[���ύX�\��֐�
	-------------------------------------
	���e�F�V�[����ύX��\�񂷂�
	-------------------------------------
	����1�F�V�[����
	����2�F�t�F�[�h����
=========================================== */
void SceneManager::ChangeScene(const std::string& sSceneName, float fFadeTime)
{
	if (m_isSceneChange) return;	// �V�[���ύX���͏������Ȃ�

	m_sNextSceneName = sSceneName;	// ���̃V�[������ݒ�
	m_isSceneChange = true;			// �V�[���ēǂݍ��݃t���O�𗧂Ă�

	// �V�[�������݂��Ă��Ȃ��ꍇ�͑����ɕύX
	if (!m_pScene)
	{
		InitScene();
		return;
	}

}

/* ========================================
	�V�[���ēǂݍ��ݗ\��֐�
	-------------------------------------
	���e�F�V�[���ēǂݍ��݂�\�񂷂�
	-------------------------------------
	����1�F�t�F�[�h����
=========================================== */
void SceneManager::ReloadScene(float fFadeTime)
{
	if (m_isSceneChange) return;	// �V�[���ύX���͏������Ȃ�

	m_sNextSceneName = m_pScene->GetSceneName();	// ���݂̃V�[�������Đݒ�
	m_isSceneChange = true;						// �V�[���ēǂݍ��݃t���O�𗧂Ă�
}


/* ========================================
	�V�[���ύX���s�֐�
	-------------------------------------
	���e�F�V�[���ύX�����s����
=========================================== */
void SceneManager::CommitSceneChange()
{
	// �V�[�������݂��Ă�����I���������s��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();  // �I������
	}

	// �V�����V�[���𐶐�
	m_pScene = m_SceneMap[m_sNextSceneName]();

	m_pScene->Init();  // ����������

	m_isSceneChange = false;	// �V�[���ēǂݍ��݃t���O��������

}


/* ========================================
	�V�[���������֐�
	-------------------------------------
	���e�F�V�[��������
=========================================== */
void SceneManager::InitScene()
{
	// �V�[�������݂��Ă��Ȃ��ꍇ�͑����ɕύX
	CommitSceneChange();

}



/* ========================================
	�V�[���o�^�֐�
	-------------------------------------
	���e�F�V�[����o�^����
=========================================== */
void SceneManager::RegisterScene(const std::string& sSceneName, CreateSceneFunc func)
{
	m_SceneMap[sSceneName] = func;
}

/* ========================================
	�Q�b�^�[(�V�[��)�֐�
	-------------------------------------
	�ߒl�FSceneBase*		�V�[��
=========================================== */
SceneBase* SceneManager::GetScene()
{
	return m_pScene.get();
}

/* ========================================
	�Q�b�^�[(�V�[�������X�g)�֐�
	-------------------------------------
	�ߒl�Fstd::vector<std::string>		�V�[�������X�g
=========================================== */
std::vector<std::string> SceneManager::GetSceneNameList()
{
	std::vector<std::string> sSceneNameList;
	for (auto& scene : m_SceneMap)
	{
		sSceneNameList.push_back(scene.first);
	}
	return sSceneNameList;
}

