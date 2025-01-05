/* ========================================
	DX22/Dropin
	------------------------------------
	�A�j�����f���f�[�^�Ǘ��pcpp
	------------------------------------
	AnimeModelManager.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ModelAnimeManager.h"

// =============== �萔��` =======================


// ���f���f�[�^�ۑ���p�X
const std::unordered_map < ANIME_BASE_KEY, std::string > BASE_MODEL_FBX_PATH
{
	{ANIME_BASE_KEY::AB_PLAYER,			"Assets/Model/Anime/Player/Player_Base.fbx" },			// �v���C���[
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	"Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Base.fbx" },	// �GMouse
};

// �e���f���f�[�^�傫���w��
const std::unordered_map < ANIME_BASE_KEY, float > BASE_MODEL_FBX_SCALE
{
	{ANIME_BASE_KEY::AB_PLAYER,			0.4f},
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	0.4f},
};

// �v���C���[(�L)�A�j���[�V�����f�[�^�ۑ���p�X
const std::unordered_map < int, std::string > ANIME_DATA_PLAYER_CAT_PATH
{
	{ANIME_KEY_PLAYER::PLYR_IDLE		, "Assets/Model/Anime/Player/Player_Idle.fbx" },	// �ҋ@
	{ANIME_KEY_PLAYER::PLYR_WALK		, "Assets/Model/Anime/Player/Player_Walk.fbx" },	// ���s
	{ANIME_KEY_PLAYER::PLYR_SHOT		, "Assets/Model/Anime/Player/Player_Shot.fbx" },	// �V���b�g
};


// �G�L����(�l�Y�~)�A�j���[�V�����f�[�^�ۑ���p�X
const std::unordered_map < int, std::string > ANIME_DATA_ENEMY_MOUSE_PATH
{
	{ANIME_KEY_ENEMY_MOUSE::ENMYM_IDLE		, "Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Idle.fbx" },	// �ҋ@
	{ANIME_KEY_ENEMY_MOUSE::ENMYM_WALK		, "Assets/Model/Anime/Enemy/Mouse/EnemyMouse_Walk.fbx" },	// ���s
	
};


// �A�j���[�V�����f�[�^�}�b�v�R�Â�
// ���f���f�[�^�L�[�Ɗe�A�j���[�V�����}�b�v��R�Â���
const std::unordered_map < ANIME_BASE_KEY, std::unordered_map < int, std::string > > ANIME_DATA_MAP_PATH
{
	{ANIME_BASE_KEY::AB_ENEMY_MOUSE,	ANIME_DATA_ENEMY_MOUSE_PATH},
	{ANIME_BASE_KEY::AB_PLAYER,		ANIME_DATA_PLAYER_CAT_PATH},
};




/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
========================================== */
ModelAnimeManager::ModelAnimeManager()
	: m_bIsInit(false)
{
	m_apAnimeModelDatas.clear();
	
}

/* ========================================
	�C���X�^���X�擾�֐�
	-------------------------------------
	���e�F�C���X�^���X���擾����
	-------------------------------------
	�ߒl�F�C���X�^���X
========================================== */
ModelAnimeManager& ModelAnimeManager::GetInstance()
{
	static ModelAnimeManager instance;
	return instance;
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
========================================== */
void ModelAnimeManager::Init()
{
	// ���ɏ���������Ă�����I��
	if (m_bIsInit) return;

	// ���f���f�[�^��ǂݍ���
	for (std::pair<ANIME_BASE_KEY, std::string> PathMap : BASE_MODEL_FBX_PATH)
	{
		ANIME_BASE_KEY eNowKey = PathMap.first;	// ���݂̃L�[
		std::string sNowPath = PathMap.second;	// ���݂̕ۑ��ʒu�p�X

		ModelAnime* pModel = new ModelAnime();
		if (!pModel->Load(sNowPath.c_str(), BASE_MODEL_FBX_SCALE.at(eNowKey), ModelAnime::E_Flip::XFlip))	// 3D���f���ǂݍ���
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//�G���[���b�Z�[�W�̕\��
		}

		m_apAnimeModelDatas.insert(std::make_pair(eNowKey, pModel));	// ���f���f�[�^�z��ɒǉ�
	}

	// �A�j���[�V�����I�u�W�F�N�g��ޕ�
	for (int i = 0; i < E_ANIME_BASE_KEY::AB_MAX; i++)
	{
		// �A�j���[�V�����f�[�^�}�b�v�擾
		std::unordered_map < int, std::string > dataPath = ANIME_DATA_MAP_PATH.at(static_cast<ANIME_BASE_KEY>(i));
		// �A�j���[�V�����f�[�^�ǂݍ���
		LoadAnimeData(static_cast<ANIME_BASE_KEY>(i), dataPath);
	}



	m_bIsInit = true;
}


/* ========================================
	�A�j���[�V�����f�[�^�ǂݍ���
	-------------------------------------
	����1�F���f���f�[�^�̃L�[
	����2�F�A�j���[�V�����f�[�^�̃p�X�}�b�v
	-------------------------------------
	���e�F�n���ꂽ���f����ރL�[�ɍ��킹��
		�@�A�j���[�V�����f�[�^��ǂݍ���
========================================== */
void ModelAnimeManager::LoadAnimeData(E_ANIME_BASE_KEY e_GetKey, std::unordered_map<int, std::string> dataPath)
{
	for(std::pair<int, std::string> PathMap : dataPath)
	{
		if (m_apAnimeModelDatas.at(e_GetKey)->AddAnimation(PathMap.second.c_str()) == ModelAnime::ANIME_NONE)
		{
			MessageBox(NULL, PathMap.second.c_str(), "Error", MB_OK);	//�G���[���b�Z�[�W�̕\��
		}

	}

}


/* ========================================
	���f���f�[�^�擾�֐�
	-------------------------------------
	���e�F�L�[�����ɊY���̃��f���f�[�^��n��
		�@���f�B�[�v�R�s�[
	-------------------------------------
	����1�F�L�[
	-------------------------------------
	�ߒl�F���f���|�C���^
========================================== */
ModelAnime* ModelAnimeManager::GetModelData(E_ANIME_BASE_KEY e_GetKey)
{
	return new ModelAnime(*m_apAnimeModelDatas.at(e_GetKey).get());
	//return m_apAnimeModelDatas.at(e_GetKey).get();
}

/* ========================================
	�Q�b�^�[(�A�j���[�V�������f���z��)�֐�
	-------------------------------------
	�ߒl�Fstd::unordered_map<E_MODEL_KEY, std::shared_ptr<ModelStatic>>	���f���f�[�^�z��
=========================================== */
std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> ModelAnimeManager::GetModelDatas()
{
	return m_apAnimeModelDatas;
}

/* ========================================
	�Q�b�^�[(���f���f�[�^�p�X)�֐�
	-------------------------------------
	�ߒl�Fstd::unordered_map<E_MODEL_KEY, std::string>	���f���f�[�^�p�X
========================================== */
std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> ModelAnimeManager::GetModelPaths()
{
	return BASE_MODEL_FBX_PATH;
}

/* ========================================
	�Q�b�^�[(�A�j���[�V�����f�[�^�p�X)�֐�
	-------------------------------------
	����1�F�L�[
	-------------------------------------
	�ߒl�Fstd::unordered_map<int, std::string>	�A�j���[�V�����f�[�^�p�X
========================================== */
std::unordered_map<int, std::string> ModelAnimeManager::GetAnimePaths(E_ANIME_BASE_KEY e_GetKey)
{
	return ANIME_DATA_MAP_PATH.at(e_GetKey);
}


