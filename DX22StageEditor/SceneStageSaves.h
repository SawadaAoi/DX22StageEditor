/* ========================================
    DX22Base/
    ------------------------------------
    �X�e�[�W�f�[�^�ۑ��V�[���p�w�b�_
    ------------------------------------
	�����F�ҏW���̃X�e�[�W�f�[�^��ۑ�����V�[��
		�@�𕡐��p�ӂ���
		  �V�[�����������ɓǂݍ��݁A�I�����ɕۑ�
    ------------------------------------
	SceneStageSaves.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "SceneBase.h"
#include "FileManager.h"

// =============== �萔 =====================
const std::string sPath = "Assets/Save/Scene/";
const std::string sFileName = "StageSave";
const std::string sExtension = ".stg";

// �t�@�C�����o�̓}�N��
#define FILE_OUTPUT(num)    FileManager::StageObjectOutput(sPath + sFileName + std::to_string(num) + sExtension)
#define FILE_INPUT(num)     FileManager::StageObjectInput(sPath + sFileName + std::to_string(num) + sExtension)


// =============== �N���X��` =====================
class SceneStageSave_1 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(1); };
    void UninitLocal() override { FILE_OUTPUT(1); };

	DEFINE_SCENE_TYPE(SceneStageSave_1)
};

class SceneStageSave_2 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(2); };
    void UninitLocal() override { FILE_OUTPUT(2); };

	DEFINE_SCENE_TYPE(SceneStageSave_2)
};

class SceneStageSave_3 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(3); };
    void UninitLocal() override { FILE_OUTPUT(3); };

	DEFINE_SCENE_TYPE(SceneStageSave_3)
};

class SceneStageSave_4 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(4); };
    void UninitLocal() override { FILE_OUTPUT(4); };

	DEFINE_SCENE_TYPE(SceneStageSave_4)
};

class SceneStageSave_5 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(5); };
    void UninitLocal() override { FILE_OUTPUT(5); };

	DEFINE_SCENE_TYPE(SceneStageSave_5)
};

class SceneStageSave_6 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(6); };
    void UninitLocal() override { FILE_OUTPUT(6); };

    DEFINE_SCENE_TYPE(SceneStageSave_6)
};

class SceneStageSave_7 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(7); };
    void UninitLocal() override { FILE_OUTPUT(7); };

    DEFINE_SCENE_TYPE(SceneStageSave_7)
};

class SceneStageSave_8 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(8); };
    void UninitLocal() override { FILE_OUTPUT(8); };

    DEFINE_SCENE_TYPE(SceneStageSave_8)
};

class SceneStageSave_9 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(9); };
    void UninitLocal() override { FILE_OUTPUT(9); };

    DEFINE_SCENE_TYPE(SceneStageSave_9)
};

class SceneStageSave_10 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(10); };
    void UninitLocal() override { FILE_OUTPUT(10); };

    DEFINE_SCENE_TYPE(SceneStageSave_10)
};