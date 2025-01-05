/* ========================================
	DX22/Dropin
	------------------------------------
	�A�j�����f���f�[�^�Ǘ��p�w�b�_
	------------------------------------
	����:�A�j�����f���̃{�[���A�e�A�j���[�V�������܂Ƃ߂ēǂݍ���ŊǗ�����
	------------------------------------
	AnimeModelManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ModelAnime.h"
#include "Shader.h"
#include <vector>
#include <unordered_map>
#include <memory>

// =============== �萔��` =======================
#define MODEL_ANIME_MNG_INST	ModelAnimeManager::GetInstance()	// �C���X�^���X�擾

#define GET_MODEL_ANIME ModelAnimeManager::GetInstance().GetModelData	// �C���X�^���X�擾

#define ANIME_BASE_KEY			ModelAnimeManager::E_ANIME_BASE_KEY		// �A�j���[�V�������f���擾�L�[�ȗ�
#define ANIME_KEY_PLAYER		ModelAnimeManager::E_ANIME_KEY_PLAYER // �v���C���[�A�j���[�V������ގ擾�L�[�ȗ�
#define ANIME_KEY_ENEMY_MOUSE	ModelAnimeManager::E_ANIME_KEY_ENEMY_MOUSE	// �G�A�j���[�V������ގ擾�L�[�ȗ�

class ModelAnimeManager
{
public:
	// ���f���f�[�^���
	enum E_ANIME_BASE_KEY
	{
		AB_PLAYER,			// �v���C���[(�L)
		AB_ENEMY_MOUSE,		// �G(�l�Y�~)
		AB_MAX,
	};

	enum E_ANIME_KEY_PLAYER
	{
		PLYR_IDLE,		// �ҋ@
		PLYR_WALK,		// ���s
		PLYR_SHOT,		// �V���b�g
		PLYR_MAX,		
	};

	enum E_ANIME_KEY_ENEMY_MOUSE
	{
		ENMYM_IDLE,		// �ҋ@
		ENMYM_WALK,		// ���s
		ENMYM_MAX,
	};


public:
	static ModelAnimeManager& GetInstance();
	void Init();

	// �Q�b�^�[
	ModelAnime* GetModelData(E_ANIME_BASE_KEY e_GetKey);

	std::unordered_map <E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> GetModelDatas();
	std::unordered_map <E_ANIME_BASE_KEY, std::string> GetModelPaths();
	std::unordered_map <int, std::string> GetAnimePaths(E_ANIME_BASE_KEY e_GetKey);

private:
	ModelAnimeManager();// �R���X�g���N�^
	void LoadAnimeData(E_ANIME_BASE_KEY e_GetKey, std::unordered_map<int, std::string> dataPath);	// �A�j���[�V�����f�[�^�ǂݍ���

	std::unordered_map <E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> m_apAnimeModelDatas;	// ���f���f�[�^���i�[���Ă���
	bool m_bIsInit;	// �������t���O


};
