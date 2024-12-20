/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクトタイプ登録用cpp
	------------------------------------
	RegisterObjectTypeMap.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectTypeRegistry.h"

// オブジェクト
#include "ObjectPlayer.h"
#include "ObjectGround.h"
#include "ObjectBlock.h"

// カメラ
#include "ObjectCamera.h"
#include "ObjectCameraDebug.h"
#include "ObjectCameraPlayer.h"

// ライト
#include "ObjectLight.h"
#include "ObjectLightDirectional.h"
#include "ObjectLightPoint.h"
#include "ObjectLightSpot.h"


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

	REGISTER_OBJECT_TYPE(ObjectBlock);

	REGISTER_OBJECT_TYPE(ObjectCamera);
	REGISTER_OBJECT_TYPE(ObjectCameraDebug);
	REGISTER_OBJECT_TYPE(ObjectCameraPlayer);

	REGISTER_OBJECT_TYPE(ObjectLight);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional);
	REGISTER_OBJECT_TYPE(ObjectLightPoint);
	REGISTER_OBJECT_TYPE(ObjectLightSpot);

}