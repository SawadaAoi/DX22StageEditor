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

// 敵キャラ
#include "ObjectEnemy.h"
#include "ObjectEnemyLinear.h"
#include "ObjectEnemyChase.h"
#include "ObjectEnemyRandom.h"

// 地形
#include "ObjectGround.h"
#include "ObjectBlock.h"
#include "ObjectBlockGroundAABB.h"
#include "ObjectBlockGroundOBB.h"
#include "ObjectWall.h"

// ステージギミック
#include "ObjectMoveFloor.h"
#include "ObjectBlockBreakable.h"

// アイテム
#include "ObjectCoin.h"
#include "ObjectCoinGroupCircle.h"
#include "ObjectCoinGroupLinear.h"

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

// その他
#include "ObjectGoal.h"
#include "ObjectSkyBox.h"
#include "ObjectRespawn.h"


/* ========================================
	全オブジェクト登録関数
	-------------------------------------
	内容：全てのオブジェクトを登録する
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	// 全オブジェクトの基底クラス
	REGISTER_OBJECT_TYPE(ObjectBase, OCT_EMPTY);

	// プレイヤー
	REGISTER_OBJECT_TYPE(ObjectPlayer, OCT_PLAYER);
	REGISTER_OBJECT_TYPE(ObjectPlayerStart, OCT_PLAYER);

	// 敵キャラ
	REGISTER_OBJECT_TYPE(ObjectEnemy, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyLinear, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyChase, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyRandom, OCT_ENEMY);


	// 地形
	REGISTER_OBJECT_TYPE(ObjectGround, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlock, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundAABB, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundOBB, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectWall, OCT_TERRAIN);

	// ステージギミック
	REGISTER_OBJECT_TYPE(ObjectMoveFloor, OCT_GIMMICK);
	REGISTER_OBJECT_TYPE(ObjectBlockBreakable, OCT_GIMMICK);

	// アイテム
	REGISTER_OBJECT_TYPE(ObjectCoin, OCT_ITEM);
	REGISTER_OBJECT_TYPE(ObjectCoinGroupCircle, OCT_ITEM);
	REGISTER_OBJECT_TYPE(ObjectCoinGroupLinear, OCT_ITEM);

	// カメラ
	REGISTER_OBJECT_TYPE(ObjectCamera, OCT_CAMERA);
	REGISTER_OBJECT_TYPE(ObjectCameraDebug, OCT_CAMERA);
	REGISTER_OBJECT_TYPE(ObjectCameraPlayer, OCT_CAMERA);

	// ライト
	REGISTER_OBJECT_TYPE(ObjectLight, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightPoint, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightSpot, OCT_LIGHT);

	// システム
	REGISTER_OBJECT_TYPE(ObjectGameStateManager, OCT_SYSTEM);

	// その他
	REGISTER_OBJECT_TYPE(ObjectSkyBox, OCT_OTHER);
	REGISTER_OBJECT_TYPE(ObjectGoal, OCT_OTHER);
	REGISTER_OBJECT_TYPE(ObjectRespawn, OCT_OTHER);


}

