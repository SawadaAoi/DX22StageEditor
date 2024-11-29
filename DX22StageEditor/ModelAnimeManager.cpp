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
	{ANIME_BASE_KEY::AB_PLAYER, "Assets/Model/Player/Player_Base2.fbx" },	// �v���C���[
	{ANIME_BASE_KEY::AB_ENEMY_G1, "Assets/Model/Enemy/Enemy_G1/Enemy_G1_Base.fbx" },	// �GG1
	{ANIME_BASE_KEY::AB_ENEMY_Y2, "Assets/Model/Enemy/Enemy_Y2/Enemy_Y2_Base.fbx" },	// �GY2
	{ANIME_BASE_KEY::AB_ENEMY_R3, "Assets/Model/Enemy/Enemy_R3/Enemy_R3_Base.fbx" },	// �GR3
};

// �e���f���f�[�^�傫���w��
const std::unordered_map < ANIME_BASE_KEY, float > BASE_MODEL_FBX_SCALE
{
	{ANIME_BASE_KEY::AB_PLAYER,	0.7f},
	{ANIME_BASE_KEY::AB_ENEMY_G1,	1.0f},
	{ANIME_BASE_KEY::AB_ENEMY_Y2,	1.0f},
	{ANIME_BASE_KEY::AB_ENEMY_R3,	1.0f},
};

// �v���C���[�A�j���[�V�����f�[�^�ۑ���p�X
const std::unordered_map < int, std::string > ANIME_DATA_PATH_PLAYER
{
	{ANIME_KEY_PLAYER::PLYR_IDLE		, "Assets/Model/Player/Player_Idle.fbx" },	// �ҋ@
	{ANIME_KEY_PLAYER::PLYR_WALK		, "Assets/Model/Player/Player_Walk.fbx" },	// ���s
	{ANIME_KEY_PLAYER::PLYR_SHOT		, "Assets/Model/Player/Player_Shot.fbx" },	// �ˌ�
	{ANIME_KEY_PLAYER::PLYR_WALK_SHOT	, "Assets/Model/Player/Player_Walk_Shot.fbx" },	// ���s�ˌ�
	{ANIME_KEY_PLAYER::PLYR_GAMECLEAR	, "Assets/Model/Player/Player_GameClear.fbx" },	// �Q�[���N���A
};

// �G�L�����A�j���[�V�����f�[�^�ۑ���p�X
const std::unordered_map < int, std::string > ANIME_DATA_ENEMY_PATH
{
	{ANIME_KEY_ENEMY::ENMY_IDLE		, "Assets/Model/Enemy/Enemy_Idle.fbx" },	// �ҋ@
	{ANIME_KEY_ENEMY::ENMY_WALK		, "Assets/Model/Enemy/Enemy_Walk.fbx" },	// ���s
	{ANIME_KEY_ENEMY::ENMY_NOT_ANIME	, "Assets/Model/Enemy/Enemy_NotAnime.fbx" },	// �A�j���[�V��������(�������)
	{ANIME_KEY_ENEMY::ENMY_VIBRATION	, "Assets/Model/Enemy/Enemy_Vibration.fbx" },	// �U��(��~���)

};

// �A�j���[�V�����f�[�^�}�b�v�R�Â�
// ���f���f�[�^�L�[�Ɗe�A�j���[�V�����}�b�v��R�Â���
const std::unordered_map < ANIME_BASE_KEY, std::unordered_map < int, std::string > > ANIME_DATA_MAP_PATH
{
	{ANIME_BASE_KEY::AB_PLAYER, ANIME_DATA_PATH_PLAYER},
	{ANIME_BASE_KEY::AB_ENEMY_G1, ANIME_DATA_ENEMY_PATH},
	{ANIME_BASE_KEY::AB_ENEMY_Y2, ANIME_DATA_ENEMY_PATH},
	{ANIME_BASE_KEY::AB_ENEMY_R3, ANIME_DATA_ENEMY_PATH},
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
		m_apAnimeModelDatas.at(e_GetKey)->AddAnimation(PathMap.second.c_str());

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


