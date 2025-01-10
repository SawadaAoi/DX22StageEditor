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

// ���̑�
#include "ObjectGoal.h"
#include "ObjectSkyBox.h"

// �n�`
#include "ObjectGround.h"
#include "ObjectBlock.h"
#include "ObjectBlockGroundAABB.h"
#include "ObjectWall.h"

// �G�L����
#include "ObjectEnemy.h"
#include "ObjectEnemyLinear.h"
#include "ObjectEnemyChase.h"
#include "ObjectEnemyRandom.h"

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


/* ========================================
	�S�I�u�W�F�N�g�o�^�֐�
	-------------------------------------
	���e�F�S�ẴI�u�W�F�N�g��o�^����
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	// �S�I�u�W�F�N�g�̊��N���X
	REGISTER_OBJECT_TYPE(ObjectBase);

	// �n�`
	REGISTER_OBJECT_TYPE(ObjectGround);
	REGISTER_OBJECT_TYPE(ObjectBlock);
	REGISTER_OBJECT_TYPE(ObjectBlockGroundAABB);
	REGISTER_OBJECT_TYPE(ObjectWall);

	// �v���C���[
	REGISTER_OBJECT_TYPE(ObjectPlayer);
	REGISTER_OBJECT_TYPE(ObjectPlayerStart);

	// ���̑�
	REGISTER_OBJECT_TYPE(ObjectSkyBox);
	REGISTER_OBJECT_TYPE(ObjectGoal);

	// �G�L����
	REGISTER_OBJECT_TYPE(ObjectEnemy);
	REGISTER_OBJECT_TYPE(ObjectEnemyLinear);
	REGISTER_OBJECT_TYPE(ObjectEnemyChase);
	REGISTER_OBJECT_TYPE(ObjectEnemyRandom);

	// �J����
	REGISTER_OBJECT_TYPE(ObjectCamera);
	REGISTER_OBJECT_TYPE(ObjectCameraDebug);
	REGISTER_OBJECT_TYPE(ObjectCameraPlayer);

	// ���C�g
	REGISTER_OBJECT_TYPE(ObjectLight);
	REGISTER_OBJECT_TYPE(ObjectLightDirectional);
	REGISTER_OBJECT_TYPE(ObjectLightPoint);
	REGISTER_OBJECT_TYPE(ObjectLightSpot);

	// �V�X�e��
	REGISTER_OBJECT_TYPE(ObjectGameStateManager);


}