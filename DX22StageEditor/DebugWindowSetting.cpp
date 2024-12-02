/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�E�B���h�E�ݒ�pcpp
	------------------------------------
	DebugWindowSetting.cpp
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��

// =============== �C���N���[�h ===================
#include "DebugWindowSetting.h"
#include "DebugMenu.h"
#include "GridLine.h"	// �O���b�h���ēǂݍ���
#include "SettingWindowAPI.h"
#include "SceneManager.h"
#include "CameraManager.h"	// �J�����}�l�[�W��
#include "FileManager.h"
// �V�[��
#include <type_traits>
#include "SceneGameTest.h"


// =============== ���O��� ===================
namespace DebugUI
{

	// �E�B���h�E��(const char*�Œ�`���Ă��邽�߁A�錾�ƒ�`�𕪂��Ă���)
	// string�ł͂Ȃ�const char*�Ȃ̂́ADebugMenu�̑����̊֐��ŕ������const char*�ň����Ă��邽��
	const char* WIN_NAME[E_WinType::MAX] =
	{
		"DebugWindowList",
		"BasicSettings",
		"CameraInfo",
		"SceneList",
		"ObjectList",
		"ObjectInfo",
		"DataInOut",
	};

	// �V�[�����A�V�[���ύX�֐��̃}�b�v
	const std::map< std::string, std::function<void()>> SCENE_MAP =
	{
		{"SceneGameTest", []() { SceneManager::ChangeScene<SceneGameTest>(); } },
	};

	/* ========================================
		�f�o�b�O�p�E�B���h�E�������֐�
		-------------------------------------
		���e�F�e�E�B���h�E�̏��������s��
	=========================================== */
	void InitDebugWindow()
	{
		// �e�E�B���h�E����
		for (int i = 1; i < E_WinType::MAX; ++i)
		{
			Menu::Create(WIN_NAME[i]);
		}

		InitBasicSettings();
		InitCameraInfo();
		InitSceneList();
		InitDataInOut();
	}

	/* ========================================
		�E�B���h�E������(��{�ݒ�)�֐�
		-------------------------------------
		���e�F��{�ݒ�̏��������s��
	=========================================== */
	void InitBasicSettings()
	{
		// �w�i�F
		Item* pBackColor = Item::CreateValue("BackColor", Item::Color, true);

		// �O���b�h��
		// �l��ύX��������̏��������s���֐�
		Item::Callback FuncClickSave = [](bool isWrite, void* arg) {
			if (isWrite)
				GridLine::Init();
		};
		Item* pGrid = Item::CreateGroup("Grid");
		pGrid->AddGroupItem(Item::CreateCallBack("Enable", Item::Bool, FuncClickSave, true));	// �O���b�h���̕\��
		pGrid->AddGroupItem(Item::CreateCallBack("Size", Item::Float, FuncClickSave, true));	// �}�X��
		pGrid->AddGroupItem(Item::CreateCallBack("Margin", Item::Float, FuncClickSave, true));	// ����
		pGrid->AddGroupItem(Item::CreateCallBack("Color", Item::Color, FuncClickSave, true));	// ���F
		pGrid->AddGroupItem(Item::CreateCallBack("Axis", Item::Bool, FuncClickSave, true));	// �����̕\��

		// �|�[�Y
		Item* pPause = Item::CreateValue("Pause", Item::Bool, false);
		pPause->SetBool(false);

		// ���t���b�V�����[�g
		Item* pRefreshRate = Item::CreateValue("RefreshRate", Item::Float, false);
		pRefreshRate->SetFloat(WindowConfig::REFRESH_RATE);
		// FPS
		Item* pFPS = Item::CreateValue("FPS", Item::Text, false);

		// ���ڈꗗ
		Item* Items[] =
		{
			pBackColor,
			pGrid,
			pPause,
			pRefreshRate,
			pFPS,
		};

		// �E�B���h�E�ɒǉ�
		for (int i = 0; i < 5; ++i)
		{
			WIN_BASIC_SETTING.AddItem(Items[i]);
		}

	}

	/* ========================================
		�E�B���h�E������(�J�������)�֐�
		-------------------------------------
		���e�F�J�������̏��������s��
	=========================================== */
	void InitCameraInfo()
	{
		WIN_CAMERA_INFO.AddItem(Item::CreateValue("ActiveCamera", Item::Kind::Text));	// �A�N�e�B�u�J������

		WIN_CAMERA_INFO.AddItem(Item::CreateValue("IsOrthographic", Item::Kind::Bool));	// ���s���e���ǂ���

		WIN_CAMERA_INFO.AddItem(Item::CreateList("CameraList", [](const void* arg)		// �J�������X�g
		{
			// �N���b�N���ꂽ�J�������A�N�e�B�u�ɂ���
			std::string sCameraName = reinterpret_cast<const char*>(arg);				// ���X�g���ږ�
			int nCameraNum = WIN_CAMERA_INFO["CameraList"].GetListNo(sCameraName.c_str());	// �J�����ԍ����擾

			CAMERA_MNG_INST.SwitchCamera(nCameraNum);	// �J�����؂�ւ�
		}));

	}

	/* ========================================
		�E�B���h�E������(�V�[���ꗗ)�֐�
		-------------------------------------
		���e�F�V�[���ꗗ�̏��������s��
	=========================================== */
	void InitSceneList()
	{
		// �V�[���ꗗ
		Item* pSceneList = Item::CreateList("SceneLists", [](const void* arg)
		{
			// �I��������V�[���ύX
			std::string sSceneName = reinterpret_cast<const char*>(arg);	// ���X�g���ږ�
			SCENE_MAP.at(sSceneName)();	// �V�[���ύX
		}, false);

		// �V�[���������X�g�ɒǉ�
		for (auto& scene : SCENE_MAP)
		{
			pSceneList->AddListItem(scene.first.c_str());
		}

		WIN_SCENE_LIST.AddItem(pSceneList);
	}

	/* ========================================
		�E�B���h�E������(�f�[�^���o��)�֐�
		-------------------------------------
		���e�F�f�[�^���o�͂̏��������s��
	=========================================== */
	void InitDataInOut()
	{
		WIN_DATA_INOUT.AddItem(Item::CreateValue("DataPath", Item::Kind::Path));	// �f�[�^�p�X
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("OutPut", Item::Kind::Command,
			[](bool isWrite, void* arg) {

			FileManager::StageObjectOutput(WIN_DATA_INOUT["DataPath"].GetPath());
		}));

		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("InPut", Item::Kind::Command,
			[](bool isWrite, void* arg) {

			FileManager::StageObjectInput(WIN_DATA_INOUT["DataPath"].GetPath());
		}));

	}

}

#endif // _DEBUG