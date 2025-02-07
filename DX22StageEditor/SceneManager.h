/* ========================================
	DX22Base/
	------------------------------------
	�V�[���Ǘ��p�w�b�_
	------------------------------------
	�����F�e�V�[���̊Ǘ����s���N���X
	------------------------------------
	SceneManager.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "SceneBase.h"

// =============== �N���X��` =====================
class SceneManager
{
public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	// �V�[���؂�ւ�
	template<typename T>
	static void ChangeScene();

	// �V�[���ēǂݍ���
	static void CallReloadScene();	// �V�[���ēǂݍ��݂��Ăяo��
	static void ReloadScene();		// �V�[���ēǂݍ���

	// �Q�b�^�[
	static SceneBase* GetScene();	// �V�[���擾
	static bool GetIsReloadScene();	// �V�[���ēǂݍ��݃t���O�擾

private:
	static inline std::unique_ptr<SceneBase> m_pScene = nullptr;	// std::unique_ptr�ɕύX
	static inline bool	m_IsReloadScene = false;					// �V�[���ēǂݍ��ݏ����Ăяo���t���O
};

// =============== �e���v���[�g�֐��̎��� =====================


/* ========================================
	�V�[���ύX�֐�
	-------------------------------------
	���e�F�V�[����ύX����
========================================== */
template<typename T>
inline void SceneManager::ChangeScene()
{
	// �V�[�������݂��Ă�����I���������s��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();  // �I������
		// delete�͕s�v�Funique_ptr�������Ń�����������s��
	}

	// �V�����V�[���𐶐��iunique_ptr���Ǘ�����j
	m_pScene = std::make_unique<T>();  // �����������������
	m_pScene->Init();  // ����������

}