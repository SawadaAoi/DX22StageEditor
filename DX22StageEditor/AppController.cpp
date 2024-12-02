/* ========================================
	DX22Base/
	------------------------------------
	�A�v������pcpp
	------------------------------------
	AppController.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "AppController.h"
#include "DirectXManager.h"		// DirectX�Ǘ�			
#include "Input.h"
#include "DebugConsole.h"
#include "ShaderManager.h"

#include "SceneManager.h"
#include "GridLine.h"
#include "CameraManager.h"
#include "TimeManager.h"
#include "ModelStaticManager.h"
#include "ModelAnimeManager.h"
#include "TextureManager.h"
#include "ObjectTypeRegistry.h"

#include "WindowAPI.h"

#include "DebugMenu.h"

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���������s��
=========================================== */
HRESULT AppController::Init()
{
	HRESULT hr;

	// DirectX������
	if(FAILED(hr = DirectXManager::InitDirectX()))
	{
		return hr;
	}

	TimeManager::Init();	// ���ԊǗ�������

	SHADER_INST.Init();	// �V�F�[�_�[�ǂݍ���
	MODEL_STATIC_MNG_INST.Init();	// �ÓI���f���Ǘ�������
	MODEL_ANIME_MNG_INST.Init();	// �A�j���[�V�������f���Ǘ�������
	TEXTURE_MNG_INST.Init();		// �e�N�X�`���Ǘ�������

#ifdef _DEBUG
	DebugUI::Menu::Init();
	DebugUI::InitDebugWindow();
#endif

	ObjectTypeRegistry::RegisterAllObjectTypes();	// �I�u�W�F�N�g�^�C�v�o�^
	SceneManager::Init();	// �V�[���Ǘ�������

	//DebugConsole::Init();	// �f�o�b�O�R���\�[��������



	GridLine::Init();	// �O���b�h��������


	return hr;
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I���������s��
=========================================== */
void AppController::Uninit()
{
#ifdef _DEBUG
	DebugUI::Menu::Uninit();
#endif


	SceneManager::Uninit();	// �V�[���Ǘ��폜


	DebugConsole::Uninit();	// �f�o�b�O�R���\�[���I��

	DirectXManager::UninitDirectX();	// DirectX�I��
}

/* ========================================
	�X�V�����֐�
	-------------------------------------
	���e�F�X�V���s��
	-------------------------------------
	����1�F�O��̃t���[������̌o�ߎ���
=========================================== */
void AppController::Update(float tick)
{

	TimeManager::Update(tick);	// ���ԊǗ��X�V




	SceneManager::Update();

	//DebugConsole::Update();	// �f�o�b�O�R���\�[���X�V

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`����s��
=========================================== */
void AppController::Draw()
{
	DirectXManager::BeginDrawDirectX();	// �r���[�N���A
	GridLine::Draw();		// �O���b�h���`��
	SceneManager::Draw();	// �V�[���`��

#ifdef _DEBUG
	DebugUI::Menu::Draw();
#endif

	DirectXManager::EndDrawDirectX();	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�ɃR�s�[
}


