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
#include "ObjectTypeRegistry.h"

// �V�[��
#include <type_traits>
#include "SceneStageSaves.h"

// =============== �萔��` =======================
const std::string DEFAULT_SAVE_PATH = "Assets/Save/";	// �f�t�H���g�̕ۑ���


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
		"ObjectTypeList",
		"TransformEdit",
	};

	// �V�[�����A�V�[���ύX�֐��̃}�b�v
	const std::map< std::string, std::function<void()>> SCENE_MAP =
	{
		{"SceneStageSave_1", []() { SceneManager::ChangeScene<SceneStageSave_1>(); } },
		{"SceneStageSave_2", []() { SceneManager::ChangeScene<SceneStageSave_2>(); } },
		{"SceneStageSave_3", []() { SceneManager::ChangeScene<SceneStageSave_3>(); } },
		{"SceneStageSave_4", []() { SceneManager::ChangeScene<SceneStageSave_4>(); } },
		{"SceneStageSave_5", []() { SceneManager::ChangeScene<SceneStageSave_5>(); } },
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
			Menu::Create(WIN_NAME[i]).SetEnable(true);
		}

		InitBasicSettings();
		InitCameraInfo();
		InitSceneList();
		InitDataInOut();
		InitObjectTypeList();
		InitTransformEdit();


	}

	/* ========================================
		�E�B���h�E������(��{�ݒ�)�֐�
		-------------------------------------
		���e�F��{�ݒ�̏��������s��
	=========================================== */
	void InitBasicSettings()
	{
		// �w�i�F
		WIN_BASIC_SETTING.AddItem(Item::CreateValue("BackColor", Item::Color, true));
		// �����蔻��\��
		WIN_BASIC_SETTING.AddItem(Item::CreateValue("DispCollision", Item::Bool, true));

		// �O���b�h��
		// �l��ύX��������̏��������s���֐�
		Item::Callback FuncClickSave = [](bool isWrite, void* arg) {
			if (isWrite)
				GridLine::Init();
		};
		Item* pGridGroup = Item::CreateGroup("Grid");
		pGridGroup->AddGroupItem(Item::CreateCallBack("GridEnable", Item::Bool, FuncClickSave, true));	// �O���b�h���̕\��
		pGridGroup->AddGroupItem(Item::CreateCallBack("AxisEnable", Item::Bool, FuncClickSave, true, true));	// �����̕\��
		pGridGroup->AddGroupItem(Item::CreateCallBack("Size", Item::Float, FuncClickSave, true));	// �}�X��
		pGridGroup->AddGroupItem(Item::CreateCallBack("Margin", Item::Float, FuncClickSave, true));	// ����
		pGridGroup->AddGroupItem(Item::CreateCallBack("Color", Item::Color, FuncClickSave, true));	// ���F

		WIN_BASIC_SETTING.AddItem(pGridGroup);

		Item* pDrawGroup = Item::CreateGroup("Draw");
		// FPS
		pDrawGroup->AddGroupItem(Item::CreateValue("FPS", Item::Text, false));
		// �|�[�Y
		pDrawGroup->AddGroupItem(Item::CreateValue("Pause", Item::Bool, false, true));
		// ���t���b�V�����[�g
		pDrawGroup->AddGroupItem(Item::CreateValue("MaxFPS", Item::Float, false));

		WIN_BASIC_SETTING.AddItem(pDrawGroup);

		WIN_BASIC_SETTING["Draw"]["Pause"].SetBool(false);
		WIN_BASIC_SETTING["Draw"]["MaxFPS"].SetFloat(WindowConfig::REFRESH_RATE);

	}


	/* ========================================
		�E�B���h�E������(�J�������)�֐�
		-------------------------------------
		���e�F�J�������̏��������s��
	=========================================== */
	void InitCameraInfo()
	{
		WIN_CAMERA_INFO.AddItem(Item::CreateValue("Active", Item::Kind::Text));	// �A�N�e�B�u�J������

		WIN_CAMERA_INFO.AddItem(Item::CreateValue("Orthographic", Item::Kind::Bool));	// ���s���e���ǂ���

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
		},false, false);

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
		// �t�@�C���ꏊ
		WIN_DATA_INOUT.AddItem(Item::CreateValue("SavePath", Item::Kind::Path));	
		// �f�t�H���g�t�@�C���ꏊ�ݒ�{�^��
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("Def", Item::Kind::Command, [](bool isWrite, void* arg)
		{
			WIN_DATA_INOUT["SavePath"].SetPath(DEFAULT_SAVE_PATH);
		}, false, true));

		// �t�@�C����
		WIN_DATA_INOUT.AddItem(Item::CreateValue("FileName", Item::Kind::Path));	

		// �g���q���X�g
		WIN_DATA_INOUT.AddItem(Item::CreateList("Extension", nullptr, false, true, true));
		WIN_DATA_INOUT["Extension"].AddListItem("");
		WIN_DATA_INOUT["Extension"].AddListItem(".stg");
		WIN_DATA_INOUT["Extension"].AddListItem(".txt");

		// �t�@�C���o��
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("OutPut", Item::Kind::Command,[](bool isWrite, void* arg) 
		{
			std::string sPath		=  WIN_DATA_INOUT["SavePath"].GetPath();
			std::string sFileName	= WIN_DATA_INOUT["FileName"].GetPath();
			std::string sExtension	= WIN_DATA_INOUT["Extension"].GetListText(WIN_DATA_INOUT["Extension"].GetInt());
			FileManager::StageObjectOutput(sPath + "/" + sFileName + sExtension);
		}));

		// �t�@�C������
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("InPut", Item::Kind::Command, [](bool isWrite, void* arg) 
		{
			std::string sPath		= WIN_DATA_INOUT["SavePath"].GetPath();
			std::string sFileName	= WIN_DATA_INOUT["FileName"].GetPath();
			std::string sExtension	= WIN_DATA_INOUT["Extension"].GetListText(WIN_DATA_INOUT["Extension"].GetInt());
			FileManager::StageObjectInput(sPath + "/" + sFileName + sExtension);
		}, false, true));


		WIN_DATA_INOUT.AddItem(Item::CreateValue("InputResult ", Item::Kind::Text, false));	// �Ǎ����ʕ\��
		WIN_DATA_INOUT.AddItem(Item::CreateValue("OutputResult", Item::Kind::Text, false));	// �o�͌��ʕ\��

		// �����l
		WIN_DATA_INOUT["SavePath"].SetPath(DEFAULT_SAVE_PATH);
	}

	/* ========================================
		�E�B���h�E������(�I�u�W�F�N�g�^�C�v�ꗗ)�֐�
		-------------------------------------
		���e�F�I�u�W�F�N�g�^�C�v�ꗗ�̏��������s��
	=========================================== */
	void InitObjectTypeList()
	{
		// �I�u�W�F�N�g�^�C�v�ꗗ
		Item* pObjectTypeList = Item::CreateList("ObjectTypes", [](const void* arg)
		{
			// �I�������炻�̃^�C�v�̃I�u�W�F�N�g�𐶐�
			std::string sObjTypeName = reinterpret_cast<const char*>(arg);	// ���X�g���ږ�

			// �I�u�W�F�N�g����
			ObjectBase* pObject = OBJ_TYPE_REGISTRY_INST.CreateObject(sObjTypeName);
			if (pObject)
			{
				SceneBase* pScene = SceneManager::GetScene();			// �V�[���擾
				pObject->Init(pScene->CreateUniqueName(sObjTypeName));	// �I�u�W�F�N�g������(���O�d��������)
				pScene->AddSceneObjectBase(pObject);					// �V�[���ɒǉ�
			}

		}, false);

		// ���X�g������
		std::unordered_map<std::string, ObjectTypeRegistry::CreateFunction*> objectTypeMap = OBJ_TYPE_REGISTRY_INST.GetObjectTypeMap();
		for (auto& objectType : objectTypeMap)
		{
			pObjectTypeList->AddListItem(objectType.first.c_str());
		}

		WIN_OBJ_TYPE_LIST.AddItem(pObjectTypeList);
	}

	/* ========================================
		�E�B���h�E������(�I�u�W�F�N�g�ό`�G�f�B�^)�֐�
		-------------------------------------
		���e�F�I�u�W�F�N�g�ό`�G�f�B�^�̏��������s��
	=========================================== */
	void InitTransformEdit()
	{
		// �ҏW���[�h�\��
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("Mode", Item::Kind::Text, false));	


		// �ҏW���[�hOff�{�^��
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("EditOff", Item::Kind::Command,	[](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(0); }, false, true));

		// ���Z�b�g�{�^��
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Reset", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->ResetTransformEdit(); }, false, true));

		// �ҏW���[�h�؂�ւ�(�ړ�)
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Position", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(1);}));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValuePos", Item::Kind::Float, false, true));
		// �ҏW���[�h�؂�ւ�(��])
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Rotation", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(2); }));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValueRot", Item::Kind::Float, false, true));
		// �ҏW���[�h�؂�ւ�(�g�k)
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Scale   ", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(3); }));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValueScale", Item::Kind::Float, false, true));


		// �����l
		WIN_TRANSFORM_EDIT["ValuePos"].SetFloat(1.0f);
		WIN_TRANSFORM_EDIT["ValueRot"].SetFloat(10.0f);
		WIN_TRANSFORM_EDIT["ValueScale"].SetFloat(1.0f);
	}

}

#endif // _DEBUG