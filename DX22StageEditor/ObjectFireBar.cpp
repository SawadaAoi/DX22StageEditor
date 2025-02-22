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
const int	DEFAULT_FIREBALL_NUM		= 5;		// 火の玉の数
const float DEFAULT_FIREBALL_COL_SCALE	= 0.35f;	// 火の玉のスケール
const float DEFAULT_FIREBALL_SPEED		= 30.0f;	// 火の玉の回転速度
const float DEFAULT_FIREBALL_SCALE		= 1.5f;		// 火の玉のスケール

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
	, m_pFireBall()
	, m_bStartCreate(false)
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

	// 火の玉親生成
	m_pFireBallParent = m_pOwnerScene->AddSceneObject<ObjectBase>("FireBallParent");
	m_pFireBallParent->SetIsSave(false);	// 子オブジェクトはセーブしない
	AddChildObject(m_pFireBallParent);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectFireBar::UpdateLocal()
{
	if (!m_bStartCreate)
	{
		InitFireBall();
		m_bStartCreate = true;
	}

	// 火の玉数が1以下の場合は1にする
	if (m_nFireBallNum < 1)
	{
		m_nFireBallNum = 1;
	}

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

		Vector3<float> vPos = m_pFireBallParent->GetTransform()->GetPosition();
		vPos.x += i * 1.0f;
		pFireBall->GetTransform()->SetPosition(vPos);

		pFireBall->GetTransform()->Scale(DEFAULT_FIREBALL_SCALE);
		m_pFireBall.push_back(pFireBall);
		m_pFireBallParent->AddChildObject(pFireBall);
	}

}

/* ========================================
	火の玉リセット関数
	-------------------------------------
	内容：火の玉をリセットする
=========================================== */
void ObjectFireBar::ResetFireBall()
{
	for (auto& pFireBall : m_pFireBall)
	{
		pFireBall->Destroy();
	}
	m_pFireBallParent->RemoveAllChildObjects();
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



#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectFireBar::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupFireBar = Item::CreateGroup("FireBar");

	// 以下の値を変更すると生成し直す
	// コイン数
	pGroupFireBar->AddGroupItem(Item::CreateCallBack("FireBallNum", Item::Kind::Int,
		[this](bool isWrite, void* arg)
	{
		int* nNum = reinterpret_cast<int*>(arg);
		if (isWrite)
		{
			m_nFireBallNum = *nNum;
			ResetFireBall();
			InitFireBall();
		}
		else
		{
			*nNum = m_nFireBallNum;
		}
	}));

	// 回転速度
	pGroupFireBar->AddGroupItem(Item::CreateBind("RotateSpeed", Item::Kind::Float, &m_fRotateSpeed));


	window.AddItem(pGroupFireBar);
}
#endif // _DEBUG