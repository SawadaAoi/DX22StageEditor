/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ジャンプバネ)用cpp
	------------------------------------
	ObjectJumpPad.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectJumpPad.h"
#include "ObjectJumpSwitch.h"
#include "SceneBase.h"

#include "ComponentModelStatic.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentCollisionAABB.h"
#include "ComponentGroundBox.h"

#include "ModelStaticManager.h"

// =============== 定数定義 =======================
const float				STRETCH_TIME	= 0.2f;						// 伸縮時間
const Vector3<float>	SPRING_SCALE	= { 0.8f, 0.8f, 0.8f };		// バネの大きさ
const float				SWITCH_SCALE_XZ = 0.8f;						// スイッチのXZスケール
const float				SWITCH_SCALE_Y	= 0.1f;						// スイッチのYスケール

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
========================================== */
ObjectJumpPad::ObjectJumpPad(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pSpringModelObj(nullptr)
	, m_pJumpSwitchObj(nullptr)
	, m_eSpringState(E_SpringState::Stay)
	, m_fStretchTime(0.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
========================================== */
void ObjectJumpPad::InitLocal()
{
	InitSpringModel();	// バネモデルの初期化
	InitJumpSwitch();	// ジャンプスイッチの初期化
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理を行う
========================================== */
void ObjectJumpPad::UpdateLocal()
{
	// バネの伸縮処理
	switch (m_eSpringState)
	{
	case ObjectJumpPad::Stay:		// そのまま
		m_pSpringModelObj->GetTransform()->SetScale(SPRING_SCALE);
		m_pSpringModelObj->GetTransform()->SetPosition(GetTransform()->GetPosition());
		m_fStretchTime = STRETCH_TIME;
		return;
	case ObjectJumpPad::Stretch:	// 伸びる
		m_fStretchTime += DELTA_TIME;
		if (m_fStretchTime >= STRETCH_TIME)
		{
			m_eSpringState = ObjectJumpPad::Stay;
		}
		break;
	case ObjectJumpPad::Shrink:		// 縮む
		m_fStretchTime -= DELTA_TIME;
		if (m_fStretchTime <= 0.0f)
		{
			m_fStretchTime = 0.0f;
		}
		break;
	}

	// 伸縮割合計算
	float fRate = m_fStretchTime / STRETCH_TIME;

	// 高さの半分の範囲内で拡縮
	float fScaleHalf	= SPRING_SCALE.y * 0.5f;		
	float fScaleY		= fScaleHalf + (fScaleHalf * fRate);
	m_pSpringModelObj->GetTransform()->SetScale({ SPRING_SCALE.x, fScaleY, SPRING_SCALE.z });

	// ばねの底面位置が変わらないように座標を調整
	float fOffsetY = (SPRING_SCALE.y - fScaleY) * 0.5f;	// 大きさを変更した分だけ高さを下げる
	Vector3<float> vPos = GetTransform()->GetPosition();
	vPos.y -= fOffsetY;
	m_pSpringModelObj->GetTransform()->SetPosition(vPos);

}

/* ========================================
	ゲッター(伸縮率)関数
	-------------------------------------
	戻値：float 伸縮率
=========================================== */
float ObjectJumpPad::GetStretchRate()
{
	return m_fStretchTime / STRETCH_TIME;
}

/* ========================================
	セッター(バネ状態)関数
	-------------------------------------
	引数：E_SpringState バネ状態
=========================================== */
void ObjectJumpPad::SetSpringState(E_SpringState eState)
{
	m_eSpringState = eState;
}

/* ========================================
	バネモデル初期化関数
	-------------------------------------
	内容：バネモデルの初期化を行う
=========================================== */
void ObjectJumpPad::InitSpringModel()
{
	m_pSpringModelObj = m_pOwnerScene->AddSceneObject<ObjectBase>("SpringModel");
	m_pSpringModelObj->AddComponent<ComponentGroundBox>();

	// 静的モデル
	m_pSpringModelObj->AddComponent<ComponentModelStatic>()
		->SetModel(GET_MODEL_STATIC(MODEL_KEY::GIMMICK_SPRING));

	// 衝突判定
	m_pSpringModelObj->AddComponent<ComponentCollisionAABB>();

	m_pSpringModelObj->GetTransform()->SetScale(SPRING_SCALE);
	m_pSpringModelObj->SetIsSave(false);
	m_pSpringModelObj->SetTag(E_ObjectTag::Ground);

	AddChildObject(m_pSpringModelObj);
}

/* ========================================
	ジャンプスイッチ初期化関数
	-------------------------------------
	内容：ジャンプスイッチの初期化を行う
=========================================== */
void ObjectJumpPad::InitJumpSwitch()
{
	m_pJumpSwitchObj = m_pOwnerScene->AddSceneObject<ObjectJumpSwitch>("JumpSwitchCol");
	m_pJumpSwitchObj->SetIsSave(false);
	
	Vector3<float> vScale = m_pSpringModelObj->GetTransform()->GetScale();
	vScale *= SWITCH_SCALE_XZ;
	vScale.y = SWITCH_SCALE_Y;
	m_pJumpSwitchObj->GetTransform()->SetScale(vScale);

	float fOffset = this->GetTransform()->GetScale().y * 0.5f;
	m_pJumpSwitchObj->GetTransform()->TranslateY(fOffset);
	m_pSpringModelObj->AddChildObject(m_pJumpSwitchObj);
}