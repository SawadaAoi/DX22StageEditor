/* ========================================
	DX22Base/
	------------------------------------
	デバッグウィンドウ設定用cpp
	------------------------------------
	DebugWindowSetting.cpp
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効

// =============== インクルード ===================
#include "DebugWindowSetting.h"
#include "DebugMenu.h"
#include "GridLine.h"	// グリッド線再読み込み
#include "SettingWindowAPI.h"
#include "SceneManager.h"
#include "CameraManager.h"	// カメラマネージャ
#include "FileManager.h"
// シーン
#include <type_traits>
#include "SceneGameTest.h"


// =============== 名前空間 ===================
namespace DebugUI
{

	// ウィンドウ名(const char*で定義しているため、宣言と定義を分けている)
	// stringではなくconst char*なのは、DebugMenuの多くの関数で文字列をconst char*で扱っているため
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

	// シーン名、シーン変更関数のマップ
	const std::map< std::string, std::function<void()>> SCENE_MAP =
	{
		{"SceneGameTest", []() { SceneManager::ChangeScene<SceneGameTest>(); } },
	};

	/* ========================================
		デバッグ用ウィンドウ初期化関数
		-------------------------------------
		内容：各ウィンドウの初期化を行う
	=========================================== */
	void InitDebugWindow()
	{
		// 各ウィンドウ生成
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
		ウィンドウ初期化(基本設定)関数
		-------------------------------------
		内容：基本設定の初期化を行う
	=========================================== */
	void InitBasicSettings()
	{
		// 背景色
		Item* pBackColor = Item::CreateValue("BackColor", Item::Color, true);

		// グリッド線
		// 値を変更したら線の初期化を行う関数
		Item::Callback FuncClickSave = [](bool isWrite, void* arg) {
			if (isWrite)
				GridLine::Init();
		};
		Item* pGrid = Item::CreateGroup("Grid");
		pGrid->AddGroupItem(Item::CreateCallBack("Enable", Item::Bool, FuncClickSave, true));	// グリッド線の表示
		pGrid->AddGroupItem(Item::CreateCallBack("Size", Item::Float, FuncClickSave, true));	// マス数
		pGrid->AddGroupItem(Item::CreateCallBack("Margin", Item::Float, FuncClickSave, true));	// 線幅
		pGrid->AddGroupItem(Item::CreateCallBack("Color", Item::Color, FuncClickSave, true));	// 線色
		pGrid->AddGroupItem(Item::CreateCallBack("Axis", Item::Bool, FuncClickSave, true));	// 軸線の表示

		// ポーズ
		Item* pPause = Item::CreateValue("Pause", Item::Bool, false);
		pPause->SetBool(false);

		// リフレッシュレート
		Item* pRefreshRate = Item::CreateValue("RefreshRate", Item::Float, false);
		pRefreshRate->SetFloat(WindowConfig::REFRESH_RATE);
		// FPS
		Item* pFPS = Item::CreateValue("FPS", Item::Text, false);

		// 項目一覧
		Item* Items[] =
		{
			pBackColor,
			pGrid,
			pPause,
			pRefreshRate,
			pFPS,
		};

		// ウィンドウに追加
		for (int i = 0; i < 5; ++i)
		{
			WIN_BASIC_SETTING.AddItem(Items[i]);
		}

	}

	/* ========================================
		ウィンドウ初期化(カメラ情報)関数
		-------------------------------------
		内容：カメラ情報の初期化を行う
	=========================================== */
	void InitCameraInfo()
	{
		WIN_CAMERA_INFO.AddItem(Item::CreateValue("ActiveCamera", Item::Kind::Text));	// アクティブカメラ名

		WIN_CAMERA_INFO.AddItem(Item::CreateValue("IsOrthographic", Item::Kind::Bool));	// 平行投影かどうか

		WIN_CAMERA_INFO.AddItem(Item::CreateList("CameraList", [](const void* arg)		// カメラリスト
		{
			// クリックされたカメラをアクティブにする
			std::string sCameraName = reinterpret_cast<const char*>(arg);				// リスト項目名
			int nCameraNum = WIN_CAMERA_INFO["CameraList"].GetListNo(sCameraName.c_str());	// カメラ番号を取得

			CAMERA_MNG_INST.SwitchCamera(nCameraNum);	// カメラ切り替え
		}));

	}

	/* ========================================
		ウィンドウ初期化(シーン一覧)関数
		-------------------------------------
		内容：シーン一覧の初期化を行う
	=========================================== */
	void InitSceneList()
	{
		// シーン一覧
		Item* pSceneList = Item::CreateList("SceneLists", [](const void* arg)
		{
			// 選択したらシーン変更
			std::string sSceneName = reinterpret_cast<const char*>(arg);	// リスト項目名
			SCENE_MAP.at(sSceneName)();	// シーン変更
		}, false);

		// シーン名をリストに追加
		for (auto& scene : SCENE_MAP)
		{
			pSceneList->AddListItem(scene.first.c_str());
		}

		WIN_SCENE_LIST.AddItem(pSceneList);
	}

	/* ========================================
		ウィンドウ初期化(データ入出力)関数
		-------------------------------------
		内容：データ入出力の初期化を行う
	=========================================== */
	void InitDataInOut()
	{
		WIN_DATA_INOUT.AddItem(Item::CreateValue("DataPath", Item::Kind::Path));	// データパス
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