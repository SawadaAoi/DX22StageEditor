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
		DEBUG_CAMERA_ICON,	// �f�o�b�O�p�J�����A�C�R��
		DEBUG_LIGHT_ICON,	// �f�o�b�O�p���C�g�A�C�R��
		DEBUG_PLAYER_START,	// �v���C���[�X�^�[�g�ʒu
		BLOCK_SIMPLE,		// �V���v���ȃu���b�N
		BLOCK_GRASS,		// ���n
		BLOCK_GRASS_TOP,	// ���n(��)
		BLOCK_GRASS_SIDE,	// ���n(��)
		BLOCK_NOGRASS_SIDE,	// ���n�Ȃ�(��)
		BLOCK_BRICK,		// �����K�u���b�N
		BLOCK_BRICK_CRACK,	// �����K�u���b�N(�T��)
		PLAYER_BULLET,		// �v���C���[�e
		WALL_FENCE,			// ��(�t�F���X)
		UI_TITLE_LOGO,		// �^�C�g�����S
		UI_PLAYER_ICON,		// �v���C���[�A�C�R��
		UI_HP_HEART,		// HP�n�[�g
		EFFECT_EXPLOSION,	// �����G�t�F�N�g
		EFFECT_SMOKE,		// ���G�t�F�N�g
		STAGE_IMG_STAGE1,	// �X�e�[�W1�摜
		MAX,
	};

	// �摜�f�[�^�ۑ���p�X
	const std::unordered_map < TextureManager::E_TEX_KEY, std::string > MODEL_TEX_PATH
	{
		{TEX_KEY::TEST,					"Assets/Texture/Test.png"						},
		{TEX_KEY::DEBUG_CAMERA_ICON,	"Assets/Texture/Debug/Debug_Camera_Icon.png"	},
		{TEX_KEY::DEBUG_LIGHT_ICON,		"Assets/Texture/Debug/Debug_Light_Icon.png"		},
		{TEX_KEY::DEBUG_PLAYER_START,	"Assets/Texture/Debug/Debug_PlayerStart.png"	},
		{TEX_KEY::BLOCK_SIMPLE,			"Assets/Texture/Block/Block_Simple.png"			},
		{TEX_KEY::BLOCK_GRASS,			"Assets/Texture/Block/Block_Grass.png"			},
		{TEX_KEY::BLOCK_GRASS_TOP,		"Assets/Texture/Block/Block_Grass_Top.png"		},
		{TEX_KEY::BLOCK_GRASS_SIDE,		"Assets/Texture/Block/Block_Grass_Side.png"		},
		{TEX_KEY::BLOCK_NOGRASS_SIDE,	"Assets/Texture/Block/Block_NoGrass_Side.png"	},
		{TEX_KEY::BLOCK_BRICK,			"Assets/Texture/Block/Block_Brick.png"			},
		{TEX_KEY::BLOCK_BRICK_CRACK,	"Assets/Texture/Block/Block_Brick_Crack.png"	},
		{TEX_KEY::PLAYER_BULLET,		"Assets/Texture/Player_Bullet.png"				},
		{TEX_KEY::WALL_FENCE,			"Assets/Texture/Wall_Fence.png"					},
		{TEX_KEY::UI_TITLE_LOGO,		"Assets/Texture/UI/UI_TitleLogo.png"			},
		{TEX_KEY::UI_PLAYER_ICON,		"Assets/Texture/UI/UI_PlayerIcon.png"			},
		{TEX_KEY::UI_HP_HEART,			"Assets/Texture/UI/UI_HpHeart.png"				},
		{TEX_KEY::EFFECT_EXPLOSION,		"Assets/Texture/Effect_Explosion.png"			},
		{TEX_KEY::EFFECT_SMOKE,			"Assets/Texture/Effect_Smoke.png"				},
		{TEX_KEY::STAGE_IMG_STAGE1,		"Assets/Texture/StageImage/Stage1.png"			},


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

