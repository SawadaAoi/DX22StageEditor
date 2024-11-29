/* ========================================
	DX22Base/
	------------------------------------
	プレイヤーオブジェクト用cpp
	------------------------------------
	ObjectPlayer.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectPlayer.h"
#include "ComponentTransform.h"
#include "ComponentGeometry.h"
#include "MathUtils.h"
#include "CameraManager.h"
#include "ComponentPlayerController.h"
#include "ComponentGroundRaycast.h"
#include "ShapeLine.h"
#include "Input.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionOBB.h"
#include "ComponentModelStatic.h"
#include "ComponentModelAnime.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectPlayer::ObjectPlayer(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pTransform(nullptr)
	, m_pGeometry(nullptr)
{
	
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectPlayer::InitLocal()
{
	m_pTransform =GetComponent<ComponentTransform>();



	/*m_pGeometry = AddComponent<ComponentGeometry>();
	m_pGeometry->SetShapeType(ComponentGeometry::TYPE_BOX);*/

	//AddComponent<ComponentModelStatic>();
	ComponentModelAnime*  a = AddComponent<ComponentModelAnime>();
	a->SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_PLAYER));
	a->PlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE, true, 1.0f);

	//m_pLine = std::make_unique<ShapeLine>(Vector3<float>(0.0f, 0.0f, 0.0f), m_pTransform->GetForwardVector());

	m_pGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.5f, 0.0f));
	m_pGroundRaycast->SetRayLength(0.1f);

	AddComponent<ComponentCollisionOBB>();
	
	AddComponent<ComponentRigidbody>();
	GetComponent<ComponentRigidbody>()->SetUseGravity(true);
	GetComponent<ComponentRigidbody>()->SetGroundDrag(0.9f);

	AddComponent<ComponentPlayerController>();


}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectPlayer::UpdateLocal()
{
	//m_pLine->UpdateLine(1,m_pTransform->GetWorldPosition(), m_pTransform->GetWorldPosition()+(m_pTransform->GetForwardVector()));


	if (m_pGroundRaycast->GetHitFlg())
	{
		Vector3<float> vPos = m_pTransform->GetWorldPosition();
		vPos.y = m_pGroundRaycast->GetHitPos().y + 0.5f;
		m_pTransform->SetLocalPosition(vPos);
	}


	if (Input::IsKeyTrigger(VK_SPACE))
	{
		m_pTransform->TranslateY(1.0f);
	}
}

void ObjectPlayer::DrawLocal()
{
	//m_pLine->Draw();
}
