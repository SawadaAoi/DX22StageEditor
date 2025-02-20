/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト種類登録用cpp
	------------------------------------
	ObjectTypeRegistry.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectTypeRegistry.h"

/* ========================================
	インスタンス取得関数
	-------------------------------------
	内容：インスタンスを取得する
	-------------------------------------
	戻値：インスタンス
=========================================== */
ObjectTypeRegistry& ObjectTypeRegistry::GetInstance()
{
	static ObjectTypeRegistry instance;
	return instance;
}

/* ========================================
	オブジェクトクラス登録関数
	-------------------------------------
	内容：オブジェクトクラスをマップに登録する
	-------------------------------------
	引数1：オブジェクトクラス名
	引数2：オブジェクト生成関数
=========================================== */
void ObjectTypeRegistry::Register(const std::string& sClassName, CreateFunction* func)
{
	m_ObjectTypeMap[sClassName] = func;
}

/* ========================================
	オブジェクト生成関数
	-------------------------------------
	内容：クラス名文字列からオブジェクトを生成する
	-------------------------------------
	引数：オブジェクトクラス名
	-------------------------------------
	戻値：オブジェクト
=========================================== */
ObjectBase* ObjectTypeRegistry::CreateObject(const std::string& sClassName)
{
	// クラス名が登録されているか確認
	if (m_ObjectTypeMap.find(sClassName) != m_ObjectTypeMap.end())
	{
		return m_ObjectTypeMap.at(sClassName)();
	}
	return nullptr;
}

/* ========================================
	オブジェクト登録関数
	-------------------------------------
	内容：オブジェクトの登録を行う
=========================================== */
void ObjectTypeRegistry::RegisterCategory(const std::string& sClassName, ObjectCategoryType cateNum)
{
	m_ObjectCategoryMap[sClassName] = cateNum;
}


/* ========================================
	ゲッター(オブジェクトマップ)関数
	-------------------------------------
	戻値：unordered_map<std::string, CreateFunction*>& オブジェクトマップ
=========================================== */
std::unordered_map<std::string, ObjectTypeRegistry::CreateFunction*>& ObjectTypeRegistry::GetObjectTypeMap()
{
	return m_ObjectTypeMap;
}

/* ========================================
	ゲッター(カテゴリマップ)関数
	-------------------------------------
	戻値：unordered_map<std::string, ObjectCategoryType>& カテゴリマップ
=========================================== */
std::unordered_map<std::string, ObjectTypeRegistry::ObjectCategoryType>& ObjectTypeRegistry::GetObjectCategoryMap()
{
	return m_ObjectCategoryMap;
}
