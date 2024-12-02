/* ========================================
	DX22Base/
	------------------------------------
	デバッグウィンドウ設定用ヘッダ
	------------------------------------
	説明：各デバッグウィンドウの初期化を管理する
	------------------------------------
	DebugWindowSetting.h
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効
#pragma once

// =============== インクルード ===================
#include <string>

// =============== 定数定義 =======================
// ウィンドウ呼び出し用マクロ
#define WIN_BASIC_SETTING	DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::BASIC_SETTINGS])
#define WIN_CAMERA_INFO		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::CAMERA_INFO])
#define WIN_SCENE_LIST		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::SCENE_LIST])
#define WIN_OBJ_LIST		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_LIST])
#define WIN_OBJ_INFO		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_INFO])
#define WIN_DATA_INOUT		DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::DATA_INOUT])
#define WIN_OBJ_TYPE_LIST	DebugUI::Menu::Get(DebugUI::WIN_NAME[DebugUI::E_WinType::OBJECT_TYPE_LIST])

// アイテム呼び出し用マクロ
const std::string ITEM_OBJ_LIST_NAME = "Objects";
#define ITEM_OBJ_LIST	WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()]


// =============== 名前空間 ===================
namespace DebugUI
{
	const std::string CHILD_HEAD_TEXT = "L";	// 子要素の先頭に表示する文字
	const std::string CHILD_HEAD_SPACE = " ";	// 子要素の先頭に表示するスペース


	// ウィンドウ種別
	enum E_WinType
	{	
		MAIN_WINDOW,		// メインウィンドウ
		BASIC_SETTINGS,		// 基本設定
		CAMERA_INFO,
		SCENE_LIST,			// シーン一覧
		OBJECT_LIST,		// オブジェクト一覧
		OBJECT_INFO,		// オブジェクト情報
		DATA_INOUT,			// データ入出力
		OBJECT_TYPE_LIST,	// オブジェクトタイプ一覧
		MAX,
	};

	// ウィンドウ名
	extern const char* WIN_NAME[E_WinType::MAX];

	// 各メニューの初期化
	void InitDebugWindow();
	void InitBasicSettings();
	void InitCameraInfo();
	void InitSceneList();
	void InitDataInOut();
	void InitObjectTypeList();

}

#endif // _DEBUG