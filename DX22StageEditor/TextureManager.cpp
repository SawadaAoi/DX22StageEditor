/* ========================================
	DX22/Dropin
	------------------------------------
	�摜�f�[�^�Ǘ��pcpp
	------------------------------------
	TextureDataManager.cpp
========================================== */


// =============== �C���N���[�h ===================
#include "TextureManager.h"



/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
TextureManager::TextureManager()
	: bIsInit(false)
{
	m_apTextureDatas.clear();
	
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�摜�f�[�^��ǂݍ���
=========================================== */
void TextureManager::Init()
{
	// ���ɏ���������Ă�����I��
	if (bIsInit) return;

	// �摜�f�[�^��ǂݍ���
	for (std::pair<E_TEX_KEY, std::string> PathMap : MODEL_TEX_PATH)
	{
		E_TEX_KEY eNowKey = PathMap.first;		// ���݂̃L�[
		std::string sNowPath = PathMap.second;	// ���݂̕ۑ��ʒu�p�X

		Texture* pTex = new Texture;				// �摜�f�[�^�쐬
		if (FAILED(pTex->Create(sNowPath.c_str())))	// �摜�f�[�^�ǂݍ���
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//�G���[���b�Z�[�W�̕\��
		}

		m_apTextureDatas.insert(std::make_pair(eNowKey, pTex));	// �摜�f�[�^�z��ɒǉ�
	}

	bIsInit = true;
}



/* ========================================
	�C���X�^���X(�V���O���g��)�擾�֐�
	-------------------------------------
	���e�F���N���X�̃C���X�^���X���擾
	-------------------------------------
	�ߒl�F���N���X�̃C���X�^���X
=========================================== */
TextureManager& TextureManager::GetInstance()
{
	static TextureManager instance;
	return instance;
}

/* ========================================
	�摜�f�[�^�擾�֐�
	-------------------------------------
	���e�F�w�肳�ꂽ�摜�f�[�^���擾
	-------------------------------------
	����1�FE_TEX_KEY e_GetKey
	-------------------------------------
	�ߒl�FTexture* �摜�f�[�^
=========================================== */
Texture* TextureManager::GetTextureData(E_TEX_KEY e_GetKey)
{
	return m_apTextureDatas[e_GetKey].get();
}



/* ========================================
	�Q�b�^�[(�摜�f�[�^�z��)�֐�
	-------------------------------------
	�ߒl�Funordered_map<E_TEX_KEY, shared_ptr<Texture>> �摜�f�[�^�z��
=========================================== */
std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> TextureManager::GetTextureDatas()
{
	return m_apTextureDatas;
}

/* ========================================
	�Q�b�^�[(�摜�f�[�^�p�X)�֐�
	-------------------------------------
	�ߒl�Funordered_map<E_TEX_KEY, string> �摜�f�[�^�p�X
=========================================== */
std::unordered_map<TextureManager::E_TEX_KEY, std::string> TextureManager::GetTexturePaths()
{
	return MODEL_TEX_PATH;
}
