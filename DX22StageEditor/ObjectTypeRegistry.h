/* ========================================
    DX22Base/
    ------------------------------------
    オブジェクト種類登録用ヘッダ
    ------------------------------------
	説明：文字列からオブジェクトを生成するための登録クラス
    ------------------------------------
    ObjectTypeRegistry.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"
#include "SceneManager.h"
#include <unordered_map>

// =============== 定数定義 =======================
#define OBJ_TYPE_REGISTRY_INST ObjectTypeRegistry::GetInstance()    // インスタンス取得用マクロ
#define REGISTER_OBJECT_TYPE(className, cateNum)									\
    ObjectTypeRegistry::GetInstance().Register(#className,                          \
        []() -> ObjectBase* { return new className(SceneManager::GetScene()); });   \
	ObjectTypeRegistry::GetInstance().RegisterCategory(#className, cateNum);		\

// =============== クラス定義 ===================
class ObjectTypeRegistry
{
public:
	using CreateFunction = ObjectBase*(); // 関数ポインタの型を定義

    enum ObjectCategoryType
	{
		OCT_EMPTY,
		OCT_PLAYER,
		OCT_ENEMY,
		OCT_TERRAIN,
		OCT_CAMERA,
		OCT_LIGHT,
		OCT_SYSTEM,
		OCT_OTHER,
	};;

public:
    static ObjectTypeRegistry& GetInstance();
	void Register(const std::string& sClassName, CreateFunction* func); // マップ(文字列・クラス生成関数)に登録
	ObjectBase* CreateObject(const std::string& sClassName);            // オブジェクト生成

	static void RegisterAllObjectTypes();                               // オブジェクトの登録を行う関数

	void RegisterCategory(const std::string& sClassName, ObjectCategoryType cateNum); // カテゴリマップに登録

    // ゲッター
    std::unordered_map<std::string, CreateFunction*>& GetObjectTypeMap();
	std::unordered_map<std::string, ObjectCategoryType>& GetObjectCategoryMap();

private:
	// 文字列とクラス生成関数のマップ
    std::unordered_map<std::string, CreateFunction*> m_ObjectTypeMap; 
	// 一覧で種別表示するためのマップ
	std::unordered_map<std::string, ObjectCategoryType> m_ObjectCategoryMap;
};

