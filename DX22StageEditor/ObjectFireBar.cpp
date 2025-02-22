/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ファイヤーバー)用cpp
	------------------------------------
	ObjectFireBar.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectFireBar.h"
#include "SceneBase.h"
#include "ObjectBlockGroundAABB.h"
#include "ComponentBillboardAnime.h"
#include "ComponentCollisionAABB.h"
#include "ComponentCollisionSphere.h"
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "TextureManager.h"


// =============== 定数定義 =======================
const int DEFAULT_FIREBALL_NUM = 5;	// 火の玉の数
const float DEFAULT_FIREBALL_COL_SCALE = 0.35f;	// 火の玉のスケール
const float DEFAULT_FIREBALL_SPEED = 30.0f;	// 火の玉の回転速度
const float DEFAULT_FIREBALL_SCALE = 1.5f;	// 火の玉のスケール

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
=========================================== */
ObjectFireBar::ObjectFireBar(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCenterBlock(nullptr)
	, m_pFireBallParent(nullptr)
	, m_nFireBallNum(DEFAULT_FIREBALL_NUM)
	, m_fRotateSpeed(DEFAULT_FIREBALL_SPEED)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
void ObjectFireBar::InitLocal()
{
	InitCenterBlock();
	InitFireBall();
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectFireBar::UpdateLocal()
{
	m_pFireBallParent->GetTransform()->RotateY(m_fRotateSpeed * DELTA_TIME);
}

/* ========================================
	中心ブロック初期化関数
	-------------------------------------
	内容：中心ブロックの初期化を行う
=========================================== */
void ObjectFireBar::InitCenterBlock()
{
	m_pCenterBlock = m_pOwnerScene->AddSceneObject<ObjectBlockGroundAABB>("CenterBlock");
	m_pCenterBlock->SetIsSave(false);	// 子オブジェクトはセーブしない
	m_pCenterBlock->GetComponent<ComponentGeometry>()->SetTextureAll(GET_TEXTURE_DATA(TEX_KEY::BLOCK_SIMPLE));

	AddChildObject(m_pCenterBlock);
}

/* ========================================
	火の玉初期化関数
	-------------------------------------
	内容：火の玉の初期化を行う
=========================================== */
void ObjectFireBar::InitFireBall()
{
	m_pFireBallParent = m_pOwnerScene->AddSceneObject<ObjectBase>("FireBallParent");
	m_pFireBallParent->SetIsSave(false);	// 子オブジェクトはセーブしない

	// 火の玉生成
	for (int i = 1; i <= m_nFireBallNum; i++)
	{
		ObjectBase* pFireBall = m_pOwnerScene->AddSceneObject<ObjectBase>("FireBall");
		pFireBall->SetTag(E_ObjectTag::Enemy);
		pFireBall->SetIsSave(false);	// 子オブジェクトはセーブしない
		pFireBall->AddComponent<ComponentBillboardAnime>()->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_FIREBALL));
		pFireBall->GetComponent<ComponentBillboardAnime>()->Play(4, { 4, 1 }, 0.5f, true);
		pFireBall->AddComponent<ComponentCollisionSphere>()->SetRadius(DEFAULT_FIREBALL_COL_SCALE);
		pFireBall->GetComponent<ComponentCollisionSphere>()->SetTrigger(true);
		pFireBall->GetComponent<ComponentCollisionSphere>()->SetRefOwnerTransform(false);
		pFireBall->GetTransform()->SetPosition({ 0.0f + (1.0f * i), 0.0f, 0.0f });
		pFireBall->GetTransform()->Scale(DEFAULT_FIREBALL_SCALE);
		m_pFireBall.push_back(pFireBall);
		m_pFireBallParent->AddChildObject(pFireBall);
	}

	AddChildObject(m_pFireBallParent);
}

/* ========================================
	コピー関数(個別処理)
	-------------------------------------
	内容：オブジェクト個別のコピー処理
	-------------------------------------
	引数1：コピーされたオブジェクト
=========================================== */
void ObjectFireBar::CopyLocal(ObjectBase* pObject)
{
	ObjectFireBar* pCopyObj = static_cast<ObjectFireBar*>(pObject);

	// 火の玉数
	pCopyObj->m_nFireBallNum = m_nFireBallNum;
	// 回転速度
	pCopyObj->m_fRotateSpeed = m_fRotateSpeed;
}


/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectFireBar::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// 火の玉数
	data.nFireBallNum = m_nFireBallNum;
	// 回転速度
	data.fRotateSpeed = m_fRotateSpeed;

	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectFireBar::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	file.read((char*)&data, sizeof(S_SaveData));

	// 火の玉数
	m_nFireBallNum = data.nFireBallNum;
	// 回転速度
	m_fRotateSpeed = data.fRotateSpeed;
}

/* ========================================
	セッター(火の玉数)関数
	-------------------------------------
	引数：int 火の玉数
=========================================== */
void ObjectFireBar::SetFireBallNum(int nFireBallNum)
{
	m_nFireBallNum = nFireBallNum;
}

/* ========================================
	セッター(回転速度)関数
	-------------------------------------
	引数：float 回転速度
=========================================== */
void ObjectFireBar::SetRotateSpeed(float fRotateSpeed)
{
	m_fRotateSpeed = fRotateSpeed;
}

