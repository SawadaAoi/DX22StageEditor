/* ========================================
    DX22Base/
    ------------------------------------
    ステージデータ保存シーン用ヘッダ
    ------------------------------------
	説明：編集中のステージデータを保存するシーン
		　を複数用意する
		  シーン初期化時に読み込み、終了時に保存
    ------------------------------------
	SceneStageSaves.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "SceneBase.h"
#include "FileManager.h"

// =============== 定数 =====================
const std::string sPath = "Assets/Save/Scene/";
const std::string sFileName = "StageSave";
const std::string sExtension = ".stg";

// ファイル入出力マクロ
#define FILE_OUTPUT(num)    FileManager::StageObjectOutput(sPath + sFileName + std::to_string(num) + sExtension)
#define FILE_INPUT(num)     FileManager::StageObjectInput(sPath + sFileName + std::to_string(num) + sExtension)


// =============== クラス定義 =====================
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