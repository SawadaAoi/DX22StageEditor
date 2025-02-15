/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(コイングループ(直線)用cpp
	------------------------------------
	ObjectCoinGroupLinear.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectCoinGroupLinear.h"
#include "ObjectCoin.h"
#include "SceneBase.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

// =============== 定数定義 =======================
const int	DEFAULT_COIN_NUM	= 5;	// コイン数
const float DEFAULT_COIN_SPACE	= 1.0f;	// コイン間隔


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectCoinGroupLinear::ObjectCoinGroupLinear(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCoinList()
	, m_nCoinNum(DEFAULT_COIN_NUM)
	, m_fCoinSpace(DEFAULT_COIN_SPACE)
	, m_bIsInitCreate(false)
	, m_bUseGravity(true)
{
}


/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectCoinGroupLinear::UpdateLocal()
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
void ObjectCoinGroupLinear::CreateCoin()
{
	// Y軸回転角度を取得（0～2π）
	float fRad = m_pCompTransform->GetRotation().ToEulerAngle(true).y;
	fRad = MathUtils::ConvertDirectionDXToTrigonometric(fRad); // DirectXの方向を三角関数の方向に変換

	// 自身が向いている方向ベクトルを取得
	Vector2<float> vDirection = Vector2<float>(cosf(fRad), sinf(fRad));

	// コインの基準位置を計算（座標が中心になるように調整）
	float fCenterOffset = ((m_nCoinNum - 1) * m_fCoinSpace) / 2;
	Vector3<float> vBasePos = m_pCompTransform->GetPosition() -
		Vector3<float>(vDirection.x * fCenterOffset, 0.0f, vDirection.y * fCenterOffset);

	// コインの生成
	std::string strCoinName = "Coin_";
	for (int i = 0; i < m_nCoinNum; i++)
	{
		ObjectCoin* pCoin = m_pOwnerScene->AddSceneObject<ObjectCoin>(strCoinName + std::to_string(i));
		// 親が向いている方向にコインを配置
		Vector3<float> vOffset = Vector3<float>(vDirection.x * m_fCoinSpace * i, 0.0f, vDirection.y * m_fCoinSpace * i);	

		pCoin->GetTransform()->SetPosition(vBasePos + vOffset);						// 位置設定
		pCoin->GetComponent<ComponentRigidbody>()->SetUseGravity(m_bUseGravity);	// 重力使用設定
		pCoin->SetIsSave(false);													// セーブしない設定（シーン遷移時に破棄されるようにする）

		m_pCoinList.push_back(pCoin);
		AddChildObject(pCoin);
	}
}


/* ========================================
	コインリセット関数
	-------------------------------------
	内容：コインをリセットする
=========================================== */
void ObjectCoinGroupLinear::ResetCoin()
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
bool ObjectCoinGroupLinear::GetUseGravity()
{
	return m_bUseGravity;
}

/* ========================================
	セッター(重力有りフラグ)関数
	-------------------------------------
	引数：bool 重力有りフラグ
=========================================== */
void ObjectCoinGroupLinear::SetUseGravity(bool bUseGravity)
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
void ObjectCoinGroupLinear::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// コイン数
	data.nCoinNum = m_nCoinNum;
	// コイン間隔
	data.fCoinSpace = m_fCoinSpace;
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
void ObjectCoinGroupLinear::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// コイン数
	m_nCoinNum = data.nCoinNum;
	// コイン間隔
	m_fCoinSpace = data.fCoinSpace;
	// 重力使用フラグ
	m_bUseGravity = data.bUseGravity;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
======================================== */
void ObjectCoinGroupLinear::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCoinLinear = Item::CreateGroup("CoinLinear");

	// 以下の値を変更すると生成し直す
	// コイン数
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CoinNum", Item::Kind::Int,
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

	// コイン間隔
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CoinSpace", Item::Kind::Float, 
	[this](bool isWrite, void* arg)
	{
		float* fSpace = reinterpret_cast<float*>(arg);
		if (isWrite)
		{
			m_fCoinSpace = *fSpace;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*fSpace = m_fCoinSpace;
		}
	}));

	// コイン生成ボタン
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CreateCoin", Item::Kind::Command,
		[this](bool isWrite, void* arg)
	{
		ResetCoin();
		CreateCoin();
	}));

	window.AddItem(pGroupCoinLinear);
}
#endif // _DEBUG