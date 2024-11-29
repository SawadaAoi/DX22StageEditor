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
	m_pPlayer = AddSceneObject<ObjectPlayer>("Player");
	m_pPlayer->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(-2.0f, 3.0f, 0.0f));


	ObjectBase* ptest = AddSceneObject<ObjectBase>("Sphere");
	ptest->AddComponent<ComponentCollisionSphere>();
	ptest->AddComponent<ComponentGeometry>();
	ptest->GetComponent<ComponentGeometry>()->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_SPHERE);
	ptest->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(0.0f, 0.5f, 0.0f));

	ObjectBase* ptest2 = AddSceneObject<ObjectBase>("Box");
	ptest2->AddComponent<ComponentCollisionAABB>();
	ptest2->AddComponent<ComponentGeometry>();
	ptest2->GetComponent<ComponentGeometry>()->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_BOX);
	ptest2->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(1.5f, 0.5f, 0.0f));


	AddSceneObject<ObjectGround>("Ground");

	m_StaticPlane = GetSceneObject<ObjectGround>("Ground");
	m_StaticPlane->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_StaticPlane->GetComponent<ComponentTransform>()->SetLocalScale(Vector3(5.0f, 0.0f, 5.0f));
	//m_StaticPlane->GetComponent<ComponentTransform>()->RotateZ(50.0f);	// ‰ñ“]

	m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));
	m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));
	m_StaticPlane->AddChildObject(AddSceneObject<ObjectGround>("Ground"));


	UIObjectBase* UI_1 =  AddUIObject<UIObjectBase>("UIObject");
	UI_1->AddComponent<UIComponentSprite>();

	UI_1->GetComponent<UIComponentTransform>()->SetLocalPosition(Vector2<float>(0.0f, 0.0f));
	UI_1->GetComponent<UIComponentTransform>()->SetLocalScale(Vector2<float>(100.0f, 100.0f));

	UI_1->GetComponent<UIComponentSprite>()->SetIsVisible(false);

	UIObjectBase* UI_2 = AddUIObject<UIObjectBase>("UIObject2");
	UI_2->AddComponent<UIComponentSprite>();

	UI_2->GetComponent<UIComponentTransform>()->SetLocalPosition(Vector2<float>(-100.0f, 0.0f));
	UI_2->GetComponent<UIComponentTransform>()->SetLocalScale(Vector2<float>(0.5f, 0.5f));
	UI_2->GetComponent<UIComponentSprite>()->SetIsVisible(false);


	UI_1->AddChildObject(UI_2);
	AddSceneObject<ObjectCamera>("Camera");

	ObjectCamera* pCamera = AddSceneObject<ObjectCamera>("Camera");
	pCamera->GetComponent<ComponentTransform>()->SetLocalPosition(Vector3(-3.0f, 3.0f, -5.0f));
}

void SceneGameTest::UpdateLocal()
{

}
