/* ========================================
	DX22Base/
	------------------------------------
	ファイル処理管理用cpp
	------------------------------------
	FileManager.cpp
========================================== */

// =============== インクルード ===================
#include "FileManager.h"
// ファイル処理用
#include <iostream>		
#include <fstream>		

#include "SceneManager.h"
#include "SceneBase.h"
#include "ObjectBase.h"

#include "ComponentTransform.h"
#include "ObjectTypeRegistry.h"

#include <Windows.h>		// メッセージボックス用
#include <unordered_map>


/* ========================================
	ファイル出力(シーンオブジェクト)関数
	-------------------------------------
	内容：オブジェクトの情報をファイルに出力する
	-------------------------------------
	引数1：ファイルパス
========================================== */
void FileManager::StageObjectOutput(std::string sPath)
{
	// ファイルを開く
	std::ofstream file(sPath, std::ios::out | std::ios::binary);

	// ファイルが開けなかったら終了
	if (!file.is_open())
	{
		// メッセージ表示(失敗)
		WIN_DATA_INOUT["OutputResult"].SetText("< Failed > " + sPath.substr(sPath.find_last_of('/') + 1));

		return;
	}

	// データ入出力時のオプション
	bool bCamera	= WIN_DATA_INOUT["Camera"].GetBool();			// カメラの出力設定
	bool bLight		= WIN_DATA_INOUT["Light"].GetBool();			// ライトの出力設定
	bool bTerrain	= WIN_DATA_INOUT["Terrain"].GetBool();			// 地形の出力設定
	bool bEnemy		= WIN_DATA_INOUT["Enemy"].GetBool();			// 敵キャラの出力設定
	bool bTransOnly = WIN_DATA_INOUT["TransformOnly"].GetBool();	// 位置、回転、拡大のみの出力設定

	// シーンに存在するオブジェクトを取得
	for (auto& pObject : SceneManager::GetScene()->GetAllSceneObjects())
	{
		// 除外チェック
		if (pObject->GetTag() == E_ObjectTag::Camera && !bCamera) continue;
		if (pObject->GetTag() == E_ObjectTag::Light && !bLight) continue;
		if (pObject->GetTag() == E_ObjectTag::Ground && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Goal && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Wall && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Enemy && !bEnemy) continue;

		if (!pObject->GetIsSave()) continue;	// 保存フラグが立っていない場合はスキップ

		// オブジェクトの情報を格納する構造体
		S_SaveDataObject data;

		// オブジェクトの情報を取得 ---------------------------------
		// クラス名
		strncpy(data.cClassType, pObject->GetObjClassName().c_str(), sizeof(data.cClassType) - 1);
		data.cClassType[sizeof(data.cClassType) - 1] = '\0';

		// 位置、回転、拡大
		ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
		data.vPos	= pTransform->GetPosition();
		data.qRot	= pTransform->GetRotation();
		data.vScale = pTransform->GetScale();

		// オブジェクト名
		strncpy(data.cObjectName, pObject->GetName().c_str(), sizeof(data.cObjectName) - 1);
		data.cObjectName[sizeof(data.cObjectName) - 1] = '\0';

		// 親オブジェクト名
		ObjectBase* pParent = pObject->GetParentObject();
		if (pParent)
		{
			strncpy(data.cParentName, pParent->GetName().c_str(), sizeof(data.cParentName) - 1);
			data.cParentName[sizeof(data.cParentName) - 1] = '\0';
		}
		else
		{
			data.cParentName[0] = '\0';	// 親オブジェクトがいない場合は空文字
		}

		// ファイルに書き込み
		file.write((char*)&data, sizeof(S_SaveDataObject));

		// 位置、回転、拡大のみの出力の場合はスキップ
		if (bTransOnly) continue;	
		// オブジェクト個別のデータ出力
		pObject->OutPutLocalData(file);
	}

	// メッセージ表示(成功)
	WIN_DATA_INOUT["OutputResult"].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();
}

/* ========================================
	ファイル入力(シーンオブジェクト)関数
	-------------------------------------
	内容：ファイルからオブジェクトの情報を読み込む
	-------------------------------------
	引数1：ファイルパス
========================================== */
void FileManager::StageObjectInput(std::string sPath)
{
	// 親オブジェクトのマップ
	std::unordered_map<ObjectBase*, std::string> mapObjectParent;

	// ファイルを開く
	std::ifstream file(sPath, std::ios::in | std::ios::binary);

	// ファイルが開けなかったら終了
	if (!file.is_open())
	{
		// メッセージ表示(失敗)
		WIN_DATA_INOUT["InputResult "].SetText("< Failed > " + sPath.substr(sPath.find_last_of('/') + 1));
		return;
	}

	SceneBase* pScene = SceneManager::GetScene();	// シーン取得

	// データ入出力時のオプション
	bool bCamera	= WIN_DATA_INOUT["Camera"].GetBool();			// カメラの出力設定
	bool bLight		= WIN_DATA_INOUT["Light"].GetBool();			// ライトの出力設定
	bool bTerrain	= WIN_DATA_INOUT["Terrain"].GetBool();			// 地形の出力設定
	bool bEnemy		= WIN_DATA_INOUT["Enemy"].GetBool();			// 敵キャラの出力設定
	bool bTransOnly = WIN_DATA_INOUT["TransformOnly"].GetBool();	// 位置、回転、拡大のみの出力設定

	// ファイルの終端まで読み込む
	while (!file.eof())
	{
		S_SaveDataObject data;
		file.read((char*)&data, sizeof(S_SaveDataObject));

		// ファイルの終端に到達したら終了
		if (file.eof())
		{
			break;
		}

		// オブジェクトの生成(渡したクラス名から生成)
		ObjectBase* pObject = OBJ_TYPE_REGISTRY_INST.CreateObject(data.cClassType);

		if (pObject)
		{
			pObject->Init(pScene->CreateUniqueName(data.cObjectName));	// オブジェクト初期化(名前重複避ける)

			// 除外チェック
			if (pObject->GetTag() == E_ObjectTag::Camera && !bCamera) continue;
			if (pObject->GetTag() == E_ObjectTag::Light && !bLight) continue;
			if (pObject->GetTag() == E_ObjectTag::Ground && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Goal && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Wall && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Enemy && !bEnemy) continue;

			// 位置、回転、拡大の設定
			ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
			pTransform->SetLocalPosition(data.vPos);
			pTransform->SetLocalRotation(data.qRot);
			pTransform->SetLocalScale(data.vScale);

			// シーンに追加
			pScene->AddSceneObjectBase(pObject);
			// 親オブジェクトマップに追加
			mapObjectParent[pObject] = data.cParentName;

			if (bTransOnly) continue;	// 位置、回転、拡大のみの出力の場合はスキップ
			// オブジェクト個別のデータ入力
			pObject->InputLocalData(file);
		}
 	}

	// 親子関係の設定
	for (auto& object : mapObjectParent)
	{
		std::string sParentName = object.second;
		// 親オブジェクトがいない場合はスキップ
		if (sParentName.empty()) continue;
		// 親オブジェクト取得
		ObjectBase* pParent = pScene->FindSceneObject(sParentName);

		// 親オブジェクトが存在していたら親子関係を設定
		if (pParent)
		{
			object.first->SetParentObject(pParent);
		}
	}

	// メッセージ表示(成功)
	WIN_DATA_INOUT["InputResult "].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();

}
