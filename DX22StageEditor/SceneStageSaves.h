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

};

class SceneStageSave_2 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(2); };
    void UninitLocal() override { FILE_OUTPUT(2); };

};

class SceneStageSave_3 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(3); };
    void UninitLocal() override { FILE_OUTPUT(3); };

};

class SceneStageSave_4 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(4); };
    void UninitLocal() override { FILE_OUTPUT(4); };

};

class SceneStageSave_5 :
    public SceneBase
{
public:
    void InitLocal() override { FILE_INPUT(5); };
    void UninitLocal() override { FILE_OUTPUT(5); };

};