#include "SceneGameTest.h"
#include "ObjectPlayer.h"
#include "ObjectGround.h"

#include "Input.h"

#include "ComponentTransform.h"
#include "CameraManager.h"
#include "ComponentCameraBase.h"
#include "ComponentGeometry.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ComponentCollisionOBB.h"
#include "SceneManager.h"
#include "UIComponentTransform.h"
#include "UIComponentSprite.h"

#include "ComponentModelAnime.h"
#include "ModelAnimeManager.h"

void SceneGameTest::InitLocal()
{
	//m_pPlayer = AddSceneObject<ObjectPlayer>("Player");
	//m_pPlayer->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(-2.0f, 3.0f, 0.0f));


	//ObjectBase* ptest = AddSceneObject<ObjectBase>("Sphere");
	//ptest->AddComponent<ComponentCollisionSphere>();
	//ptest->AddComponent<ComponentGeometry>();
	//ptest->GetComponent<ComponentGeometry>()->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_SPHERE);
	//ptest->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(0.0f, 0.5f, 0.0f));

	//ObjectBase* ptest2 = AddSceneObject<ObjectBase>("Box");
	//ptest2->AddComponent<ComponentCollisionAABB>();
	//ptest2->AddComponent<ComponentGeometry>();
	//ptest2->GetComponent<ComponentGeometry>()->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_BOX);
	//ptest2->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(1.5f, 0.5f, 0.0f));


	//AddSceneObject<ObjectGround>("Ground");

	//m_StaticPlane = GetSceneObject<ObjectGround>("Ground");
	//m_StaticPlane->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_StaticPlane->GetComponent<ComponentTransform>()->SetLocalScale(Vector3(5.0f, 0.0f, 5.0f));
	////m_StaticPlane->GetComponent<ComponentTransform>()->RotateZ(50.0f);	// ‰ñ“]

	//m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));
	//m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));
	//m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));

}

void SceneGameTest::UpdateLocal()
{

}
