/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクトタイプ登録用cpp
	------------------------------------
	RegisterObjectTypeMap.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectTypeRegistry.h"

// プレイヤー
#include "ObjectPlayer.h"
#include "ObjectPlayerStart.h"

// その他
#include "ObjectGoal.h"
#include "ObjectSkyBox.h"

// 地形
#include "ObjectGround.h"
#include "ObjectBlock.h"
#include "ObjectBlockGroundAABB.h"
#include "ObjectWall.h"

// 敵キャラ
#include "ObjectEnemy.h"
#include "ObjectEnemyLinear.h"
#include "ObjectEnemyChase.h"
#include "ObjectEnemyRandom.h"

// カメラ
#include "ObjectCamera.h"
#include "ObjectCameraDebug.h"
#include "ObjectCameraPlayer.h"

// ライト
#include "ObjectLight.h"
#include "ObjectLightDirectional.h"
#include "ObjectLightPoint.h"
#include "ObjectLightSpot.h"

// システム
#include "ObjectGameStateManager.h"


/* ========================================
	全オブジェクト登録関数
	-------------------------------------
	内容：全てのオブジェクトを登録する
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	// 全オブジェクトの基底クラス
	REGISTER_OBJECT_TYPE(ObjectBase);

	// 地形
	REGISTER_OBJECT_TYPE(ObjectGround);
	REGISTER_OBJECT_TYPE(ObjectBlock);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundAABB);
	REGISTER_OBJECT_TYPE(ObjectWall);

	// プレイヤー
	REGISTER_OBJECT_TYPE(ObjectPlayer);
	REGISTER_OBJECT_TYPE(ObjectPlayerStart);

	// その他
	REGISTER_OBJECT_TYPE(ObjectSkyBox);
	REGISTER_OBJECT_TYPE(ObjectGoal);

	// 敵キャラ
	REGISTER_OBJECT_TYPE(ObjectEnemy);
	REGISTER_OBJECT_TYPE(ObjectEnemyLinear);
	REGISTER_OBJECT_TYPE(ObjectEnemyChase);
	REGISTER_OBJECT_TYPE(ObjectEnemyRandom);

	// カメラ
	REGISTER_OBJECT_TYPE(ObjectCamera);
	REGISTER_OBJECT_TYPE(ObjectCameraDebug);
	REGISTER_OBJECT_TYPE(ObjectCameraPlayer);

	// ライト
	REGISTER_OBJECT_TYPE(ObjectLight);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional);
	REGISTER_OBJECT_TYPE(ObjectLightPoint);
	REGISTER_OBJECT_TYPE(ObjectLightSpot);

	// システム
	REGISTER_OBJECT_TYPE(ObjectGameStateManager);


}