/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト種類登録用cpp
	------------------------------------
	ObjectTypeRegistry.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectTypeRegistry.h"

// オブジェクト
#include "ObjectPlayer.h"
#include "ObjectGround.h"
#include "ObjectGroundGrass.h"
#include "ObjectBlock.h"
#include "ObjectCamera.h"
#include "ObjectLight.h"
#include "ObjectLightDirectional.h"
#include "ObjectLightPoint.h"
#include "ObjectLightSpot.h"

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
	全オブジェクト登録関数
	-------------------------------------
	内容：全てのオブジェクトを登録する
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	REGISTER_OBJECT_TYPE(ObjectBase);
	REGISTER_OBJECT_TYPE(ObjectPlayer);
	REGISTER_OBJECT_TYPE(ObjectGround);
	REGISTER_OBJECT_TYPE(ObjectGroundGrass);
	REGISTER_OBJECT_TYPE(ObjectBlock);
	REGISTER_OBJECT_TYPE(ObjectCamera);
	REGISTER_OBJECT_TYPE(ObjectLight);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional);
	REGISTER_OBJECT_TYPE(ObjectLightPoint);
	REGISTER_OBJECT_TYPE(ObjectLightSpot);
    
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
