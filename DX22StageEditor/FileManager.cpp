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

#include "ComponentTransform.h"
#include "ObjectTypeRegistry.h"

#include <Windows.h>	// メッセージボックス用


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

	// シーンに存在するオブジェクトを取得
	for (auto& object : SceneManager::GetScene()->GetAllSceneObjects())
	{
		S_SaveDataObject data;

		// オブジェクトの情報を取得 ---------------------------------
		// クラス名
		strncpy(data.cClassType, object->GetObjClassName().c_str(), sizeof(data.cClassType) - 1);
		data.cClassType[sizeof(data.cClassType) - 1] = '\0';

		// 位置、回転、拡大
		ComponentTransform* pTransform = object->GetComponent<ComponentTransform>();
		data.vPos = pTransform->GetLocalPosition();
		data.qRot = pTransform->GetLocalRotation();
		data.vScale = pTransform->GetLocalScale();

		// オブジェクト名
		strncpy(data.cObjectName, object->GetName().c_str(), sizeof(data.cObjectName) - 1);
		data.cObjectName[sizeof(data.cObjectName) - 1] = '\0';

		// 親オブジェクト名
		ObjectBase* pParent = object->GetParentObject();
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
			
			// 位置、回転、拡大の設定
			ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
			pTransform->SetLocalPosition(data.vPos);
			pTransform->SetLocalRotation(data.qRot);
			pTransform->SetLocalScale(data.vScale);

			// シーンに追加
			pScene->AddSceneObjectBase(pObject);
		}
 	}

	// 親子関係の設定 -----------------------------------------------

	// ファイルの位置を先頭に戻す
	// ファイルのオブジェクトを全て登録してから親子関係を設定するため
	file.clear();
	file.seekg(0, std::ios::beg);

	// ファイルの終端まで読み込む
	while (!file.eof())
	{
		S_SaveDataObject data;
		file.read((char*)&data, sizeof(S_SaveDataObject));

		if (file.eof())
		{
			break;
		}

		// 親子関係の設定
		std::string sObjectName = data.cObjectName;
		std::string sParentName = data.cParentName;

		// 親オブジェクトがいない場合はスキップ
		if (sParentName.empty())	continue;

		// オブジェクトと親オブジェクトを取得
		ObjectBase* pObject = pScene->FindSceneObject(sObjectName);
		ObjectBase* pParent = pScene->FindSceneObject(sParentName);

		// どちらも存在していたら親子関係を設定
		if (pObject && pParent)
		{
			pObject->SetParentObject(pParent);
		}
	}

	// メッセージ表示(成功)
	WIN_DATA_INOUT["InputResult "].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();

}
