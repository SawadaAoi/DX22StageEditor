/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g�^�C�v�o�^�pcpp
	------------------------------------
	RegisterObjectTypeMap.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectTypeRegistry.h"

// �v���C���[
#include "ObjectPlayer.h"
#include "ObjectPlayerStart.h"

// �G�L����
#include "ObjectEnemy.h"
#include "ObjectEnemyLinear.h"
#include "ObjectEnemyChase.h"
#include "ObjectEnemyRandom.h"

// �n�`
#include "ObjectGround.h"
#include "ObjectBlock.h"
#include "ObjectBlockGroundAABB.h"
#include "ObjectBlockGroundOBB.h"
#include "ObjectWall.h"

// �X�e�[�W�M�~�b�N
#include "ObjectMoveFloor.h"
#include "ObjectBlockBreakable.h"

// �A�C�e��
#include "ObjectCoin.h"
#include "ObjectCoinGroupCircle.h"
#include "ObjectCoinGroupLinear.h"

// �J����
#include "ObjectCamera.h"
#include "ObjectCameraDebug.h"
#include "ObjectCameraPlayer.h"

// ���C�g
#include "ObjectLight.h"
#include "ObjectLightDirectional.h"
#include "ObjectLightPoint.h"
#include "ObjectLightSpot.h"

// �V�X�e��
#include "ObjectGameStateManager.h"

// ���̑�
#include "ObjectGoal.h"
#include "ObjectSkyBox.h"
#include "ObjectRespawn.h"


/* ========================================
	�S�I�u�W�F�N�g�o�^�֐�
	-------------------------------------
	���e�F�S�ẴI�u�W�F�N�g��o�^����
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	// �S�I�u�W�F�N�g�̊��N���X
	REGISTER_OBJECT_TYPE(ObjectBase, OCT_EMPTY);

	// �v���C���[
	REGISTER_OBJECT_TYPE(ObjectPlayer, OCT_PLAYER);
	REGISTER_OBJECT_TYPE(ObjectPlayerStart, OCT_PLAYER);

	// �G�L����
	REGISTER_OBJECT_TYPE(ObjectEnemy, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyLinear, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyChase, OCT_ENEMY);
	REGISTER_OBJECT_TYPE(ObjectEnemyRandom, OCT_ENEMY);


	// �n�`
	REGISTER_OBJECT_TYPE(ObjectGround, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlock, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundAABB, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundOBB, OCT_TERRAIN);
	REGISTER_OBJECT_TYPE(ObjectWall, OCT_TERRAIN);

	// �X�e�[�W�M�~�b�N
	REGISTER_OBJECT_TYPE(ObjectMoveFloor, OCT_GIMMICK);
	REGISTER_OBJECT_TYPE(ObjectBlockBreakable, OCT_GIMMICK);

	// �A�C�e��
	REGISTER_OBJECT_TYPE(ObjectCoin, OCT_ITEM);
	REGISTER_OBJECT_TYPE(ObjectCoinGroupCircle, OCT_ITEM);
	REGISTER_OBJECT_TYPE(ObjectCoinGroupLinear, OCT_ITEM);

	// �J����
	REGISTER_OBJECT_TYPE(ObjectCamera, OCT_CAMERA);
	REGISTER_OBJECT_TYPE(ObjectCameraDebug, OCT_CAMERA);
	REGISTER_OBJECT_TYPE(ObjectCameraPlayer, OCT_CAMERA);

	// ���C�g
	REGISTER_OBJECT_TYPE(ObjectLight, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightPoint, OCT_LIGHT);
	REGISTER_OBJECT_TYPE(ObjectLightSpot, OCT_LIGHT);

	// �V�X�e��
	REGISTER_OBJECT_TYPE(ObjectGameStateManager, OCT_SYSTEM);

	// ���̑�
	REGISTER_OBJECT_TYPE(ObjectSkyBox, OCT_OTHER);
	REGISTER_OBJECT_TYPE(ObjectGoal, OCT_OTHER);
	REGISTER_OBJECT_TYPE(ObjectRespawn, OCT_OTHER);


}

