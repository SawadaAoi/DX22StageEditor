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
#include "ObjectTypeRegistry.h"
#include "ComponentTransform.h"

// シーン
#include <type_traits>
#include "SceneStageSaves.h"

// =============== 定数定義 =======================
const std::string DEFAULT_SAVE_PATH = "Assets/Save/";	// デフォルトの保存先


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
		"ObjectTypeList",
		"TransformEdit",
	};

	// シーン名、シーン変更関数のマップ
	const std::map< std::string, std::function<void()>> SCENE_MAP =
	{
		{"SceneStageSave_1", []() { SceneManager::ChangeScene<SceneStageSave_1>(); } },
		{"SceneStageSave_2", []() { SceneManager::ChangeScene<SceneStageSave_2>(); } },
		{"SceneStageSave_3", []() { SceneManager::ChangeScene<SceneStageSave_3>(); } },
		{"SceneStageSave_4", []() { SceneManager::ChangeScene<SceneStageSave_4>(); } },
		{"SceneStageSave_5", []() { SceneManager::ChangeScene<SceneStageSave_5>(); } },
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
		ウィンドウ初期化(基本設定)関数
		-------------------------------------
		内容：基本設定の初期化を行う
	=========================================== */
	void InitBasicSettings()
	{
		// 背景色
		WIN_BASIC_SETTING.AddItem(Item::CreateValue("BackColor", Item::Color, true));
		// 当たり判定表示
		WIN_BASIC_SETTING.AddItem(Item::CreateValue("DispCollision", Item::Bool, true));

		// グリッド線
		// 値を変更したら線の初期化を行う関数
		Item::Callback FuncClickSave = [](bool isWrite, void* arg) {
			if (isWrite)
				GridLine::Init();
		};
		Item* pGridGroup = Item::CreateGroup("Grid");
		pGridGroup->AddGroupItem(Item::CreateCallBack("GridEnable", Item::Bool, FuncClickSave, true));	// グリッド線の表示
		pGridGroup->AddGroupItem(Item::CreateCallBack("AxisEnable", Item::Bool, FuncClickSave, true, true));	// 軸線の表示
		pGridGroup->AddGroupItem(Item::CreateCallBack("Size", Item::Float, FuncClickSave, true));	// マス数
		pGridGroup->AddGroupItem(Item::CreateCallBack("Margin", Item::Float, FuncClickSave, true));	// 線幅
		pGridGroup->AddGroupItem(Item::CreateCallBack("Color", Item::Color, FuncClickSave, true));	// 線色

		WIN_BASIC_SETTING.AddItem(pGridGroup);

		Item* pDrawGroup = Item::CreateGroup("Draw");
		// FPS
		pDrawGroup->AddGroupItem(Item::CreateValue("FPS", Item::Text, false));
		// ポーズ
		pDrawGroup->AddGroupItem(Item::CreateValue("Pause", Item::Bool, false, true));
		// リフレッシュレート
		pDrawGroup->AddGroupItem(Item::CreateValue("MaxFPS", Item::Float, false));

		WIN_BASIC_SETTING.AddItem(pDrawGroup);

		WIN_BASIC_SETTING["Draw"]["Pause"].SetBool(false);
		WIN_BASIC_SETTING["Draw"]["MaxFPS"].SetFloat(WindowConfig::REFRESH_RATE);

	}


	/* ========================================
		ウィンドウ初期化(カメラ情報)関数
		-------------------------------------
		内容：カメラ情報の初期化を行う
	=========================================== */
	void InitCameraInfo()
	{
		WIN_CAMERA_INFO.AddItem(Item::CreateValue("Active", Item::Kind::Text));	// アクティブカメラ名

		WIN_CAMERA_INFO.AddItem(Item::CreateValue("Orthographic", Item::Kind::Bool));	// 平行投影かどうか

		WIN_CAMERA_INFO.AddItem(Item::CreateList("CameraList", [](const void* arg)		// カメラリスト
		{
			// クリックされたカメラをアクティブにする
			std::string sCameraName = reinterpret_cast<const char*>(arg);				// リスト項目名
			int nCameraNum = WIN_CAMERA_INFO["CameraList"].GetListNo(sCameraName.c_str());	// カメラ番号を取得

			CAMERA_MNG_INST.SwitchCamera(nCameraNum);	// カメラ切り替え
		}, false, false, false, 3));

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
		}, true, true);

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
		// ファイル場所
		WIN_DATA_INOUT.AddItem(Item::CreateValue("SavePath", Item::Kind::Path));	
		// デフォルトファイル場所設定ボタン
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("Def", Item::Kind::Command, [](bool isWrite, void* arg)
		{
			WIN_DATA_INOUT["SavePath"].SetPath(DEFAULT_SAVE_PATH);
		}, false, true));

		// ファイル名
		WIN_DATA_INOUT.AddItem(Item::CreateValue("FileName", Item::Kind::Path));	

		// 拡張子リスト
		WIN_DATA_INOUT.AddItem(Item::CreateList("Extension", nullptr, false, true, true));
		WIN_DATA_INOUT["Extension"].AddListItem("");
		WIN_DATA_INOUT["Extension"].AddListItem(".stg");
		WIN_DATA_INOUT["Extension"].AddListItem(".txt");

		// ファイル出力
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("OutPut", Item::Kind::Command,[](bool isWrite, void* arg) 
		{
			std::string sPath		=  WIN_DATA_INOUT["SavePath"].GetPath();
			std::string sFileName	= WIN_DATA_INOUT["FileName"].GetPath();
			std::string sExtension	= WIN_DATA_INOUT["Extension"].GetListText(WIN_DATA_INOUT["Extension"].GetInt());
			FileManager::StageObjectOutput(sPath + "/" + sFileName + sExtension);
		}));

		// ファイル入力
		WIN_DATA_INOUT.AddItem(Item::CreateCallBack("InPut", Item::Kind::Command, [](bool isWrite, void* arg) 
		{
			std::string sPath		= WIN_DATA_INOUT["SavePath"].GetPath();
			std::string sFileName	= WIN_DATA_INOUT["FileName"].GetPath();
			std::string sExtension	= WIN_DATA_INOUT["Extension"].GetListText(WIN_DATA_INOUT["Extension"].GetInt());
			FileManager::StageObjectInput(sPath + "/" + sFileName + sExtension);
		}, false, true));

		// データ入出力
		WIN_DATA_INOUT.AddItem(Item::CreateValue("OutputCamera", Item::Kind::Bool, true, true));
		WIN_DATA_INOUT.AddItem(Item::CreateValue("OutputLight", Item::Kind::Bool, true, true));

		WIN_DATA_INOUT.AddItem(Item::CreateValue("InputResult ", Item::Kind::Text, false));	// 読込結果表示
		WIN_DATA_INOUT.AddItem(Item::CreateValue("OutputResult", Item::Kind::Text, false));	// 出力結果表示

		// 初期値
		WIN_DATA_INOUT["SavePath"].SetPath(DEFAULT_SAVE_PATH);
	}

	/* ========================================
		ウィンドウ初期化(オブジェクトタイプ一覧)関数
		-------------------------------------
		内容：オブジェクトタイプ一覧の初期化を行う
	=========================================== */
	void InitObjectTypeList()
	{
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateValue("CreatePos", Item::Kind::Vector));

		// リストを選択したときの実行処理
		DebugUI::Item::ConstCallback Clickfunc = [](const void* arg) {ClickObjectTypeList(arg); };

		// 各オブジェクト種類のグループ作成(見やすさの為)
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Empty"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Player"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Enemy"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Terrain"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Camera"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Light"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("System"));
		WIN_OBJ_TYPE_LIST.AddItem(Item::CreateGroup("Other"));

		// グループにリストを追加(各グループに1リスト。最後の数字は表示行数)
		WIN_OBJ_TYPE_LIST["Empty"].AddGroupItem(Item::CreateList("EmptyList",		Clickfunc, false, false, false, 1));
		WIN_OBJ_TYPE_LIST["Player"].AddGroupItem(Item::CreateList("PlayerList",		Clickfunc, false, false, false, 3));
		WIN_OBJ_TYPE_LIST["Enemy"].AddGroupItem(Item::CreateList("EnemyList",		Clickfunc, false, false, false, 5));
		WIN_OBJ_TYPE_LIST["Terrain"].AddGroupItem(Item::CreateList("TerrainList",	Clickfunc, false, false, false, 5));
		WIN_OBJ_TYPE_LIST["Camera"].AddGroupItem(Item::CreateList("CameraList",		Clickfunc, false, false, false, 3));
		WIN_OBJ_TYPE_LIST["Light"].AddGroupItem(Item::CreateList("LightList",		Clickfunc, false, false, false, 5));
		WIN_OBJ_TYPE_LIST["System"].AddGroupItem(Item::CreateList("SystemList",		Clickfunc, false, false, false, 1));
		WIN_OBJ_TYPE_LIST["Other"].AddGroupItem(Item::CreateList("OtherList",		Clickfunc, false, false, false, 3));

		// リストに中身を追加
		CreateObjectTypeList();
	}

	/* ========================================
		ウィンドウ初期化(オブジェクト変形エディタ)関数
		-------------------------------------
		内容：オブジェクト変形エディタの初期化を行う
	=========================================== */
	void InitTransformEdit()
	{
		// 編集モード表示
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("Mode", Item::Kind::Text, false));	


		// 編集モードOffボタン
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("EditOff", Item::Kind::Command,	[](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(0); }, false, true));

		// リセットボタン
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Reset", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->ResetTransformEdit(); }, false, true));

		// 編集モード切り替え(移動)
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Position", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(1);}));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValuePos", Item::Kind::Float, false, true));
		// 編集モード切り替え(回転)
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Rotation", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(2); }));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValueRot", Item::Kind::Float, false, true));
		// 編集モード切り替え(拡縮)
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateCallBack("Scale   ", Item::Kind::Command, [](bool isWrite, void* arg) {SceneManager::GetScene()->SetTransformEditMode(3); }));
		WIN_TRANSFORM_EDIT.AddItem(Item::CreateValue("ValueScale", Item::Kind::Float, false, true));


		// 初期値
		WIN_TRANSFORM_EDIT["ValuePos"].SetFloat(1.0f);
		WIN_TRANSFORM_EDIT["ValueRot"].SetFloat(10.0f);
		WIN_TRANSFORM_EDIT["ValueScale"].SetFloat(1.0f);
	}

	/* ========================================
		オブジェクトタイプ一覧クリック関数
		-------------------------------------
		内容：オブジェクトタイプ一覧のクリック処理
		-------------------------------------
		引数：arg	選択した項目の表示内容
	=========================================== */
	void ClickObjectTypeList(const void* arg)
	{
		// 選択したらそのタイプのオブジェクトを生成
		std::string sObjTypeName = reinterpret_cast<const char*>(arg);	// リスト項目名

		// オブジェクト生成
		ObjectBase* pObject = OBJ_TYPE_REGISTRY_INST.CreateObject(sObjTypeName);
		if (pObject)
		{
			SceneBase* pScene = SceneManager::GetScene();			// シーン取得
			pObject->Init(pScene->CreateUniqueName(sObjTypeName));	// オブジェクト初期化(名前重複避ける)

			Vector3<float> vPos;
			vPos = WIN_OBJ_TYPE_LIST["CreatePos"].GetVector();
			pObject->GetTransform()->SetLocalPosition(vPos);

			pScene->AddSceneObjectBase(pObject);					// シーンに追加
		}
	}

	/* ========================================
		オブジェクトタイプリスト作成関数
		-------------------------------------
		内容：オブジェクトタイプリストを作成する
			　種類別分グループを作成する
	=========================================== */
	void CreateObjectTypeList()
	{
		// オブジェクトクラスマップ取得
		std::unordered_map<std::string, ObjectTypeRegistry::CreateFunction*>	objectTypeMap		= OBJ_TYPE_REGISTRY_INST.GetObjectTypeMap();
		// オブジェクトカテゴリマップ取得
		std::unordered_map<std::string, ObjectTypeRegistry::ObjectCategoryType> objectCategoryMap	= OBJ_TYPE_REGISTRY_INST.GetObjectCategoryMap();
		
		// オブジェクトクラス一覧
		for (auto& objectType : objectTypeMap)
		{
			// カテゴリ取得
			ObjectTypeRegistry::ObjectCategoryType category = objectCategoryMap.at(objectType.first);

			// 空
			if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_EMPTY)
				WIN_OBJ_TYPE_LIST["Empty"]["EmptyList"].AddListItem(objectType.first.c_str());
			// プレイヤー
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_PLAYER)
				WIN_OBJ_TYPE_LIST["Player"]["PlayerList"].AddListItem(objectType.first.c_str());
			// 敵
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_ENEMY)
				WIN_OBJ_TYPE_LIST["Enemy"]["EnemyList"].AddListItem(objectType.first.c_str());
			// 地形
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_TERRAIN)
				WIN_OBJ_TYPE_LIST["Terrain"]["TerrainList"].AddListItem(objectType.first.c_str());
			// カメラ
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_CAMERA)
				WIN_OBJ_TYPE_LIST["Camera"]["CameraList"].AddListItem(objectType.first.c_str());
			// ライト
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_LIGHT)
				WIN_OBJ_TYPE_LIST["Light"]["LightList"].AddListItem(objectType.first.c_str());
			// システム
			else if (category == ObjectTypeRegistry::ObjectCategoryType::OCT_SYSTEM)
				WIN_OBJ_TYPE_LIST["System"]["SystemList"].AddListItem(objectType.first.c_str());
			// その他
			else
				WIN_OBJ_TYPE_LIST["Other"]["OtherList"].AddListItem(objectType.first.c_str());

		}
		
	}

}

#endif // _DEBUG