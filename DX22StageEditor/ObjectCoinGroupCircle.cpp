/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(コイングループ(円)用cpp
	------------------------------------
	ObjectCoinGroupCircle.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectCoinGroupCircle.h"
#include "ObjectCoin.h"
#include "SceneBase.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

// =============== 定数定義 =======================
const int	DEFAULT_COIN_NUM	= 5;	// コイン数
const float DEFAULT_COIN_RADIUS = 2.0f;	// コイン円半径


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectCoinGroupCircle::ObjectCoinGroupCircle(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCoinList()
	, m_nCoinNum(DEFAULT_COIN_NUM)
	, m_fCircleRadius(DEFAULT_COIN_RADIUS)
	, m_bIsInitCreate(false)
	, m_bUseGravity(true)
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectCoinGroupCircle::UpdateLocal()
{	
	// 初期生成
	if (!m_bIsInitCreate)
	{
		m_bIsInitCreate = true;
		CreateCoin();
	}

}

/* ========================================
	コイン生成関数
	-------------------------------------
	内容：コインを生成する
=========================================== */
void ObjectCoinGroupCircle::CreateCoin()
{
	float fCreateRad = 0.0f;

	std::string strCoinName = "Coin_";
	for (int i = 0; i < m_nCoinNum; i++)
	{
		ObjectCoin* pCoin = m_pOwnerScene->AddSceneObject<ObjectCoin>(strCoinName + std::to_string(i));

		Vector2<float> vDirection	= Vector2<float>(cosf(fCreateRad), sinf(fCreateRad));	// 生成方向ベクトル
		Vector3<float> vOffset		= Vector3<float>(vDirection.x * m_fCircleRadius, 0.0f, vDirection.y * m_fCircleRadius);

		pCoin->GetTransform()->SetPosition(m_pCompTransform->GetPosition() + vOffset);	// 位置設定
		pCoin->GetComponent<ComponentRigidbody>()->SetUseGravity(m_bUseGravity);		// 重力使用設定
		pCoin->SetIsSave(false);														// セーブしない設定（シーン遷移時に破棄されるようにする）

		fCreateRad += MathUtils::ToRadian(360.0f / m_nCoinNum);	// 生成位置を進める(円一周をコイン数で分割した角度分)

		m_pCoinList.push_back(pCoin);

		AddChildObject(pCoin);
	}

}
/* ========================================
	コインリセット関数
	-------------------------------------
	内容：コインをリセットする
=========================================== */
void ObjectCoinGroupCircle::ResetCoin()
{
	for (auto& pCoin : m_pCoinList)
	{
		pCoin->Destroy();
	}
	m_pCoinList.clear();
}

/* ========================================
	ゲッター(重力有りフラグ)関数
	-------------------------------------
	戻値：bool 重力有りフラグ
=========================================== */
bool ObjectCoinGroupCircle::GetUseGravity()
{
	return m_bUseGravity;
}

/* ========================================
	セッター(重力有りフラグ)関数
	-------------------------------------
	引数：bool 重力有りフラグ
=========================================== */
void ObjectCoinGroupCircle::SetUseGravity(bool bUseGravity)
{
	m_bUseGravity = bUseGravity;
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectCoinGroupCircle::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// コイン数
	data.nCoinNum = m_nCoinNum;
	// コイン円半径
	data.fCoinRadius = m_fCircleRadius;
	// 重力使用フラグ
	data.bUseGravity = m_bUseGravity;

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectCoinGroupCircle::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// コイン数
	m_nCoinNum = data.nCoinNum;
	// コイン円半径
	m_fCircleRadius = data.fCoinRadius;
	// 重力使用フラグ
	m_bUseGravity = data.bUseGravity;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectCoinGroupCircle::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCoinCircle = Item::CreateGroup("CoinCircle");

	// 以下の値を変更すると生成し直す
	// コイン数
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CoinNum", Item::Kind::Int,
		[this](bool isWrite, void* arg)
	{
		int* nNum = reinterpret_cast<int*>(arg);
		if (isWrite)
		{
			m_nCoinNum = *nNum;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*nNum = m_nCoinNum;
		}
	}));

	// コイン円半径
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CircleRadius", Item::Kind::Float,
		[this](bool isWrite, void* arg)
	{
		float* fRadius = reinterpret_cast<float*>(arg);
		if (isWrite)
		{
			m_fCircleRadius = *fRadius;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*fRadius = m_fCircleRadius;
		}
	}));

	// コイン生成ボタン
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CreateCoin", Item::Kind::Command,
		[this](bool isWrite, void* arg)
	{
		ResetCoin();
		CreateCoin();
	}));

	window.AddItem(pGroupCoinCircle);
}
#endif // _DEBUG