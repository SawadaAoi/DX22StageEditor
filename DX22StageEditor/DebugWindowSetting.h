/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�E�B���h�E�ݒ�p�w�b�_
	------------------------------------
	�����F�e�f�o�b�O�E�B���h�E�̏��������Ǘ�����
	------------------------------------
	DebugWindowSetting.h
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��
#pragma once

// =============== �C���N���[�h ===================
#include <string>

// =============== �萔��` =======================
// �E�B���h�E�Ăяo���p�}�N��
#define WIN_BASIC_SETTING	DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::BASIC_SETTINGS])
#define WIN_CAMERA_INFO		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::CAMERA_INFO])
#define WIN_SCENE_LIST		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::SCENE_LIST])
#define WIN_OBJ_LIST		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_LIST])
#define WIN_OBJ_INFO		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_INFO])
#define WIN_DATA_INOUT		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::DATA_INOUT])
#define WIN_OBJ_TYPE_LIST	DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_TYPE_LIST])

// �A�C�e���Ăяo���p�}�N��
const std::string ITEM_OBJ_LIST_NAME = "Objects";
#define ITEM_OBJ_LIST	WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()]


// =============== ���O��� ===================
namespace DebugUI
{
	const std::string CHILD_HEAD_TEXT = "L";	// �q�v�f�̐擪�ɕ\�����镶��
	const std::string CHILD_HEAD_SPACE = " ";	// �q�v�f�̐擪�ɕ\������X�y�[�X


	// �E�B���h�E���
	enum E_WinType
	{	
		MAIN_WINDOW,		// ���C���E�B���h�E
		BASIC_SETTINGS,		// ��{�ݒ�
		CAMERA_INFO,
		SCENE_LIST,			// �V�[���ꗗ
		OBJECT_LIST,		// �I�u�W�F�N�g�ꗗ
		OBJECT_INFO,		// �I�u�W�F�N�g���
		DATA_INOUT,			// �f�[�^���o��
		OBJECT_TYPE_LIST,	// �I�u�W�F�N�g�^�C�v�ꗗ
		MAX,
	};

	// �E�B���h�E��
	extern const char* WIN_NAME[E_WinType::MAX];

	// �e���j���[�̏�����
	void InitDebugWindow();
	void InitBasicSettings();
	void InitCameraInfo();
	void InitSceneList();
	void InitDataInOut();
	void InitObjectTypeList();

}

#endif // _DEBUG