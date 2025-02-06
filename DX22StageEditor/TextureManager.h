/* ========================================
	DX22/Dropin
	------------------------------------
	�摜�f�[�^�Ǘ��p�w�b�_
	------------------------------------
	����:�摜�f�[�^���܂Ƃ߂ēǂݍ���ŊǗ�����
	------------------------------------
	TextureManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Texture.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

// =============== �萔��` =======================
#define TEX_KEY				TextureManager::E_TEX_KEY	// �L�[�ȗ�
#define GET_TEXTURE_DATA	TextureManager::GetInstance().GetTextureData	// ���͏ȗ�
#define TEXTURE_MNG_INST TextureManager::GetInstance()	// �C���X�^���X�擾

class TextureManager
{
public:
	// ���f���f�[�^���
	enum E_TEX_KEY
	{
		TEST,				// �e�X�g�摜
		TEST2,				// 
		CAMERA_ICON,		// �f�o�b�O�p�J�����A�C�R��
		LIGHT_ICON,			// �f�o�b�O�p���C�g�A�C�R��
		BLOCK_SIMPLE,		// �V���v���ȃu���b�N
		GROUND_GRASS,		// ���n
		PLAYER_HP,			// �v���C���[HP
		PLAYER_BULLET,		// �v���C���[�e
		STAGE_GOAL,			// �S�[��
		WALL_FENCE,			// ��(�t�F���X)
		PLAYER_STARTPOS,	// �v���C���[�X�^�[�g�ʒu
		TITLE_LOGO,			// �^�C�g�����S
		EFFECT_EXPLOSION,	// �����G�t�F�N�g
		EFFECT_SMOKE,		// ���G�t�F�N�g
		MAX,
	};

	// �摜�f�[�^�ۑ���p�X
	const std::unordered_map < TextureManager::E_TEX_KEY, std::string > MODEL_TEX_PATH
	{
		{TEX_KEY::TEST,				"Assets/Texture/Test.png"				},	// �e�X�g�摜
		{TEX_KEY::TEST2,			"Assets/Texture/Star.png"				},
		{TEX_KEY::CAMERA_ICON,		"Assets/Texture/Debug_Camera_Icon.png"	},
		{TEX_KEY::LIGHT_ICON,		"Assets/Texture/Debug_Light_Icon.png"	},
		{TEX_KEY::BLOCK_SIMPLE,		"Assets/Texture/Block_Simple.png"		},
		{TEX_KEY::GROUND_GRASS,		"Assets/Texture/Ground_Grass.png"		},
		{TEX_KEY::PLAYER_HP,		"Assets/Texture/Player_Hp.png"			},
		{TEX_KEY::PLAYER_BULLET,	"Assets/Texture/Player_Bullet.png"		},
		{TEX_KEY::STAGE_GOAL,		"Assets/Texture/Stage_Goal.png"			},
		{TEX_KEY::WALL_FENCE,		"Assets/Texture/Wall_Fence.png"			},
		{TEX_KEY::PLAYER_STARTPOS,	"Assets/Texture/Player_StartPos.png"	},
		{TEX_KEY::TITLE_LOGO,		"Assets/Texture/Title_Logo.png"			},
		{TEX_KEY::EFFECT_EXPLOSION, "Assets/Texture/Effect_Explosion.png"	},
		{TEX_KEY::EFFECT_SMOKE,		"Assets/Texture/Effect_Smoke.png"		},

	};

public:
	static TextureManager& GetInstance();
	void Init();

	// �Q�b�^�[
	Texture* GetTextureData(E_TEX_KEY e_GetKey);
	E_TEX_KEY GetTextureKey(Texture* pTex);

	std::unordered_map <E_TEX_KEY, std::shared_ptr<Texture>> GetTextureDatas();
	std::unordered_map <E_TEX_KEY, std::string> GetTexturePaths();
private:
	TextureManager();
private:
	std::unordered_map <E_TEX_KEY, std::shared_ptr<Texture>> m_apTextureDatas;	// ���f���f�[�^���i�[���Ă���
	bool bIsInit;

};

