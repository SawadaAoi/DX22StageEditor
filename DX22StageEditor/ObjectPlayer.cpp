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
	, m_pGroundRaycast(nullptr)
	, m_pRigidbody(nullptr)
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectPlayer::InitLocal()
{
	m_pTransform = GetComponent<ComponentTransform>();


	m_pGroundRaycast = AddComponent<ComponentGroundRaycast>();
	m_pGroundRaycast->SetStartPosOffset(Vector3<float>(0.0f, -0.4f, 0.0f));
	m_pGroundRaycast->SetRayLength(0.2f);

	AddComponent<ComponentCollisionOBB>();

	m_pRigidbody = AddComponent<ComponentRigidbody>();
	m_pRigidbody->SetUseGravity(true);
	m_pRigidbody->SetGroundDrag(0.9f);

	AddComponent<ComponentPlayerController>();

	AddComponent<ComponentModelAnime>();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectPlayer::UpdateLocal()
{
	//m_pLine->UpdateLine(1,m_pTransform->GetWorldPosition(), m_pTransform->GetWorldPosition()+(m_pTransform->GetForwardVector()));

	// 地面に設置している場合　かつ　ジャンプ中でない場合
	if (m_pGroundRaycast->GetHitFlg() && m_pRigidbody->GetVelocity().y <= 0.0f)
	{
		Vector3<float> vPos = m_pTransform->GetWorldPosition();
		vPos.y = m_pGroundRaycast->GetHitPos().y + 0.5f;
		m_pTransform->SetLocalPosition(vPos);
	}

}

void ObjectPlayer::DrawLocal()
{
	//m_pLine->Draw();
}
